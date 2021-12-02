#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int validBit;
    unsigned tag;
    // no data block, only simulating
    int timeStamp; // for LRU replacement policy
} CacheLine;

enum TraceOP {
    NOP,
    INSTR_LOAD,
    DATA_LOAD,
    DATA_STORE,
    DATA_MODIFY,
};

// simulate memeory accession
void simulate(int verboseFlag, int setBits, int cntLines, int blockOffset, CacheLine* cacheSpace, const char* traceFileName);

// help function convert hex character to number
inline int hex2dec(char ch);

// try to hit cache, if succeed return 1
int hitCache(CacheLine* cacheSpace, unsigned setInd, int cntLines, unsigned tag, int timeStamp);

// try to fill a record into a set, if succeed return 1
int fillCache(CacheLine* cacheSpace, unsigned setInd, int cntLines, unsigned fillTag, int timeStamp);

// evict a record in a set with LRU replcement policy
void evict(CacheLine* cacheSpace, unsigned setInd, int cntLines, unsigned fillTag, int timeStamp);

int main(int argc, char** argv)
{
    // processing arguments
    const char *helpMsg =   "Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
                            "-h: Optional help flag that prints usage info\n"
                            "-v: Optional verbose flag that displays trace info\n"
                            "-s <s>: Number of set index bits (S = 2^s is the number of sets)\n"
                            "-E <E>: Associativity (number of lines per set)\n"
                            "-b <b>: Number of block bits (B = 2^b is the block size)\n"
                            "-t <tracefile>: Name of the valgrind trace to replay";
    int verboseFlag = 0;
    int cntSetIdxBits = 0, cntLines = 0, cntBlockBits = 0;
    unsigned cntSets = 0;
    const char * traceFileName = "";
    int opt;
    while ((opt = getopt(argc, argv, "h::v::s:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            puts(helpMsg);
            return 0;
        case 'v':
            verboseFlag = 1;
            break; 
        case 's':
            cntSetIdxBits = atoi(optarg);
            cntSets = 1 << cntSetIdxBits;
            break;
        case 'E':
            cntLines = atoi(optarg);
            break;
        case 'b':
            cntBlockBits = atoi(optarg);
            break;
        case 't':
            traceFileName = optarg;
            break;
        default:
            fprintf(stderr, "Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
            exit(EXIT_FAILURE);
        }
    }
    
    unsigned long totalLines = cntLines * cntSets; 
    // cacheSpace is the first address of cache
    CacheLine* cacheSpace = malloc(sizeof(CacheLine) * totalLines);
    // init
    for(unsigned long i = 0; i < totalLines; ++i) {
        cacheSpace[i].validBit = 0;
        cacheSpace[i].tag = 0;
        cacheSpace[i].timeStamp = 0;
    }
    // simulating
    simulate(verboseFlag, cntSetIdxBits, cntLines, cntBlockBits, cacheSpace, traceFileName);
    free(cacheSpace);
    return 0;
}

void simulate(int verboseFlag, int setBits, int cntLines, int blockOffset, CacheLine* cacheSpace, const char* traceFileName) {

    int cntHits = 0, cntMisses = 0, cntEvictions = 0;

    FILE* tracefile = fopen(traceFileName, "r");
    // record time stamp
    int curTime = 0;
    
    const int MAXSIZE = 64;
    // every line of input file
    char input[MAXSIZE];
    printf("blockoffset %d, setBits %d\n", blockOffset, setBits);
    while(fgets(input, MAXSIZE, tracefile)) {
        // format: [space]operation address,size
        //“I” denotes an instruction load, “L” a data load,
        //“S” a data store, and “M” a data modify (i.e., a data load followed by a data store).
        enum TraceOP operation = NOP;
        unsigned address = 0, size = 0;
        int metCommaFlag = 0;
        char ch = '0';
        for(int i = 0; i < MAXSIZE && (ch = input[i]) != '\n'; ++i) {
            switch (ch)
            {
            case ' ':
                break;
            case 'I':
                operation = INSTR_LOAD;
                break;
            case 'L':
                operation = DATA_LOAD;
                break;
            case 'S':
                operation = DATA_STORE;
                break;
            case 'M':
                operation = DATA_MODIFY;
                break;
            case ',':
                metCommaFlag = 1;
                break;
            default: // [0-9][a-f][A-F]
                if(metCommaFlag)
                    size = (size << 4) + hex2dec(ch);
                else
                    address = (address << 4) + hex2dec(ch);
                break;
            }
        }
        // unsigned offset = address & ((1 << blockOffset) - 1);
        unsigned setIdx = (address >> blockOffset) & ((1 << setBits) - 1);
        unsigned tag = address >> (blockOffset + setBits);
        
        if(verboseFlag && operation != INSTR_LOAD) { // memory trace
            for(int i = 1; i < MAXSIZE && input[i] != '\n'; ++i)
            putchar(input[i]);
        }
        
        // debug
        // if(verboseFlag && operation != INSTR_LOAD) {
        //     printf(" address %x ", address);
        //     printf(" setInd %u", setIdx);
        // }

        // memory access
        if(operation == DATA_LOAD || operation == DATA_STORE || operation == DATA_MODIFY) {
            // access once
            if(hitCache(cacheSpace, setIdx, cntLines, tag, curTime)) {
                ++cntHits;
                if(verboseFlag) printf(" hit");
            } else {
                ++cntMisses;
                if(verboseFlag) printf(" miss");
                // miss, cache load 
                if(fillCache(cacheSpace, setIdx, cntLines, tag, curTime) == 0) {
                    evict(cacheSpace, setIdx, cntLines, tag, curTime);
                    ++cntEvictions;
                    if(verboseFlag) printf(" eviction");
                }
            }

            if(operation == DATA_MODIFY) {
                // hitCache(cacheSpace, setIdx, cntLines, tag, curTime);
                ++cntHits;
                if(verboseFlag) printf(" hit");
            }
            printf("\n");
        }

        ++curTime;
    }

    printSummary(cntHits, cntMisses, cntEvictions);
    fclose(tracefile);
}

int hex2dec(char ch) {
    if('0' <= ch && ch <= '9')
        return ch - '0';
    else if('a' <= ch && ch <= 'f')
        return ch - 'a' + 10;
    else if('A' <= ch && ch <= 'F')
        return ch - 'A' + 10;
    return 0;
}

unsigned getCacheInd(unsigned setInd, int cntLines, int offset) {
    return setInd * cntLines + offset;
}

int hitCache(CacheLine* cacheSpace, unsigned setInd, int cntLines, unsigned tag, int timeStamp) {
    for(int i = 0; i < cntLines; ++i) {
        unsigned cacheInd = getCacheInd(setInd, cntLines, i);
        if(cacheSpace[cacheInd].tag == tag && cacheSpace[cacheInd].validBit == 1) {
            cacheSpace[cacheInd].timeStamp = timeStamp;
            return 1;
        }
    }
    return 0;
}

int fillCache(CacheLine* cacheSpace, unsigned setInd, int cntLines, unsigned fillTag, int timeStamp) {
    // printf("\nfillCache set %u ", setInd);
    for(int i = 0; i < cntLines; ++i) {
        unsigned cacheInd = getCacheInd(setInd, cntLines, i);
        if(cacheSpace[cacheInd].validBit == 0) {
            // printf("line %d\n", i);
            cacheSpace[cacheInd].validBit = 1;
            cacheSpace[cacheInd].tag = fillTag;
            cacheSpace[cacheInd].timeStamp = timeStamp;
            return 1;
        }
    }
    return 0;
}

// LRU replacement policy
void evict(CacheLine* cacheSpace, unsigned setInd, int cntLines, unsigned fillTag, int timeStamp) {
    // printf("\nevict set %u\n", setInd);
    int pivotInd = 0, pivotVal = cacheSpace[getCacheInd(setInd, cntLines, 0)].timeStamp;
    for(int i = 1; i < cntLines; ++i) {
        unsigned cacheInd = getCacheInd(setInd, cntLines, i);
        if(cacheSpace[cacheInd].timeStamp < pivotVal) {
            pivotVal = cacheSpace[cacheInd].timeStamp;
            pivotInd = i;
        }
    }
    
    int evictInd = getCacheInd(setInd, cntLines, pivotInd);
    cacheSpace[evictInd].validBit = 1;
    cacheSpace[evictInd].tag = fillTag;
    cacheSpace[evictInd].timeStamp = timeStamp;
}