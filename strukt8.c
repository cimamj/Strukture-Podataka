#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

#include<stdlib.h>
#include<malloc.h>
#include<ctype.h>

#define MAX 1024
#define razmak 10


struct _stablo {
	int element;

	struct _stablo* left;
	struct _stablo* right;
};

typedef struct _stablo Stablo;

int Menu();
Stablo* Operacija(Stablo*);
int getNum();
Stablo* Unesi(Stablo*, int);
void Print(Stablo*, int);
Stablo* Find(Stablo*, int);
Stablo* PronadiNajmanji(Stablo*);
Stablo* Izbrisi(const Stablo*, Stablo*);

int main()
{
	Stablo* root;
	root = NULL;

	Menu();
	do {
		root = Operacija(root);
	} while (root);

	return 0;
}

int Menu() {
	printf("*****************************************\n");
	printf("*\t1 - Unesi novi cvor\t\t*\n");
	printf("*\t2 - Ispisi elemente stabla\t*\n");
	printf("*\t3 - Izbrisi element stabla\t*\n");
	printf("*\t4 - Ponovo ispisi izbornik\t*\n");
	printf("*\t0 - Zatvori program\t\t*\n");
	printf("*****************************************\n");

	return 0;
}
Stablo* Operacija(Stablo* root) {
	int command = 0;
	Stablo* S;

	printf("\nVas izbor -> ");
	scanf(" %d", &command);

	switch (command) {
	case 1:
		root = Unesi(root, getNum());
		return root;

	case 2:
		Print(root, 0);
		return root;

	case 3:
		S = Find(root, getNum());
		Izbrisi(root, S);
		return root;

	case 4:
		Menu();
		return root;

	case 0:
		return NULL;

	default:
		printf("Krivi unos, pokusajte ponovo!\n");
	}
}



int getNum()
{
	int n;
	printf("Unesite broj: ");
	scanf(" %d", &n);

	return n;
}

Stablo* Unesi(Stablo* root, int x)
{
	if (root == NULL) {
		root = (Stablo*)malloc(sizeof(Stablo));
		root->element = x;
		root->left = NULL;
		root->right = NULL;
		return root;
	}

	else if (x > root->element) {
		root->right = Unesi(root->right, x);
	}

	else if (x < root->element) {
		root->left = Unesi(root->left, x);
	}

	return root;
}

void Print(Stablo* root, int space) {

	int i = 0;

	if (root == NULL)
		return;

	space += razmak;

	Print(root->right, space);

	printf("\n");
	for (i = razmak; i < space; i++)
		printf(" ");
	printf("%d\n", root->element);

	Print(root->left, space);
}

Stablo* Find(Stablo* root, int pronadi)
{
	Stablo* cvor;
	cvor = root;

	if (cvor == NULL)
		return NULL;

	else if (cvor->element == pronadi) {
		return cvor;
	}

	else if (pronadi > cvor->element) {
		return Find(cvor->right, pronadi);
	}

	else if (pronadi < cvor->element) {
		return Find(cvor->left, pronadi);
	}

	return cvor;
}

Stablo* PronadiNajmanji(Stablo* root)
{
	if (root == NULL)
		return NULL;
	else if (root->left == NULL)
		return root;

	return PronadiNajmanji(root->left);
}

Stablo* Izbrisi(const Stablo* root, Stablo* del)
{
	Stablo* pomocna = (Stablo*)root;
	Stablo* temp = NULL;

	if (pomocna == NULL)
		return NULL;

	else if (del->element < pomocna->element)
		pomocna->left = Izbrisi(pomocna->left, del);

	else if (del->element > pomocna->element)
		pomocna->right = Izbrisi(pomocna->right, del);

	else if (pomocna->left && pomocna->right) {
		temp = PronadiNajmanji(pomocna->right);
		pomocna->element = temp->element;
		pomocna->right = Izbrisi(pomocna->right, pomocna);
	}

	else {
		temp = pomocna;

		if (pomocna->left == NULL)
			pomocna = pomocna->right;
		else
			pomocna = pomocna->left;

		free(temp);
	}

	return pomocna;
}