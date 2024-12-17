#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE 120

struct _Directory;
typedef struct _Directory* DirP;
typedef struct _Directory {
	char name[MAX_SIZE];
	DirP sibling;
	DirP child;
}Directory;

struct _stack;
typedef  struct _stack* StackP;
typedef struct _stack
{
	StackP directory;
	StackP next;
}Stack;

int Menu(DirP root, StackP head);
int MakeDirectory(DirP current_directory);
DirP CreateNewDirectory(char name[]);
DirP FindLastDirectory(DirP first);
int InsertAfter(DirP last_directory, DirP new_directory);
DirP ChangeDirectory(DirP current_directory);
int DeleteAfter(StackP head);
int Push(StackP head, DirP current_directory);
DirP Pop(StackP head);
int StackInsert(StackP head, StackP new_element);
int PrintDirectories(DirP first);
int NameCheck(DirP first, char name[]);

int main()
{
	Directory root = { .name = "C:", .child = NULL, .sibling = NULL };
	Stack head = { .directory = NULL, .next = NULL };
	int status = 0;

	Push(&head, &root);

	do
	{
		status = Menu(&root, &head);

	} while (status >= 0);

	return status;
}

int Menu(DirP root, StackP head)
{
	char choice[MAX_SIZE] = { 0 };
	DirP temp = NULL;
	DirP current_directory = Pop(head);
	int status = 0;

	printf("Sto zelite napraviti:");
	scanf(" %[^\n]s", choice);

	if (strcmp(choice, "md") == 0)
	{
		status = MakeDirectory(current_directory);
		return status;
	}

	else if (strcmp(choice, "cd dir") == 0)
	{
		temp = ChangeDirectory(current_directory);
		if (!temp)
			return 0;
		else
		{
			status = Push(head, temp);
			return status;
		}
	}

	else if (strcmp(choice, "cd..") == 0)
	{
		if (head->next->next == NULL)
			return 0;

		DeleteAfter(head);
		return 0;
	}

	else if (strcmp(choice, "dir") == 0)
	{
		if (current_directory->child == NULL)
		{
			printf("There aren't any directories.\n");
			return 0;
		}

		PrintDirectories(current_directory->child);
		return 0;
	}

	else if (strcmp(choice, "exit") == 0)
		return -1;

	else
	{
		printf("Your command is not recognized. Please try again.\n");
		return 0;
	}

}

int MakeDirectory(DirP current_directory)
{
	char name[MAX_SIZE] = { 0 };
	DirP NewDirectory = NULL;
	DirP LastDirectory = NULL;

	printf("What is the name of the new file: ");
	scanf(" %[^\n]s", name);

	if (NameCheck(current_directory->child, name))
	{
		printf("Directory with that name already exists.\n");
		return 0;
	}

	NewDirectory = CreateNewDirectory(name);
	if (!NewDirectory)
		return -1;

	if (current_directory->child == NULL)
	{
		current_directory->child = NewDirectory;
	}

	else
	{
		LastDirectory = FindLastDirectory(current_directory->child);
		InsertAfter(LastDirectory, NewDirectory);
	}

	return 0;
}

DirP CreateNewDirectory(char name[])
{
	DirP NewDirectory = (DirP)malloc(sizeof(Directory));
	if (!NewDirectory)
	{
		printf("Couldn't allocate memory!");
		return NULL;
	}

	strcpy(NewDirectory->name, name);
	NewDirectory->child = NULL;
	NewDirectory->sibling = NULL;
	return NewDirectory;
}

DirP FindLastDirectory(DirP first)
{
	DirP temp = first;

	while (temp->sibling != NULL)
	{
		temp = temp->sibling;
	}

	return temp;
}

int InsertAfter(DirP last_directory, DirP new_directory)
{
	new_directory->sibling = last_directory->sibling;
	last_directory->sibling = new_directory;

	return 0;
}

DirP ChangeDirectory(DirP current_directory)
{
	char name[MAX_SIZE] = { 0 };
	DirP temp = current_directory->child;

	printf("What directory do you want to go to: ");
	scanf(" %[^\n]s", name);

	while (temp != NULL && strcmp(name, temp->name) != 0)
	{
		temp = temp->sibling;
	}

	if (!temp)
	{
		printf("The system could not find the path specified.\n");
		return NULL;
	}

	return temp;
}

int DeleteAfter(StackP head)
{
	StackP temp = head->next;

	head->next = temp->next;
	free(temp);

	return 0;
}

int Push(StackP head, DirP current_directory)
{
	StackP NewElement = (StackP)malloc(sizeof(Stack));
	if (!NewElement)
	{
		printf("Could not allocate memory!\n");
		return -1;
	}

	NewElement->directory = current_directory;
	NewElement->next = NULL;

	StackInsert(head, NewElement);

	return 0;
}

int StackInsert(StackP head, StackP new_element)
{
	new_element->next = head->next;
	head->next = new_element;

	return 0;
}

int PrintDirectories(DirP first)
{
	while (first != NULL)
	{
		printf(" %s\n", first->name);
		first = first->sibling;
	}

	return 0;
}

DirP Pop(StackP head)
{
	return head->next->directory;
}

int NameCheck(DirP first, char name[])
{
	DirP temp = first;

	while (temp != NULL)
	{
		if (strcmp(name, temp->name) == 0)
			return 1;
		temp = temp->sibling;
	}

	return 0;
}
