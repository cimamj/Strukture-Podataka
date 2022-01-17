#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRMAX 1024

typedef struct _node {
    char name[STRMAX];
    struct _node* child;
    struct _node* next;
} Node;

typedef struct _historyStack {
    Node* upperDirectory;
    struct _historyStack* next;
} history;

Node* createNewStruct(char* name);
int insertSort(Node* parent, Node* newNode);
Node* findLastSmallerName(Node* refElement, Node* searchElement);
int showListDirectory(Node* parent, history* historyHEAD);
int shell(Node* treeROOT, history* historyHEAD);
int pushHistoryStack(history* HEAD, Node* upperDirectory);
Node* popHistoryStack(history* HEAD);
int argumentIsValid(char* argument);
int direcoryExistsInCurrent(Node* currentDirectory, char* directoryName);
Node* getDirecoryFromName(Node* currentDirectory, char* directoryName);
int md(Node* currentDirectory, char* argument);
Node* cd(Node* currentDirectory, Node* treeROOT, char* argument, history* historyHEAD);
int dir(Node* currentDirectory, char* argument, history* historyHEAD);


int main(void)
{
    Node* ROOT = createNewStruct("root");
    history* historyHEAD = (history*)malloc(sizeof(history));

    if (!ROOT) {
        printf("Root nije alociran!");
        return -1;
    }

    if (!historyHEAD) {
        printf("History stack nije alociran!");
        return -1;
    }

    historyHEAD->next = NULL;
    historyHEAD->upperDirectory = NULL;

    return shell(ROOT, historyHEAD);
}

Node* createNewStruct(char* name)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);

    newNode->next = NULL;
    newNode->child = NULL;

    return newNode;
}

int insertSort(Node* parent, Node* newNode)
{
    Node* firstLeftElement = parent->child;

    if (!firstLeftElement) {
        parent->child = newNode;
    }
    else {
        Node* lastSmallerElement = findLastSmallerName(parent->child, newNode);

        if (direcoryExistsInCurrent(parent, newNode->name))
            return -2; /* Element s istim imenom vec postoji. */
        else {
            lastSmallerElement->next = newNode;
        }
    }

    return 0;
}

Node* findLastSmallerName(Node* firstElementInDir, Node* searchElement)
{
    Node* P = firstElementInDir;

    while (P->next != NULL && strcmp(P->name, searchElement->name) < 0)
        P = P->next;

    return P;
}

int showListDirectory(Node* parent, history* historyHEAD)
{
    Node* currentFile = parent->child;

    printf("%s\n ", parent->name);

    if (!currentFile) {
        if (historyHEAD->next)
            printf("|-- ..\n ");

        return -3; /* Direktorij je prazan. */
    }

    do {
        if (!currentFile->child)
            printf("|-- %s\n ", currentFile->name);
        else
            printf("|-- %s*\n ", currentFile->name);
        currentFile = currentFile->next;
    } while (currentFile != NULL);

    printf("\n");

    return 0;
}

int pushHistoryStack(history* HEAD, Node* upperDirectory)
{
    history* new = (history*)malloc(sizeof(history));

    if (!new)
        return -1;

    new->next = HEAD->next;
    HEAD->next = new;
    new->upperDirectory = upperDirectory;

    return 0;
}

Node* popHistoryStack(history* HEAD)
{
    if (!HEAD->next)
        return NULL; /* Prazan stack */

    history* temp = HEAD->next;
    Node* upperDirectory = HEAD->next->upperDirectory;
    HEAD->next = HEAD->next->next;
    free(temp);

    return upperDirectory;
}

int shell(Node* treeROOT, history* historyHEAD)
{
    Node* currentDirectory = treeROOT;
    int status = 0;
    int error = 0;
    char input[STRMAX] = { 0 };
    char command[STRMAX] = { 0 };
    char argument[STRMAX] = { 0 };

    while (!status) {
        printf("%s > ", currentDirectory->name);

        memset(command, 0, STRMAX);
        memset(argument, 0, STRMAX);
        error = 0;

        fgets(input, STRMAX, stdin);
        sscanf(input, "%s %s", command, argument);

        if (!strcmp(command, "md")) {
            md(currentDirectory, argument);
        }
        else if (!strcmp(command, "cd")) {
            currentDirectory = cd(currentDirectory, treeROOT, argument, historyHEAD);
        }
        else if (!strcmp(command, "dir")) {
            dir(currentDirectory, argument, historyHEAD);
        }
        else if (!strcmp(command, "exit"))
            status = 1;
        else if (!strcmp(command, "clear") || !strcmp(command, "cls")) {
#if __linux__
            system("clear");
#elif _WIN32
            system("cls");
#endif
        }
        else
            printf("Naredba nije pronađena!\n");
    }

    return 0;
}

int argumentIsValid(char* argument)
{
    if (!strlen(argument)) {
        return 0;
    }
    return 1;
}

int direcoryExistsInCurrent(Node* currentDirectory, char* directoryName)
{
    Node* P = currentDirectory->child;

    while (P != NULL && strcmp(P->name, directoryName))
        P = P->next;

    if (P == NULL)
        return 0;

    return 1;
}

Node* getDirecoryFromName(Node* currentDirectory, char* directoryName)
{
    Node* P = currentDirectory->child;

    while (P != NULL && strcmp(P->name, directoryName))
        P = P->next;

    return P;
}

int md(Node* currentDirectory, char* argument)
{
    int error = 0;

    if (!argumentIsValid(argument))
        printf("Nije uneseno ime direktorija.\n");
    else {
        error = insertSort(currentDirectory, createNewStruct(argument));
        if (error == -2)
            printf("Direktorij već postoji.\n");
    }

    return error;
}

Node* cd(Node* currentDirectory, Node* treeROOT, char* argument, history* historyHEAD)
{
    int error = 0;

    if (!argumentIsValid(argument))
        printf("Nije uneseno ime direktorija.\n");
    else if (!strcmp(argument, "..")) {
        currentDirectory = popHistoryStack(historyHEAD);
        if (!currentDirectory) {
            printf("Nalazite se u root direktoriju.\n");
            currentDirectory = treeROOT;
        }
    }
    else if (!direcoryExistsInCurrent(currentDirectory, argument)) {
        printf("Direktorij ne postoji!\n");
    }
    else {
        error = pushHistoryStack(historyHEAD, currentDirectory);
        if (error == -1)
            printf("History stack: greska s alokacijom memorije.\n");
        else
            currentDirectory = getDirecoryFromName(currentDirectory, argument);
    }

    return currentDirectory;
}

int dir(Node* currentDirectory, char* argument, history* historyHEAD)
{
    int error = 0;

    if (argumentIsValid(argument))
        if (direcoryExistsInCurrent(currentDirectory, argument))
            error = showListDirectory(getDirecoryFromName(currentDirectory, argument), historyHEAD);
        else
            printf("Direktorij nije pronađen!\n");
    else
        error = showListDirectory(currentDirectory, historyHEAD);

    if (error == -3)
        printf("Direktorij je prazan.\n");

    return error;
}
