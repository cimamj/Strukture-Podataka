#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRMAX 1024

typedef struct _student {
    char ime[STRMAX];
    char prezime[STRMAX];
    int godinaRodenja;
    struct _student* next;
} Student;

int unosPocetak(Student* HEAD, Student* element);
int unosKraj(Student* HEAD, Student* element);
int ispis(Student* HEAD);
int menu(Student* HEAD);
Student* pronadiElement(Student* HEAD, char* prezime);
int brisiElement(Student* HEAD, Student* element);
int dodajElementIza(Student* element, Student* noviElement);
int dodajElementIspred(Student* HEAD, Student* element);
int spremiUDatoteku(Student* HEAD, char* filename);
int ucitajIzDatoteke(Student* HEAD, char* filename);
int sortirajPoPrezimenu(Student* HEAD);
Student* pronadiZadnjiElement(Student* HEAD);
int zamijeniPozicije(Student* HEAD, Student* prvi, Student* drugi);
Student* pronadiPrethodniElement(Student* HEAD, Student* element);
Student* unosPodataka(void);

int main(void)
{
    Student* HEAD;
    int status = 0;
    HEAD = (Student*)malloc(sizeof(Student));
    if (!HEAD)
        return -1;
    HEAD->next = NULL;

    do {
        status = menu(HEAD);
    } while (!status);

    return 0;
}

int menu(Student* HEAD)
{
    int izbor = 0;
    char prezime[STRMAX] = { 0 };
    Student* element = NULL;
    char fileName[STRMAX] = { 0 };

    printf("Odaberi zeljenu opciju:\n\t");
    printf("1. Unesi element na pocetak liste\n\t");
    printf("2. Unesi element na kraj liste\n\t");
    printf("3. Pronadi element u listi (po prezimenu)\n\t");
    printf("4. Ispisi listu\n\t");
    printf("5. Dodaj element iza nekog elementa\n\t");
    printf("6. Dodaj element ispred nekog elementa\n\t");
    printf("7. Sortiraj listu\n\t");
    printf("8. Spremi listu u datoteku\n\t");
    printf("9. Ucitaj listu iz datoteke\n\t");
    printf("10. Izbrisi element\n\t");
    printf("0. Zatvori program\n\n");

    scanf(" %d", &izbor);

    switch (izbor)
    {
    case (1):
        unosPocetak(HEAD, unosPodataka());
        break;
    case (2):
        unosKraj(HEAD, unosPodataka());
        break;
    case (3):
        printf("Unesi prezime: ");
        scanf(" %s", prezime);
        element = pronadiElement(HEAD, prezime);
        printf("%s %s %d", element->ime, element->prezime, element->godinaRodenja);
        break;
    case (4):
        ispis(HEAD);
        break;
    case (5):
        printf("Unesi prezime: ");
        scanf(" %s", prezime);
        element = pronadiElement(HEAD, prezime);
        dodajElementIza(element, unosPodataka());
        break;
    case (6):
        printf("Unesi prezime: ");
        scanf(" %s", prezime);
        element = pronadiElement(HEAD, prezime);
        dodajElementIspred(HEAD, element);
        break;
    case (7):
        sortirajPoPrezimenu(HEAD);
        ispis(HEAD);
        break;
    case (8):
        printf("Unesi ime datoteke: ");
        scanf(" %s", fileName);
        spremiUDatoteku(HEAD, fileName);
        break;
    case (9):
        printf("Unesi ime datoteke: ");
        scanf(" %s", fileName);
        ucitajIzDatoteke(HEAD, fileName);
        break;
    case (10):
        printf("Unesi prezime: ");
        scanf(" %s", prezime);
        brisiElement(HEAD, pronadiElement(HEAD, prezime));
        break;
    case (0):
        return 1;
    }
    return 0;
}

int unosPocetak(Student* HEAD, Student* element)
{

    element->next = HEAD->next;
    HEAD->next = element;

    return 0;
}

int ispis(Student* HEAD)
{
    Student* P = HEAD->next;

    while (P != NULL) {
        printf("Ime: %s\nPrezime: %s\nGodina rodenja: %d\n\n", P->ime, P->prezime, P->godinaRodenja);
        P = P->next;
    }

    return 0;
}

int unosKraj(Student* HEAD, Student* element)
{
    Student* P = pronadiZadnjiElement(HEAD);

    element->next = NULL;
    P->next = element;

    return 0;
}

Student* pronadiElement(Student* HEAD, char* prezime)
{
    Student* P = HEAD->next;

    while (P != NULL && strcmp(P->prezime, prezime)) {
        P = P->next;
    }

    return P;
}

int brisiElement(Student* HEAD, Student* element)
{
    Student* P = HEAD;

    while (P != NULL && P->next != element) {
        P = P->next;
    }

    if (P == NULL)
        return -1;
    else
    {
        P->next = P->next->next;
        free(element);
    }

    return 0;
}

int dodajElementIza(Student* element, Student* noviElement)
{
    noviElement->next = element->next;
    element->next = noviElement;

    return 0;
}

int dodajElementIspred(Student* HEAD, Student* element)
{
    Student* P = HEAD;
    Student* noviElement = (Student*)malloc(sizeof(Student));

    while (P != NULL && P->next != element)
        P = P->next;

    if (P == NULL) {
        free(noviElement);
        return -1;
    }

    dodajElementIza(P, unosPodataka());

    return 0;
}

int spremiUDatoteku(Student* HEAD, char* filename)
{
    Student* P = HEAD->next;
    FILE* fp = NULL;
    fp = fopen(filename, "w");

    if (fp == NULL)
        return -1;

    while (P != NULL) {
        fprintf(fp, "%s %s %d\n", P->ime, P->prezime, P->godinaRodenja);
        P = P->next;
    }

    fclose(fp);

    return 0;
}

int ucitajIzDatoteke(Student* HEAD, char* filename)
{
    FILE* fp;
    int brojStudenata = 0;
    int i = 0;
    Student* P = pronadiZadnjiElement(HEAD);
    char buffer[STRMAX] = { 0 };

    fp = fopen(filename, "r");

    if (fp == NULL)
        return -1;

    while (fgets(buffer, STRMAX, fp))
        brojStudenata++;

    rewind(fp);

    for (i = 0; i < brojStudenata; i++) {
        Student* noviElement = (Student*)malloc(sizeof(Student));
        noviElement->next = NULL;
        P->next = noviElement;
        P = P->next;

        fscanf(fp, "%s %s %d", noviElement->ime, noviElement->prezime, &(noviElement->godinaRodenja));
    }

    fclose(fp);
    return 0;
}

int zamijeniPozicije(Student* HEAD, Student* prvi, Student* drugi)
{
    Student* prethodni = pronadiPrethodniElement(HEAD, prvi);
    Student* temp = drugi->next;

    prethodni->next = drugi;
    drugi->next = prvi;
    prvi->next = temp;

    return 0;
}

Student* pronadiPrethodniElement(Student* HEAD, Student* element)
{
    Student* P = HEAD;

    while (P != NULL && P->next != element)
        P = P->next;

    return P;
}

Student* pronadiZadnjiElement(Student* HEAD)
{
    return pronadiPrethodniElement(HEAD, NULL);
}

int sortirajPoPrezimenu(Student* HEAD)
{
    Student* L = HEAD->next;
    Student* R = NULL;

    while (HEAD->next != R) {
        while (L->next != R) {
            if (strcmp(L->prezime, L->next->prezime) > 0) {
                zamijeniPozicije(HEAD, L, L->next);
            }
            else
                L = L->next;
        }
        R = pronadiPrethodniElement(HEAD, R);
        L = HEAD->next;
    }

    return 0;
}

Student* unosPodataka(void)
{
    Student* P = (Student*)malloc(sizeof(Student));

    if (!P)
        return NULL;

    printf("Unesite ime prezime i godinu rodenja [IME PREZIME GODINA]: ");
    scanf("%s %s %d", P->ime, P->prezime, &(P->godinaRodenja));

    return P;
}