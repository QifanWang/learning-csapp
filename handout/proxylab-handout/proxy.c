#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_ELEMENTS 10
#define HEAD 0
#define TAIL 11

struct Cache_Element
{
    u_int16_t prev, next;
    char *req;
    char *respHead;
    char *respBody;
    int respBodySz;
};

struct
{
    struct Cache_Element ele[MAX_ELEMENTS + 2];
    int cached_cnt;
    sem_t mutex;
} Cache;

/* You won't lose style points for including this long line in your code */
static const char *http_phead = "http://";
static const char *contentlen_head = "Content-length: ";
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_conn_hdr = "Proxy-Connection: close\r\n";

void init_cache();
void destroy_cache();
int getHash(char *);
int use_cache(int hashVal, char* key, int fd);
void insert_cache(int hashVal, char* req, int reqLen, char* respHead, int headLen, char* respBody, int bodySz);
int write2buf(char* buf, int pos, const char* content);
void toBeleast(int hashVal);

void *thread(void *);
void do_it(int);
void extract_hostname(char *, char *);
void extract_portnum(char *, char *);
void extract_newUri(char *, char *);
void proxyerror(int, char *, char *, char *, char *);

int main(int argc, char **argv)
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clinetaddr;
    pthread_t tid;

    if (argc != 2)
    {
        printf("Usage: %s <port_number>\n", argv[0]);
        exit(0);
    }

    init_cache();

    // argv[1] is listen port
    listenfd = Open_listenfd(argv[1]);

    clientlen = sizeof(struct sockaddr_storage);
    while (1)
    {
        connfdp = Malloc(sizeof(int));

        *connfdp = Accept(listenfd, (SA *)&clinetaddr, &clientlen);
        Pthread_create(&tid, NULL, thread, connfdp);
    }

    destroy_cache();

    return 0;
}

void *thread(void *vargp)
{
    Pthread_detach(pthread_self());

    int connfd = *((int *)vargp);
    Free(vargp);
    // do work
    do_it(connfd);

    Close(connfd);
    return NULL;
}

void do_it(int fd)
{

    char buf[MAXLINE], method[MAXLINE], oldUrl[MAXLINE], version[MAXLINE];
    char hostName[MAXLINE], port[MAXLINE], newUrl[MAXLINE];
    char req[MAXLINE], cacheBuf[MAXLINE];
    int fd2server;
    rio_t rio, rio2server;

    /* client -> proxy starts */
    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);

    sscanf(buf, "%s %s %s", method, oldUrl, version);
    if (strcasecmp(method, "GET"))
    {
        proxyerror(fd, method, "501", "Not Implemented",
                   "Proxy does not implement this method");
        return;
    }

    // store original method
    strcpy(req, buf);

    int http_phead_len = strlen(http_phead);
    int content_len = 0;
    char *colon = strstr(oldUrl + http_phead_len, ":");
    char *forwardSlash = strstr(oldUrl + http_phead_len, "/");

    extract_hostname(hostName, oldUrl + http_phead_len);
    extract_portnum(port, colon);
    extract_newUri(newUrl, forwardSlash);

    // browser sends any additional req heads
    Rio_readlineb(&rio, buf, MAXLINE);
    while (strcmp(buf, "\r\n"))
    { // read until \r\n
        // actually ignore
        // Rio_writen(fd2server, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
    }

    /* client -> proxy ends */

    /* client <- proxy try to use cache*/
    int hashVal = getHash(req);
    if (use_cache(hashVal, req, fd))
    {
        return;
    }
    /* client <- proxy ends */

    /* proxy -> server starts */
    fd2server = Open_clientfd(hostName, port);
    Rio_readinitb(&rio2server, fd2server);

    sprintf(buf, "%s %s %s\r\n", method, newUrl, "HTTP/1.0");
    Rio_writen(fd2server, buf, strlen(buf));

    sprintf(buf, "Host: %s\r\n", hostName);
    Rio_writen(fd2server, buf, strlen(buf));

    sprintf(buf, "%s\r\n", user_agent_hdr);
    Rio_writen(fd2server, buf, strlen(buf));

    sprintf(buf, "%s\r\n", connection_hdr);
    Rio_writen(fd2server, buf, strlen(buf));

    sprintf(buf, "%s\r\n", proxy_conn_hdr);
    Rio_writen(fd2server, buf, strlen(buf));

    sprintf(buf, "\r\n");
    Rio_writen(fd2server, buf, strlen(buf));

    /* proxy -> server ends */

    // client <- proxy <- server Updates Cache
    int posOfRespHead = 0;
    Rio_readlineb(&rio2server, buf, MAXLINE);
    posOfRespHead = write2buf(cacheBuf, posOfRespHead, buf);
    while (strcmp(buf, "\r\n"))
    {
        if (strncmp(buf, contentlen_head, strlen(contentlen_head)) == 0)
        {
            sscanf(buf, "Content-length: %d", &content_len);
        }
        Rio_writen(fd, buf, strlen(buf));
        Rio_readlineb(&rio2server, buf, MAXLINE);
        posOfRespHead = write2buf(cacheBuf, posOfRespHead, buf);
    }
    Rio_writen(fd, buf, strlen(buf)); // \r\n

    // reponse content
    Rio_readnb(&rio2server, buf, content_len);
    Rio_writen(fd, buf, content_len);

    insert_cache(hashVal, req, strlen(req) + 1, cacheBuf, posOfRespHead + 1, buf, content_len);    

    close(fd2server);
}

void ignore_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n"))
    { // read until \r\n
        Rio_readlineb(rp, buf, MAXLINE);
    }
}

void extract_hostname(char *hostname, char *str)
{
    int i = 0;
    while (str[i] != '\0' && str[i] != '/' && str[i] != ':')
    {
        hostname[i] = str[i];
        ++i;
    }
    hostname[i] = '\0';
}

void extract_portnum(char *port, char *colon)
{
    if (colon == NULL)
    {
        port = "80";
        return;
    }

    char *cur = colon + 1;
    int i = 0;
    while (cur[i] != '\0' && cur[i] != '/')
    {
        port[i] = cur[i];
        ++i;
    }
    port[i] = '\0';
}

void extract_newUri(char *newUri, char *slash)
{
    if (slash == NULL)
    {
        newUri = "/";
        return;
    }

    char *cur = slash;
    int i = 0;
    while (cur[i] != '\0')
    {
        newUri[i] = cur[i];
        ++i;
    }
    newUri[i] = '\0';
}

/*
 * proxyerror - returns an error message to the client
 */
/* $begin proxyerror */
void proxyerror(int fd, char *cause, char *errnum,
                char *shortmsg, char *longmsg)
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor="
                 "ffffff"
                 ">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Proxy server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
/* $end proxyerror */

void init_cache()
{
    Cache.cached_cnt = 0;
    Sem_init(&Cache.mutex, 0, 1); // Binary semaphore for locking

    Cache.ele[HEAD].prev = -1;
    Cache.ele[HEAD].next = TAIL;
    Cache.ele[HEAD].req = NULL;
    Cache.ele[HEAD].respHead = NULL;
    Cache.ele[HEAD].respBody = NULL;
    Cache.ele[HEAD].respBodySz = 0;

    Cache.ele[TAIL].prev = HEAD;
    Cache.ele[TAIL].next = -1;
    Cache.ele[TAIL].req = NULL;
    Cache.ele[TAIL].respHead = NULL;
    Cache.ele[TAIL].respBody = NULL;
    Cache.ele[TAIL].respBodySz = 0;

    int i;
    for(i = 1; i <= MAX_ELEMENTS; ++i) {
        Cache.ele[i].prev = -1;
        Cache.ele[i].next = -1;
        Cache.ele[i].req = NULL;
        Cache.ele[i].respHead = NULL;
        Cache.ele[i].respBody = NULL;
        Cache.ele[i].respBodySz = 0;
    }
}

// try to use the cached data at hashVal position
// if cache hit, write cached data into fd
int use_cache(int hashVal, char* key, int fd)
{
    int ret = 0;
    P(&Cache.mutex);
    if (Cache.ele[hashVal].req != NULL &&
        strcmp(Cache.ele[hashVal].req, key) == 0)
    {

        // use cache successfully
        ret = 1;

        toBeleast(hashVal);

        // send data to client
        char* cached = Cache.ele[hashVal].respHead;
        char* end = cached;
        while((end = strstr(cached, "\r\n")) != NULL) {
            int wrBytes = end - cached;
            wrBytes += 2;
            Rio_writen(fd, cached, wrBytes);

            cached = end + 2;
        }
        Rio_writen(fd, Cache.ele[hashVal].respBody, Cache.ele[hashVal].respBodySz);
    }
    V(&Cache.mutex);

    return ret;
}

void insert_cache(int hashVal, char* req, int reqLen, char* respHead, int headLen, char* respBody, int bodySz) {
    if(headLen <= 0 || bodySz > MAX_OBJECT_SIZE) {
        return;
    }

    P(&Cache.mutex);

    if(Cache.ele[hashVal].req != NULL || Cache.cached_cnt == MAX_ELEMENTS) { // overwrite cached elements
        free(Cache.ele[hashVal].req);
        Cache.ele[hashVal].req = malloc(reqLen);
        memcpy(Cache.ele[hashVal].req, req, reqLen);
        
        free(Cache.ele[hashVal].respHead);
        Cache.ele[hashVal].respHead = malloc(headLen);
        memcpy(Cache.ele[hashVal].respHead, respHead, headLen);

        free(Cache.ele[hashVal].respBody);
        Cache.ele[hashVal].respBody = malloc(bodySz);
        memcpy(Cache.ele[hashVal].respBody, respBody, bodySz);

        Cache.ele[hashVal].respBodySz = bodySz;

        toBeleast(hashVal);
    } else { // add new elements

        Cache.ele[hashVal].req = malloc(reqLen);
        memcpy(Cache.ele[hashVal].req, req, reqLen);
        
        Cache.ele[hashVal].respHead = malloc(headLen);
        memcpy(Cache.ele[hashVal].respHead, respHead, headLen);

        Cache.ele[hashVal].respBody = malloc(bodySz);
        memcpy(Cache.ele[hashVal].respBody, respBody, bodySz);
        
        Cache.ele[hashVal].respBodySz = bodySz;

        // add to head
        Cache.ele[hashVal].next = Cache.ele[HEAD].next;
        Cache.ele[hashVal].prev = HEAD;
        Cache.ele[HEAD].next = hashVal;
        Cache.ele[Cache.ele[hashVal].next].prev = hashVal;

        ++Cache.cached_cnt;
    }

    V(&Cache.mutex);
}

void destroy_cache()
{
    int i;
    for (i = 0; i <= MAX_ELEMENTS + 1; ++i)
    {
        if  (Cache.ele[i].req != NULL)
        free(Cache.ele[i].req);
        if  (Cache.ele[i].respHead != NULL)
        free(Cache.ele[i].respHead);
        if  (Cache.ele[i].respBody != NULL)
        free(Cache.ele[i].respBody);
    }
}

// Simple hashing
int getHash(char *buf)
{
    int ret = 0;
    while (*buf != '\0')
    {
        ret += (*buf);
        ret %= MAX_ELEMENTS;
        ++buf;
    }
    return ret + 1; // ret [1, MAX_ELEMENTS]
}

// write content to buf at position pos
// return the terminating byte position
// if content is too large return -1;
int write2buf(char* buf, int pos, const char* content) {
    int len = strlen(content);
    
    if(pos < 0 || pos + len >= MAXLINE) return -1;

    int i;
    for(i = 0; i < len; ++i)
        buf[pos+i] = content[i];
    
    buf[pos+i] = '\0';
    return pos + len;
}


// move the cached element ( identified by hasVal )
// this function must be called in critical area
void toBeleast(int hashVal) {
    int prev = Cache.ele[hashVal].prev;
    int next = Cache.ele[hashVal].next;
    
    // todo use cache element and mark it least used;
    Cache.ele[prev].next = next;
    Cache.ele[next].prev = prev;

    Cache.ele[hashVal].next = Cache.ele[HEAD].next;
    Cache.ele[hashVal].prev = HEAD;

    Cache.ele[HEAD].next = hashVal;
    Cache.ele[Cache.ele[hashVal].next].prev = hashVal;
}