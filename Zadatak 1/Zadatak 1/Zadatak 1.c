#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1024
#define MAX_NAME 128
#define MAX_BODOVI 100

typedef struct{
	char ime[MAX_NAME];
	char prezime[MAX_NAME];
	double bodovi;
	
}student;

int procitajBrStud(char* imeDatoteke);
student* alocirajIUcitajStudenteIzDatoteke(char* imeDatoteke, int brStud);
void ispisiStudente(student* studenti, int brStud);

int main()
{
	char* imeDatoteke = NULL;
	int brojStud = 0;
	student* studenti = NULL;
		
	imeDatoteke = "studenti.txt";

	brojStud = procitajBrStud(imeDatoteke);

	if (brojStud <= 0)
	{
		printf("Datoteka je prazna ili se ne moze ucitati\n");
		return -1;
	}

	studenti = alocirajIUcitajStudenteIzDatoteke(imeDatoteke, brojStud);

	ispisiStudente(studenti, brojStud);

	free(studenti);
	return 0;
}

int procitajBrStud(char* imeDatoteke)
{
	int brojac = 0;
	FILE* datoteka = NULL;
	char buffer[MAX_LINE] = { 0 };

	datoteka = fopen(imeDatoteke, "r");

	if (!datoteka)
	{
		printf("Neuspjesno ucitana datoteka\n");
		return -1;
	}
	
	while (!feof(datoteka))
	{
		fgets(buffer, MAX_LINE, datoteka);
		brojac++;
	}

	fclose(datoteka);
	return brojac;
}

student* alocirajIUcitajStudenteIzDatoteke(char* imeDatoteke, int brStud)
{
	
	student* studenti = NULL;
	FILE* datoteka = NULL;
	int brojac = 0;
	
	datoteka = fopen(imeDatoteke, "r");

	if (!datoteka)
	{
		printf("Neuspjesno ucitana datoteka\n");
		return NULL;
	}

	studenti = (student*)malloc(brStud * sizeof(student));

	if (!studenti)
	{
		printf("Neuspjesna alokacija studenata\n");
		return NULL;
	}

	while (!feof(datoteka))
	{
		fscanf(datoteka, " %s %s %lf", studenti[brojac].ime, studenti[brojac].prezime, &studenti[brojac].bodovi);
		brojac++;
	}

	fclose(datoteka);
	return studenti;

}

void ispisiStudente(student* studenti, int brStud)
{
	int i = 0;
	double maksBrBodStud = 0;

	maksBrBodStud = studenti[0].bodovi;

	for (i = 0;i < brStud;i++)
		if (studenti[i].bodovi > maksBrBodStud)
			maksBrBodStud = studenti[i].bodovi;


	printf("IME\tPREZIME\tAPSOL BR BOD\tREL BR BOD\n");

	for (i = 0; i < brStud; i++)
	{
		printf("%s\t%s\t%lf\t%lf\n",
			studenti[i].ime,
			studenti[i].prezime,
			studenti[i].bodovi / MAX_BODOVI * 100,
			studenti[i].bodovi / maksBrBodStud * 100);
	}
}