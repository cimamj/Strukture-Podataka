#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN (10)
#define MAX (90)


struct Tree;
typedef struct Tree* Position;
typedef struct Tree
{
	int element;
	Position Left;
	Position Right;

}tree;

Position Insert(int element, Position position);
Position Create(int element);
int Print(Position root, FILE* fp);
int Replace(Position root);
int FindRandom();

int main()
{
	Position root = NULL;
	FILE* fp = fopen("dat.txt", "w");
	if (!fp)
	{
		printf("File is not opened!\n");

	}
	int num = 0;



	printf("Do you want to work with numbers from task or with random numbers?\nChose an action:\n");
	printf("1 --> Numbers from task\n");
	printf("2 --> Random numbers\n");
	printf("0 --> Exit\n");


	scanf("%d", &num);

	do {
		switch (num)
		{
		case 1:
			root = Insert(2, root);
			root = Insert(5, root);
			root = Insert(7, root);
			root = Insert(8, root);
			root = Insert(11, root);
			root = Insert(1, root);
			root = Insert(4, root);
			root = Insert(2, root);
			root = Insert(3, root);
			root = Insert(7, root);

			fprintf(fp, "Numbers from the task:\n");
			Print(root, fp);
			fprintf(fp, "\n");
			Replace(root);
			Print(root, fp);
			fprintf(fp, "\n");

			break;

		case 2:
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);
			root = Insert(FindRandom(), root);

			fprintf(fp, "\nRandom numbers:\n");
			Print(root, fp);
			fprintf(fp, "\n");
			Replace(root);
			Print(root, fp);
			fprintf(fp, "\n");
			break;

		default:
			printf("Wrong input!\nTry again!\n");
		}
		scanf("%d", &num);
	} while (num);

	fclose(fp);
	return EXIT_SUCCESS;

}

Position Insert(int element, Position position)
{
	Position temp = NULL;
	Position NewElement = NULL;
	temp = position;

	if (temp == NULL)
	{
		NewElement = Create(element);
		return NewElement;
	}
	else if (element < temp->element)
		temp->Right = Insert(element, temp->Right);
	else if (element >= temp->element)
		temp->Left = Insert(element, temp->Left);


	return temp;


}

Position Create(int element)
{
	Position NewElement = NULL;
	NewElement = (Position)malloc(sizeof(tree));

	if (!NewElement)
	{
		perror("Can't allocate memory!\n");
		return NULL;
	}

	NewElement->element = element;
	NewElement->Left = NULL;
	NewElement->Right = NULL;

	return NewElement;
}

int Print(Position root, FILE* fp)
{
	Position current = NULL;
	current = root;

	if (current != NULL)
	{
		Print(current->Left, fp);
		fprintf(fp, "%d ", current->element);
		Print(current->Right, fp);


	}


	return EXIT_SUCCESS;
}


int Replace(Position root)
{
	Position current = NULL;
	current = root;
	int sum = 0, value = 0;

	if (current->Left == NULL && current->Right == NULL)
	{
		sum = current->element;
		current->element = 0;
		return sum;

	}
	else if (current->Left != NULL && current->Right == NULL)
	{

		sum = Replace(current->Left);
		value = current->element + sum;
		current->element = sum;
		return value;

	}
	else if (current->Left == NULL && current->Right != NULL)
	{

		sum = Replace(current->Right);
		value = current->element + sum;
		current->element = sum;
		return value;

	}
	else
	{
		sum = Replace(current->Left) + Replace(current->Right);
		value = current->element + sum;
		current->element = sum;
		return value;
	}

	return EXIT_SUCCESS;

}

int FindRandom()
{
	int num = 0;
	num = MIN + (rand() % (MAX - MIN + 1));
	return num;
}
