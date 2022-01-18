#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIST 3
#define MAX_ROW 64
#define MAX_WORD 32

typedef struct _Branch* BranchPosition;
typedef struct _Branch {
	char* city_name;
	int city_population;
	BranchPosition left;
	BranchPosition right;
} Branch;

typedef struct _Node* NodePosition;
typedef struct _Node {
	char* country_name;
	char* file_name;
	NodePosition next;
	BranchPosition root_city;
	
} Node;

// FUNKCIJE

NodePosition createHashFunction(int);
int inputIntoHashTable(NodePosition, int, NodePosition);
int addSortedIntoHashTable(NodePosition, NodePosition);
NodePosition findPrevioustNode(NodePosition, NodePosition);
int addNodeToLastPosition(NodePosition, NodePosition);
int addIntoHashTableFirstPosition(NodePosition, NodePosition);


int inputDataIntoCountryNode(NodePosition, char*, char*);
int createHashKey(NodePosition, int);

int inputCityInfo(BranchPosition, char*, int);
int addCityToTree(BranchPosition, BranchPosition);


int readCountriesFromFile(NodePosition);
int readCitiesFromFile(NodePosition);

NodePosition createNode();
BranchPosition createBranch();
char* createString(int);

int printHashTable(NodePosition, int);
int printList(NodePosition);
int printTree(BranchPosition);

// MAIN --------------------------------------------------------------------------------------

int main() {
	printf("\n----- START -----\n");

	NodePosition head = createHashFunction(MAX_LIST);
	readCountriesFromFile(head);
	printHashTable(head, MAX_LIST);

	printf("\n\n");
	return 1;
}

// FUNKCIJE ----------------------------------------------------------------------------------


NodePosition createHashFunction(int hash_rows) {
	printf("\ncreating hash function with %d rows...", hash_rows);
	int i = 0;

	NodePosition head_hash_table = (NodePosition)malloc(hash_rows * sizeof(Node));
	if (head_hash_table == NULL) {
		
		printf("\nError creating hash function!");
		return 0;
	}

	for (i = 0; i < hash_rows; i++) {
		(head_hash_table + i)->next = NULL;
		(head_hash_table + i)->root_city = NULL;
	}
	printf("\nhash function created successfully");
	return head_hash_table;
}

int inputIntoHashTable(NodePosition head, int row, NodePosition country) {
	printf("\nadding '%s' into hash table", country->country_name);
	//addIntoHashTableFirstPosition(head + row, country);
	addSortedIntoHashTable(head + row, country);

	return 1;
}

int addSortedIntoHashTable(NodePosition head, NodePosition country) {
	if (head->next == NULL) {
		head->next = country;
		return 1;
	}
	NodePosition temp = NULL;
	NodePosition previousNode = NULL;
	for (temp = head->next; temp != NULL; temp = temp->next) { 
		if (strcmp(country->country_name, temp->country_name) < 0) {
			previousNode = findPrevioustNode(head, temp);
			country->next = previousNode->next; 
			previousNode->next = country;
			return 1;
		}
	}

	addNodeToLastPosition(head, country);
	return 1;
}

NodePosition findPrevioustNode(NodePosition head, NodePosition node) {
	NodePosition previousNode = NULL;
	NodePosition temp = NULL;
	for (temp = head; temp->next != NULL; temp = temp->next) {
		if (temp->next == node) return temp;
	}
	return temp;
}

int addNodeToLastPosition(NodePosition head, NodePosition node) {
	NodePosition temp = NULL;
	for (temp = head; temp->next != NULL; temp = temp->next);
	temp->next = node;
	return 1;
}

int addIntoHashTableFirstPosition(NodePosition head, NodePosition country) {
	country->next = head->next;
	head->next = country;

	return 1;
}


int inputDataIntoCountryNode(NodePosition country, char* country_name, char* file_name) {
	country->country_name = country_name;
	country->file_name = file_name;
	return 1;
}

int createHashKey(NodePosition country, int hash_rows) {
	int sum = 0, i = 0, letter_value = 0;
	char letter;
	for (i = 0; i < 5; i++) { // 5 jer zbrajamo prvih 5 ascii vrijednosti slova drzave
		letter = *(country->country_name + i);
		letter_value = (int)letter;
		sum += letter_value;
	}
	return sum % hash_rows;
}

// tree...

int inputCityInfo(BranchPosition city, char* name, int population) {
	city->city_name = name;
	city->city_population = population;
	return 1;
}

int addCityToTree(BranchPosition branch, BranchPosition city) {
	if (city->city_population > branch->city_population) {
		if (branch->left == NULL) {
			branch->left = city;
			return 1;
		}
		else addCityToTree(branch->left, city);
	}
	else {
		if (branch->right == NULL) {
			branch->right = city;
			return 1;
		}
		else addCityToTree(branch->right, city);
	}
}



int readCountriesFromFile(NodePosition head) {
	printf("\nopening 'drzave.txt' file...");
	FILE* fp = fopen("drzave.txt", "r");

	NodePosition newCountry = NULL;
	int hash_key = 0;
	char* file_row;
	char* file_name;
	char* country_name;

	printf("\nreading 'drzave.txt' file...");
	while (!feof(fp)) {
		newCountry = createNode();
		country_name = createString(MAX_WORD);
		file_name = createString(MAX_WORD);
		file_row = createString(MAX_ROW);

		fgets(file_row, MAX_ROW, fp);
		sscanf(file_row, "%s %s", country_name, file_name);

		inputDataIntoCountryNode(newCountry, country_name, file_name);

		hash_key = createHashKey(newCountry, MAX_LIST);
		inputIntoHashTable(head, hash_key, newCountry);

		readCitiesFromFile(newCountry);
	}
	printf("\nsuccessfully read 'drzave.txt' file...");

	fclose(fp);
	return 1;
}

int readCitiesFromFile(NodePosition country) {
	FILE* fp = fopen(country->file_name, "r");

	BranchPosition newCity = NULL;
	char* city_name = NULL;
	int city_population = 0;
	char* file_row = NULL;

	while (!feof(fp)) {
		newCity = createBranch();
		city_name = createString(MAX_WORD);
		file_row = createString(MAX_ROW);

		fgets(file_row, MAX_ROW, fp);
		sscanf(file_row, "%s %d", city_name, &city_population);
		inputCityInfo(newCity, city_name, city_population);
		if (country->root_city == NULL) country->root_city = newCity;
		else addCityToTree(country->root_city, newCity);
	}

	fclose(fp);
	return 1;
}


int printHashTable(NodePosition head, int hash_rows) {
	int i = 0;
	printf("\n\n\n----- HASH TABLE -----");
	for (i = 0; i < hash_rows; i++) {
		printf("\n\n%d. row: ", i + 1);
		printList(head + i);
	}
	return 1;
}

int printList(NodePosition head) {
	NodePosition temp = NULL;
	for (temp = head->next; temp != NULL; temp = temp->next) {
		printf("\n\n%s\n", temp->country_name);
		printTree(temp->root_city);
	}
	return 1;
}

int printTree(BranchPosition branch) {
	if (branch->left != NULL) printTree(branch->left);
	printf("\ncity: %s, population: %d", branch->city_name, branch->city_population);
	if (branch->right != NULL) printTree(branch->right);
	return 1;
}

NodePosition createNode() {
	NodePosition temp = (NodePosition)malloc(sizeof(Node));
	if (temp == NULL) {
		printf("\n\tError while allocating memory for 'Node'");
		return NULL;
	}
	temp->next = NULL;
	temp->root_city = NULL;
	return temp;
}

BranchPosition createBranch() {
	BranchPosition temp = (BranchPosition)malloc(sizeof(Branch));
	if (temp == NULL) {
		printf("\n\tError while alloctaing memory for 'Branch'");
		return NULL;
	}
	temp->city_population = 0;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

char* createString(int length) {
	char* string = (char*)malloc(length * sizeof(char));
	return string;
}