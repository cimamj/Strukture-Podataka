#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR (0)
#define SUCCESS (1)

#define MAX_ROW (64)
#define MAX (32)

typedef struct _Tree* Branch;
typedef struct _List* Position;

typedef struct _Tree {
	char* ime;
	Branch left;
	Branch right; // svako stablo izgleda ka prve 3 linije

	int broj_stanovnika; // samo pod a)

	char* datoteka_gradova; //samo pod b)
	Position nultiClanListeGradova; // samo pod b)
} Tree;

typedef struct _List {
	char* ime;
	Position next; // svaka lista -||-

	Branch korijenStablaGradova; // samo pod a)
	char* datoteka_gradova; // samo pod a)

	int broj_stanovnika; // samo pod b)
} List;

// samo printf
int mainMenu();
int zad();
int printA();
int printB();
int printList(Position);
int printTrees(Position);
int printTree(Branch, int);
// main od A i B
int zadatakA();
int zadatakB();
// dodatno
int unosInfoDrzavaLista(Position, char*, char*);
// funkcije dokumenata
int fileToListDrzave(Position);
int fileToListGradovi(Branch);
int fileToTreeDrzave(Branch);
int fileToTreeGradovi(Position);
// funkcije stabla
int addToTree(Branch, Branch);
int getCityFromTree(Branch temp);
// funkcije liste
int addSortCountry(Position, Position);
Position findPrevious(Position, Position);
int addLast(Position, Position);
int getCityFromList(Position);
// funkcije alokacije
Position createPosition();
Branch createBranch();
char* createString(int);

int main() {
	int input = -1;

	while (input != 0) {
		if (mainMenu() == ERROR) return ERROR;
		if (scanf("%d", &input) == ERROR) return ERROR; // netriba ovako al evo primjer

		switch (input) {
		case 0:
			printf("\nExiting...\n");
			break;
		case 1:
			printf("\nZadatak a)\n");
			zadatakA();
			break;
		case 2:
			printf("\nZadatak b)\n");
			zadatakB();
			break;
		default:
			printf("\nPogresan unos!\n");
			continue;
		}
	}

	return SUCCESS;
}

// SAMO PRINTF FUNKCIJE

int mainMenu() {
	printf("\nDobrodosli u zadatak");
	printf("\n\n1 - Zadatak pod a)");
	printf("\n2 - Zadatak pod b)");
	printf("\n0 - Izlaz");
	printf("\n\nVas odabir: ");
	return SUCCESS;
}

int zad() {
	printf("\nUcitaj Datoteku drzave.txt formata (ime_drzave ime_datoteke_s_gradovima)\nUcitaj datoteku ime_drzave.txt formata (ime_grada broj_stanovnika)\n");
	return SUCCESS;
}
int printA() {
	zad();
	printf("\na) Postavi drzave u listu, svaka lista sadrzi korijen stabla gradova\n\n");
	return SUCCESS;
}

int printB() {
	zad();
	printf("\nb) Postavi drzave u stabla, svaka grana sadrzi listu gradova\n\n");
	return SUCCESS;
}

int printLists(Branch temp) {
	if (temp->left != NULL) printLists(temp->left);

	printf("\n\n\t----- %s -----\n\n", temp->ime);
	printList(temp->nultiClanListeGradova);

	if (temp->right != NULL) printLists(temp->right);
	return SUCCESS;
}

int printList(Position head) {
	if (head->next == NULL) {
		printf("\nPrazna Lista\n");
		return ERROR;
	}
	Position temp = head->next;
	while (temp != NULL) {
		printf("%s\n", temp->ime);
		temp = temp->next;
	}
	return SUCCESS;
}

int printTrees(Position head) {
	if (head->next == NULL) return ERROR;
	Position temp = head->next;
	while (temp != NULL) {
		printf("\n\n\t----- %s -----\n\n", temp->ime);
		printTree(temp->korijenStablaGradova, 0);
		temp = temp->next;
	}
	return SUCCESS;
}

int printTree(Branch temp, int n) {
	int i = 0;
	if (temp->left != NULL)
		printTree(temp->left, n + 1);
	for (i = 0; i < n; i++) printf("\t");
	printf("%s\n", temp->ime);
	if (temp->right != NULL)
		printTree(temp->right, n + 1);
	return SUCCESS;
}

// MAIN OD A I B

int zadatakA() {
	printA();
	Position head = createPosition();
	fileToListDrzave(head);
	printList(head);
	getCityFromList(head);
	printTrees(head);
	return SUCCESS;
}

int zadatakB() {
	printB();
	Branch root = createBranch();
	fileToTreeDrzave(root);
	printTree(root->left, 0);
	getCityFromTree(root->left);
	printLists(root->left);
	return SUCCESS;
}

// dodatno

int unosInfoDrzavaLista(Position temp, char* ime, char* datoteka) {
	temp->ime = ime;
	temp->datoteka_gradova = datoteka;
	return SUCCESS;
}

// FUNKCIJE DOKUMENATA

int fileToListDrzave(Position head) {
	FILE* fp;
	fp = fopen("drzave.txt", "r");

	char* ime;
	char* datoteka;
	char* red = createString(MAX_ROW);
	Position temp = NULL;

	while (!feof(fp)) {
		fgets(red, MAX_ROW, fp);
		if (strlen(red) < 1) return ERROR;

		ime = createString(MAX);
		datoteka = createString(MAX_ROW);

		temp = createPosition();

		sscanf(red, "%s %s", ime, datoteka);
		unosInfoDrzavaLista(temp, ime, datoteka);
		addSortCountry(head, temp);
	}

	fclose(fp);
	return SUCCESS;
}

int fileToTreeGradovi(Position current) {
	FILE* fp;
	fp = fopen(current->datoteka_gradova, "r");

	char* ime;
	char* red = createString(MAX_ROW);
	Branch temp = NULL;

	while (!feof(fp)) {
		fgets(red, MAX_ROW, fp);
		ime = createString(MAX);
		temp = createBranch();

		sscanf(red, "%s %d", ime, &temp->broj_stanovnika);
		temp->ime = ime;

		if (current->korijenStablaGradova == NULL) {
			current->korijenStablaGradova = temp;
		}
		else {
			addToTree(current->korijenStablaGradova, temp);
		}
	}

	fclose(fp);
	return SUCCESS;
}

int fileToTreeDrzave(Branch root) {
	FILE* fp;
	fp = fopen("drzave.txt", "r");

	char* ime;
	char* datoteka;
	char* red = createString(MAX_ROW);
	Branch temp = NULL;

	while (!feof(fp)) {
		fgets(red, MAX_ROW, fp);

		ime = createString(MAX);
		datoteka = createString(MAX);

		sscanf(red, "%s %s", ime, datoteka);

		temp = createBranch();
		temp->ime = ime;
		temp->datoteka_gradova = datoteka;
		if (root->left == NULL) {
			root->left = temp;
		}
		else {
			addToTree(root->left, temp);
		}
	}

	fclose(fp);
	return SUCCESS;
}

int fileToListGradovi(Branch root) {
	FILE* fp;
	fp = fopen(root->datoteka_gradova, "r");

	char* ime;
	char* red = createString(MAX_ROW);
	Position temp = NULL;

	while (!feof(fp)) {
		fgets(red, MAX_ROW, fp);
		ime = createString(MAX);
		temp = createPosition();
		sscanf(red, "%s %d", ime, &temp->broj_stanovnika);
		temp->ime = ime;

		addSortCountry(root->nultiClanListeGradova, temp);
	}

	fclose(fp);
	return SUCCESS;
}

// FUNKCIJE STABLA

int getCityFromTree(Branch temp) {
	if (temp->left != NULL) getCityFromTree(temp->left);

	fileToListGradovi(temp);

	if (temp->right != NULL) getCityFromTree(temp->right);
	return SUCCESS;
}

int addToTree(Branch temp, Branch current) {
	if (current->broj_stanovnika > temp->broj_stanovnika) {
		if (temp->left != NULL) {
			addToTree(temp->left, current);
		}
		else {
			temp->left = current;
		}
	}
	else if (current->broj_stanovnika < temp->broj_stanovnika) {
		if (temp->right != NULL)
			addToTree(temp->right, current);
		else temp->right = current;
	}
	else {
		if (strcmp(current->ime, temp->ime) < 0) {
			if (temp->left != NULL)
				addToTree(temp->left, current);
			else temp->left = current;
		}
		else {
			if (temp->right != NULL)
				addToTree(temp->right, current);
			else temp->right = current;
		}
	}
	return SUCCESS;
}

// FUNKCIJE LISTE

int addSortCountry(Position head, Position current) {
	if (head->next == NULL) {
		head->next = current;
		return SUCCESS;
	}
	Position temp = head->next;
	Position previous = NULL;

	while (temp != NULL) {
		if (strcmp(current->ime, temp->ime) < 0) {
			previous = findPrevious(head, temp);
			current->next = previous->next;
			previous->next = current;
			return SUCCESS;
		}
		temp = temp->next;
	}
	addLast(head, current);
	return SUCCESS;
}

Position findPrevious(Position head, Position current) {
	if (head->next == NULL) {
		printf("\nLista je prazna\n");
		return NULL;
	}
	Position temp = head;
	while (temp != NULL) {
		if (strcmp(current->ime, temp->next->ime) == 0) return temp;
		temp = temp->next;
	}
	printf("\nNije pronaden prethodni clan\n");
	return NULL;
}

int addLast(Position head, Position current) {
	if (head->next == NULL) {
		head->next = current;
		return SUCCESS;
	}
	Position temp = head->next;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = current;
	return SUCCESS;
}

int getCityFromList(Position head) {
	if (head->next == NULL) {
		printf("\nprazna lista");
		return ERROR;
	}
	Position temp = head->next;

	while (temp != NULL) {
		fileToTreeGradovi(temp);
		temp = temp->next;
	}
	return SUCCESS;
}

// FUNKCIJE ALOKACIJE

Position createPosition() {
	Position temp = (Position)malloc(sizeof(List));
	if (temp == NULL) return NULL;
	temp->broj_stanovnika = 0;
	temp->korijenStablaGradova = NULL;
	temp->next = NULL;
	return temp;
}

Branch createBranch() {
	Branch temp = (Branch)malloc(sizeof(Tree));
	if (temp == NULL) return NULL;
	temp->broj_stanovnika = 0;
	temp->left = NULL;
	temp->right = NULL;
	temp->nultiClanListeGradova = createPosition();
	return temp;
}

char* createString(int len) {
	char* temp = (char*)malloc(len * sizeof(char));
	return temp;
}