#include "Functions.h"


char* UnionStr(char* s1, char* s2) {
    size_t length1 = strlen(s1);
    size_t length2 = strlen(s2);

    char* result = malloc(length1 + length2 + 1);

    if (!result) {
        return NULL;
    }

    memcpy(result, s1, length1);
    memcpy(result + length1, s2, length2 + 1);

    return result;
}

char** ReadFromFile(int* linesNum, char* fileName) {
    FILE* file;
    char** fileLines = NULL;

    if (fopen_s(&file, fileName, "r") != 0) {
        return fileLines;
    }

    char tempChar = 0;

    while ((tempChar = fgetc(file)) != EOF)
        if (tempChar == '\n') (*linesNum)++;

    if (*linesNum == 0) {
        return fileLines;
    }

    fseek(file, 0, SEEK_SET);

    fileLines = (char**)malloc(sizeof(char*) * (*linesNum));

    for (int i = 0; i < (*linesNum); i++) {
        fileLines[i] = (char*)malloc(sizeof(char) * 60);
        fgets(fileLines[i], 60, file);
    }
    fclose(file);

    return fileLines;
}

void AddNodeAfter(struct worker_t* node, struct worker_t* newNode) {
    newNode->next = node->next;
    node->next = newNode;
}

void AddNodeBefore(struct worker_t* node, struct worker_t* newNode) {
    struct worker_t nodeTemp = *newNode;
    *newNode = *node;
    *node = nodeTemp;
    node->next = newNode;
}

struct worker_t* NodeCreate(char* line) {
    struct worker_t* node = (struct worker_t*)malloc(sizeof(struct worker_t));
    if ((node == 0) || (line == NULL)) {
        return NULL;
    }

    int numTemp = 0, wordNum = 0;

    while (line[wordNum] != ' ')
        node->date[numTemp++] = line[wordNum++];

    node->date[numTemp] = '\0';
    wordNum++;
    numTemp = 0;

    while (line[wordNum] != ' ')
        node->name[numTemp++] = line[wordNum++];

    node->name[numTemp] = '\0';
    wordNum++;
    numTemp = 0;

    while (line[wordNum] != ' ')
        node->surname[numTemp++] = line[wordNum++];

    node->surname[numTemp] = '\0';
    wordNum++;

    node->workHours = 0;
    while (line[wordNum] != '\n')
        node->workHours = node->workHours * 10 + CHAR_TO_INT(line[wordNum++]);

    node->next = NULL;

    return node;
}

struct worker_t* ListCreate(int linesNum, char** fileLines) {

        struct worker_t* headNode = NodeCreate(fileLines[0]);
    if (headNode == NULL)
        return NULL;

    for (int i = 1; i < linesNum; i++) {
        struct worker_t* putNode = NodeCreate(fileLines[i]);
        if (putNode == NULL)
            return NULL;

        struct worker_t* iterNode = headNode;

        while (1) {
            if (iterNode->workHours > putNode->workHours) {
                AddNodeBefore(iterNode, putNode);
                break;
            }
            else if (iterNode->workHours == putNode->workHours) {
                if (strcmp(iterNode->name, putNode->name) > 0) {
                    AddNodeBefore(iterNode, putNode);
                    break;
                }
                else if (strcmp(iterNode->name, putNode->name) == 0) {
                    if (strcmp(iterNode->surname, putNode->surname) > 0) {
                        AddNodeBefore(iterNode, putNode);
                        break;
                    }
                }
            }
            if (iterNode->next == NULL) {
                AddNodeAfter(iterNode, putNode);
                break;
            }
            iterNode = iterNode->next;
        }
    }

    return headNode;
}

char* LineOutput(struct worker_t* pHead, struct worker_t* p, int N) {
    struct worker_t* pTemp = pHead;
    int hoursSumm = 0;

    if ((pHead == NULL) || (p == NULL))
        return NULL;

    if ((strcmp(pTemp->name, p->name) == 0) && (strcmp(pTemp->surname, p->surname) == 0))
        hoursSumm += pTemp->workHours;
    while (pTemp->next != NULL) {
        pTemp = pTemp->next;
        if ((strcmp(pTemp->name, p->name) == 0) && (strcmp(pTemp->surname, p->surname) == 0))
            hoursSumm += pTemp->workHours;
    }

    if (hoursSumm >= N)
        return UnionStr(UnionStr(p->date, " "), p->surname);
    else
        return NULL;
}

void FreeList(struct worker_t* pHead) {
    struct worker_t* node = pHead;
    while (node != NULL) {
        struct worker_t* delNode = node;
        node = node->next;
        free(delNode);
    }
    free(pHead);
}