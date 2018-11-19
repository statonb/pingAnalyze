#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <getopt.h>
#include <deque>
#include <unordered_map>
#include <map>
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include "stctok.h"
#include <omp.h>

void usage(const char *arg)
{
    fprintf(stderr, "%s -i inputFile\n", arg);
    exit(0);
}

int main(int argc, char *argv[])
{
    FILE *fp = (FILE *)(NULL);
    char *inFile = (char *)(NULL);
    int opt;
    bool usageError = false;
    bool verboseFlag = false;
    bool veryVerboseFlag = false;
    uint32_t lastSeq = 0;
    uint32_t deltaThreshold = 1;

    while ((opt = getopt(argc, argv, "i:d:vVh?")) != -1)
    {
        switch (opt)
        {
        case 'i':
            inFile = optarg;
            break;
        case 'd':
            deltaThreshold = (uint32_t)strtoul(optarg, NULL, 10);
            break;
        case 'V':
            veryVerboseFlag = true;
            // intentional fall-through
        case 'v':
            verboseFlag = true;
            break;
        case 'h':
        case '?':
        default:
            usageError = true;
            break;
        }
    }

    if  (   (usageError)
         || ((char *)(NULL) == inFile)
        )
    {
        usage(argv[0]);
        return -1;
    }

    fp = fopen(inFile, "r");
    if ((FILE *)(NULL) == fp)
    {
        fprintf(stderr, "Unable to find input file\n");
        return -1;
    }

    char lineBuf[256];
    char prevLineBuf[256];
    char resultBuf[32];
    uint32_t x;
    char *ptr;
    char *tokStr = (char *)("\t ,:=");
    prevLineBuf[0] = '\0';

    while(fgets(lineBuf, sizeof(lineBuf), fp))
    {
        int n=0;
        ptr = lineBuf;
        do
        {
            ptr = stctok(ptr, resultBuf, sizeof(resultBuf), tokStr, 1);
            n++;
            if (veryVerboseFlag)
            {
                printf("%d) %s\n", n, resultBuf);
            }
            if (14 == n)
            {
                // This one is the sequence number
                uint32_t thisSeq = (uint32_t)strtoul(resultBuf, NULL, 10);
                if (verboseFlag)
                {
                    printf("%d\n", thisSeq);
                }
                uint32_t deltaSeq = thisSeq - lastSeq;
                if  (   (lastSeq != 0)
                     && ((deltaSeq) > deltaThreshold)
                    )
                {
                    printf("Check(%d %d)\n%s%s", lastSeq, deltaSeq, prevLineBuf, lineBuf);
                }
                lastSeq = thisSeq;
            }
        }
        while (ptr && *ptr);
        strcpy(prevLineBuf, lineBuf);
    }
    fclose(fp);
}
