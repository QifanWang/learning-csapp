#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* Default HTTP port */
// #define HTTP_PORT 80

/* You won't lose style points for including this long line in your code */
static const char *http_phead = "http://";
static const char *contentlen_head = "Content-length: ";
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_conn_hdr = "Proxy-Connection: close\r\n";

void *thread(void*);
void do_it(int);
void forward(int, char *, char *, char *);
void extract_hostname(char *, char*);
void extract_portnum(char *, char*);
void extract_newUri(char*, char*);
void proxyerror(int, char *, char *, char *, char *);

int main(int argc, char **argv)
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clinetaddr;
    pthread_t tid;

    if (argc != 2) {
        printf("Usage: %s <port_number>\n", argv[0]);
        exit(0);
    }

    
    // argv[1] is listen port
    listenfd = Open_listenfd(argv[1]);

    clientlen = sizeof(struct sockaddr_storage);
    while(1) {
        connfdp = Malloc(sizeof(int));

        *connfdp = Accept(listenfd, (SA*) &clinetaddr, &clientlen);
        Pthread_create(&tid, NULL, thread, connfdp);
    }
    
    return 0;
}

void *thread(void *vargp) {
    Pthread_detach(pthread_self());

    int connfd = *((int *)vargp);
    Free(vargp);
    // do work
    do_it(connfd);

    Close(connfd);
    return NULL;
}

void do_it(int fd) {

    char buf[MAXLINE], method[MAXLINE], oldUrl[MAXLINE], version[MAXLINE];
    char hostName[MAXLINE], port[MAXLINE], newUrl[MAXLINE];
    int fd2server;
    rio_t rio, rio2server;

    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);

    sscanf(buf, "%s %s %s", method, oldUrl, version);
    if (strcasecmp(method, "GET")) {                    
        proxyerror(fd, method, "501", "Not Implemented",
                    "Proxy does not implement this method");
        return;
    }

    int http_phead_len = strlen(http_phead);
    int content_len = 0;
    char* colon = strstr(oldUrl + http_phead_len, ":");
    char* forwardSlash = strstr(oldUrl + http_phead_len, "/");

    extract_hostname(hostName, oldUrl + http_phead_len);
    extract_portnum(port, colon);
    extract_newUri(newUrl, forwardSlash);
    

    fd2server = Open_clientfd(hostName, port);
    Rio_readinitb(&rio2server, fd2server);

    // client -> proxy -> server
    sprintf(buf, "%s %s %s\r\n", method, newUrl, "HTTP/1.0");
    Rio_writen(fd2server, buf, strlen(buf));

    // browser sends any additional req heads
    Rio_readlineb(&rio, buf, MAXLINE);
    while(strcmp(buf, "\r\n")) { // read until \r\n
        // ignore
        // Rio_writen(fd2server, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);;
    }

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

    // client <- proxy <- server 
    Rio_readlineb(&rio2server, buf, MAXLINE);
    while(strcmp(buf, "\r\n")) {
        if(strncmp(buf, contentlen_head, strlen(contentlen_head)) == 0) {
            sscanf(buf, "Content-length: %d", &content_len);
        }
        Rio_writen(fd, buf, strlen(buf));
        Rio_readlineb(&rio2server, buf, MAXLINE);
    }
    Rio_writen(fd, buf, strlen(buf)); // \r\n

    // reponse content
    Rio_readnb(&rio2server, buf, content_len);
    Rio_writen(fd, buf, content_len);



    close(fd2server);
    // ignore_requesthdrs(&rio);


    // todo: cache the server result
    // forward(fd, method, oldUrl, version);
}

void ignore_requesthdrs(rio_t *rp) {
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    while(strcmp(buf, "\r\n")) { // read until \r\n
        Rio_readlineb(rp, buf, MAXLINE);;
    }    
}

void forward(int fd, char *method, char *uri, char *version) {
    
    // strip uri of host domain name
    int http_phead_len = strlen(http_phead);
    char hostname[MAXLINE], port[MAXLINE], newUri[MAXLINE], buf[MAXLINE];
    int clientfd;
    rio_t rio2server;
    
    // uri doesn't start with http://
    if(strncmp(uri, http_phead, http_phead_len) != 0) {
        proxyerror(fd, "GET", "400", "Bad Request",
                "The request could not be understood by the server due to malformed syntax. \
                The client should not repeat the request without modifications.");
        return;
    }
    
    char* colon = strstr(uri + http_phead_len, ":");
    char* forwardSlash = strstr(uri + http_phead_len, "/");

    extract_hostname(hostname, uri+http_phead_len);
    extract_portnum(port, colon);
    extract_newUri(newUri, forwardSlash);

    clientfd = Open_clientfd(hostname, port);
    Rio_readinitb(&rio2server, clientfd);

    sprintf(buf, "%s %s %s\r\n", method, newUri, "HTTP/1.0");
    Rio_writen(clientfd, buf, strlen(buf));

    sprintf(buf, "Host: %s\r\n", hostname);
    Rio_writen(clientfd, buf, strlen(buf));
    
    sprintf(buf, "%s\r\n", user_agent_hdr);
    Rio_writen(clientfd, buf, strlen(buf));

    sprintf(buf, "%s\r\n", connection_hdr);
    Rio_writen(clientfd, buf, strlen(buf));

    sprintf(buf, "%s\r\n", proxy_conn_hdr);
    Rio_writen(clientfd, buf, strlen(buf));

    sprintf(buf, "\r\n");
    Rio_writen(clientfd, buf, strlen(buf));


}


void extract_hostname(char *hostname, char* str) {
    int i = 0;
    while(str[i] != '\0' && str[i] != '/' && str[i] != ':' ) {
        hostname[i] = str[i];
        ++i;
    }
    hostname[i] = '\0';
}

void extract_portnum(char *port, char* colon) {
    if(colon == NULL) {
        port = "80";
        return;
    }

    char* cur = colon + 1;
    int i = 0;
    while(cur[i] != '\0' && cur[i] != '/') {
        port[i] = cur[i];
        ++i;
    } 
    port[i] = '\0';
}

void extract_newUri(char* newUri, char* slash) {
    if(slash == NULL) {
        newUri = "/";
        return;
    }

    char* cur = slash;
    int i = 0;
    while(cur[i] != '\0') {
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
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Proxy server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
/* $end proxyerror */