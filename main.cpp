#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <iostream>
#include <pthread.h>

#ifdef USE_EASTL

#define stl eastl

#include <EASTL/utility.h>
#include <EASTL/string.h>
#include <EASTL/map.h>

const int64_t TOTAL_ALLOCATION = 90797926;
char gHeap[TOTAL_ALLOCATION];
char* gHeapPtr = gHeap;

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    char* alloc = gHeapPtr;
    gHeapPtr += size;
    return alloc;
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    char* alloc = gHeapPtr;
    gHeapPtr += size;
    return alloc;
}

#else

#define stl std

#include <utility>
#include <string>
#include <map>

#endif

#define BUFFER_SIZE 704
typedef stl::map<stl::string, int64_t> dictionary;

struct testInfo
{
    int64_t i;
    int64_t a, b;
    dictionary* testCase;
};

dictionary gDictionary[40];
pthread_t gThreads[40];
char gOutput[40][BUFFER_SIZE];
testInfo gInfo[40];

int64_t gTestCases = 0;

void* sort(void* param)
{
    testInfo info = *(testInfo*)param;
    dictionary testCase = *info.testCase;

    uint64_t check = -1;
    stl::map<int64_t, stl::string> rank;
    stl::map<stl::string, int64_t>::iterator entry = testCase.begin();
    for(; entry != testCase.end(); entry++)
    {
        rank.insert(stl::make_pair(entry->second, entry->first));
        if(check != rank.size())
        {
            testCase.erase(entry);
            entry = testCase.begin();
            check = rank.size();
        }
    }

    int64_t i = 0;
    char* cursor = gOutput[info.i];
    memset(cursor, '\0', BUFFER_SIZE);
    strcat(cursor, "Testcase #");
    sprintf(cursor + strlen(cursor), "%d", info.i+1);
    strcat(cursor + strlen(cursor), ":\n");

    stl::map<int64_t, stl::string>::iterator finalRank = rank.end();
    finalRank--;
    info.a--;
    for(; i < info.b; finalRank--)
    {
        if(i >= info.a)
        {
            strcat(cursor + strlen(cursor), finalRank->second.c_str());
            strcat(cursor + strlen(cursor), ":");
            sprintf(cursor + strlen(cursor), "%ld\n", finalRank->first);
            while(*cursor != '\0')
            {
                cursor++;
            }
        }
        i++;
        stl::map<stl::string, int64_t>::iterator entry = testCase.begin();
        for(; entry != testCase.end() && i < info.b; entry++)
        {
            if(entry->second == finalRank->first)
            {
                if(i >= info.a)
                {
                    strcat(cursor + strlen(cursor), entry->first.c_str());
                    strcat(cursor + strlen(cursor), ":");
                    sprintf(cursor + strlen(cursor), "%ld\n", entry->second);
                    testCase.erase(entry); //No reason to iterate over this entry again
                    while(*cursor != '\0')
                    {
                        cursor++;
                    }
                }
                i++;
            }
        }
    }

    return NULL;
}

int main(int argc, char** argv)
{
    fseek(stdin, 0, SEEK_END);
    int64_t size = ftell(stdin);
    rewind(stdin);
    char* data = (char*)malloc(size);
    int64_t read = fread(data, 1, size, stdin);
    if(read != size) return -1;

    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);

    char c;
    char* current = buffer;
    int64_t lineCount; int64_t a; int64_t b;
    gTestCases = strtol(data, NULL, 10);
    int64_t cases = gTestCases;
    char* head = data;
    char* tail = NULL;
    stl::string tempString;
    tempString.reserve(64);

    int sleeve = 0;
    while(cases--)
    {
        head = strpbrk(head, "\n");
        tail = strpbrk(head+1, "\n");

        tail = strpbrk(head, " ");
        int64_t length = tail - head;

        lineCount = strtol(head, NULL, 10);
        a = strtol(tail, NULL, 10);
        head = strpbrk(tail+1, " ");
        b = strtol(head+1, NULL, 10);
        tail = strpbrk(head, "\n");
        tail++;

        gInfo[sleeve].i = sleeve;
        gInfo[sleeve].a = a;
        gInfo[sleeve].b = b;
        gInfo[sleeve].testCase = &gDictionary[sleeve];

        head = tail;
        while(lineCount--)
        {
            tail = strpbrk(head, "\n");
            length = tail - head;
            current = buffer;
            while(length--)
            {
                c = *head;
                if(c == ' ')
                {
                    *current = '\0';
                    current = buffer;
                    c = *current;
                    tempString.clear();
                    while(c != '\0')
                    {
                        if(isspace(c))
                        {
                            //Do nothing
                        }
                        else
                        {
                            tempString.append(1, c);
                        }
                        current++;
                        c = *current;
                    }
                    stl::map<stl::string, int64_t>::iterator entry = gDictionary[sleeve].find(tempString);
                    if(entry != gDictionary[sleeve].end())
                    {
                        entry->second++;
                    }
                    else if(tempString.length() >= 1)
                    {              
                        gDictionary[sleeve].insert(stl::make_pair(tempString, 1));
                    }
                    current = buffer;
                    memset(buffer, '\0', BUFFER_SIZE);
                }
                else
                { 
                    *current = c;
                    current++;
                }
                head++;
            }
            head = ++tail;
        }
        head--;
        sleeve++;
    }

    cases = gTestCases;
    sleeve = 0;
    while(cases--)
    {
        pthread_create(&gThreads[sleeve], NULL, sort, &gInfo[sleeve]);
        sleeve++;
    }

    cases = gTestCases;
    sleeve = 0;
    while(cases--)
    {
        pthread_join(gThreads[sleeve], NULL);
        if(sleeve < gTestCases-1)
            std::cout << gOutput[sleeve] << std::endl;
        else
            std::cout << gOutput[sleeve];
        sleeve++;
    }

    return 0;
}
