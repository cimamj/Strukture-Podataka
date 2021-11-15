#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRMAX 1024

typedef struct _element
{
    float broj;
    struct _element* next;
} element;

int Postfix(element* HEAD, char* filename);
int Operacija(element* HEAD, char operator);
int Lista(element* HEAD, float broj);
float prviDrugi(element* HEAD);

int main()
{
    element* HEAD = (element*)malloc(sizeof(element));

    if (!HEAD)
        return -1;

    HEAD->next = NULL;
    char filename[STRMAX];

    printf("Unesi ime datoteke ");
    scanf(" %s", filename);

    Postfix(HEAD, filename);

    if (!HEAD->next)
        return -1;

    printf("%.3f\n", HEAD->next->broj);

    return 0;
}

int Postfix(element* HEAD, char* filename)
{
    FILE* fp = NULL;
    char buffer[STRMAX];
    char* pBuffer = buffer;

    fp = fopen(filename, "r");

    if (!fp)
        return -1;

    fgets(buffer, STRMAX, fp);

    fclose(fp);

    while (strlen(pBuffer) > 0)
    {
        char operator= 0;
        int br = 0;
        float broj = 0;

        if (sscanf(pBuffer, " %f%n", &broj, &br) == 1)
        {
            Lista(HEAD, broj);
            pBuffer += br;
        }
        else
        {
            sscanf(pBuffer, " %c%n", &operator, &br);
            Operacija(HEAD, operator);
            pBuffer += br;
        }
    }

    return 0;
}

int Operacija(element* HEAD, char operator)
{
    float oper1 = 0;
    float oper2 = 0;

    switch (operator)
    {
    case ('*'):
        oper1 = prviDrugi(HEAD);
        oper2 = prviDrugi(HEAD);
        Lista(HEAD, oper1 * oper2);
        break;
    case ('/'):
        oper1 = prviDrugi(HEAD);
        oper2 = prviDrugi(HEAD);
        Lista(HEAD, oper2 / oper1);
        break;
    case ('+'):
        oper1 = prviDrugi(HEAD);
        oper2 = prviDrugi(HEAD);
        Lista(HEAD, oper1 + oper2);
        break;
    case ('-'):
        oper1 = prviDrugi(HEAD);
        oper2 = prviDrugi(HEAD);
        Lista(HEAD, oper2 - oper1);
        break;
    }

    return 0;
}

int Lista(element* HEAD, float broj)
{
    element* novi = (element*)malloc(sizeof(element));

    if (!novi)
        return -1;

    novi->next = HEAD->next;
    HEAD->next = novi;
    novi->broj = broj;

    return 0;
}

float prviDrugi(element* HEAD)
{
    float broj = HEAD->next->broj;
    element* temp = HEAD->next;
    HEAD->next = HEAD->next->next;

    free(temp);
    return broj;
}