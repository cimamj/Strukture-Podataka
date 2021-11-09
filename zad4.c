#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 512

typedef struct _Pol {
    int koeficijent;
    int eksponent;
    struct _Pol* next;
}Polinom;

int ucitajIzDatoteke(Polinom*, Polinom*, char*);
int citajIzDatoteke(Polinom* head, char* BUFFER);
int Sort(Polinom* head, int koef, int eksp);
Polinom* pronadiPrethodniElement(Polinom* head, Polinom* Pozicija);
int ispis(Polinom* head);
int zbrojiPolinome(Polinom* head1, Polinom* head2, Polinom* suma);
int pomnoziPolinome(Polinom* head1, Polinom* head2, Polinom* produkt);
int ocistiNulaElemente(Polinom* head);

int main()
{
    char* imeDat = "Text.txt";
    Polinom* head1 = (Polinom*)malloc(sizeof(Polinom));
    Polinom* head2 = (Polinom*)malloc(sizeof(Polinom));
    Polinom* suma = (Polinom*)malloc(sizeof(Polinom));
    Polinom* produkt = (Polinom*)malloc(sizeof(Polinom));

    head1->next = NULL;
    head2->next = NULL;
    suma->next = NULL;
    produkt->next = NULL;


    ucitajIzDatoteke(head1, head2, imeDat);

    printf("Prvi polinom: \n\t");
    ispis(head1);
    printf("\nDrugi polinom: \n\t");
    ispis(head2);

    zbrojiPolinome(head1, head2, suma);
    printf("\nSuma polinoma: \n\t");
    ispis(suma);
    pomnoziPolinome(head1, head2, produkt);
    printf("\nUmnozak polinoma: \n\t");
    ispis(produkt);
    printf("\n");

    return 0;
}

int ucitajIzDatoteke(Polinom* head1, Polinom* head2, char* imeDat)
{
    FILE* a = NULL;
    char BUFFER[MAX];
    a = fopen(imeDat, "r");

    fgets(BUFFER, MAX, a);
    citajIzDatoteke(head1, BUFFER);
    fgets(BUFFER, MAX, a);
    citajIzDatoteke(head2, BUFFER);

    fclose(a);

    return 0;
}

int citajIzDatoteke(Polinom* head, char* BUFFER)
{
    int n = 0;
    int koef = 0;
    int eksp = 0;
    BUFFER[strlen(BUFFER)] = 0;
    char* tempBUFFER = BUFFER; // tempBUFFER pomocni pokazivac

    while (strlen(tempBUFFER) > 1) {

        sscanf(tempBUFFER, " %d %d%n", &koef, &eksp, &n);
        tempBUFFER += n;

        Sort(head, koef, eksp);
    }

    return 0;
}

int Sort(Polinom* head, int koef, int eksp)
{
    Polinom* P = head;

    while (P->next != NULL && P->next->eksponent > eksp)
        P = P->next;

    if (P->next != NULL && P->next->eksponent == eksp) {
        P->next->koeficijent += koef;
    }
    else {
        Polinom* novi = (Polinom*)malloc(sizeof(Polinom));

        if (!novi)
            return -1;

        novi->next = P->next;
        P->next = novi;

        novi->koeficijent = koef;
        novi->eksponent = eksp;
    }

    return 0;
}

Polinom* pronadiPrethodniElement(Polinom* head, Polinom* Pozicija)
{
    Polinom* P = head;

    while (P != NULL && P->next != Pozicija)
        P = P->next;

    return P;
}

int ispis(Polinom* head)
{
    Polinom* P = head->next;

    while (P != NULL) {
        printf("%dx^%d", P->koeficijent, P->eksponent);
        if (P->next != NULL)
            printf(" + ");
        P = P->next;
    }

    return 0;
}

int zbrojiPolinome(Polinom* head1, Polinom* head2, Polinom* suma)
{
    Polinom* phead1 = head1->next;
    Polinom* phead2 = head2->next;

    while (phead1 != NULL) {
        Sort(suma, phead1->koeficijent, phead1->eksponent);
        phead1 = phead1->next;
    }

    while (phead2 != NULL) {
        Sort(suma, phead2->koeficijent, phead2->eksponent);
        phead2 = phead2->next;
    }

    ocistiNulaElemente(suma);

    return 0;
}

int pomnoziPolinome(Polinom* head1, Polinom* head2, Polinom* produkt)
{
    Polinom* phead1 = head1->next;
    Polinom* phead2 = head2->next;

    while (phead1 != NULL) {
        phead2 = head2->next;
        while (phead2 != NULL) {
            Sort(produkt, phead1->koeficijent * phead2->koeficijent, phead1->eksponent + phead2->eksponent);
            phead2 = phead2->next;
        }
        phead1 = phead1->next;
    }

    ocistiNulaElemente(produkt);

    return 0;
}

int ocistiNulaElemente(Polinom* head)
{
    Polinom* P = head;
    Polinom* temp;
    
    while (P->next != NULL) {
        if (!P->next->koeficijent) {
            temp = P->next;
            P->next = P->next->next;
            free(temp);
        }
        P = P->next;
    }

    return 0;
}

