#pragma once

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FILENAME "list.txt"
#define DATE_LENGHT 11
#define NAME_LENGHT 30
#define CHAR_TO_INT(x) x - 48

    struct worker_t {
        char date[DATE_LENGHT];
        char name[NAME_LENGHT];
        char surname[NAME_LENGHT];
        int workHours;
        struct worker_t* next;
    };

    char** ReadFromFile(int* linesNum, char* fileName);

    char* UnionStr(char* s1, char* s2);

    void AddNodeAfter(struct worker_t* node, struct worker_t* newNode);

    //void AddNodeBefore(struct worker_t* node, struct worker_t* newNode);

    struct worker_t* NodeCreate(char* line);

    struct worker_t* ListCreate(int linesNum, char** fileLines);

    char* LineOutput(struct worker_t* pHead, struct worker_t* p, int N);

    void FreeList(struct worker_t* pHead);

#ifdef __cplusplus
}
#endif