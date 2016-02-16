/*
 * Author: TA
 * Due Date: 11/23 Sunday midnight
 *
 * The priority of this project is to fully understand linked list behavior.
 * Given data; apply insert,delete,search
 */

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

/**
 * Account node structure
 */
typedef struct _node{
	char name[20];
	int balance;
	int accountNum;
	struct _node *next;
}node;

/**
 * Linked List structure
 */
typedef struct _list{
	int count;
	node* head;
}list;

/**
 * initialize head node
 * and set size in list
 */
void init(list* li){
	li->count = 0;
	li->head = NULL;
}

/**
 * Check, if list is empty;
 * any integer that is not 0 is true, 0 is false
 */
int empty(list* li){return (li->count == 0);}

/**
 * return size of the L.L.
 */
int getSize(list* li){return li->count;}

/**
 * insert case1;
 * add first node
 */
void insertCase1(list* li, node* newNode){
	li->head = newNode;
}

/**
 * insert case2;
 * prepend node to head
 */
void insertCase2(list* li, node* newNode){
	newNode->next = li->head;
	li->head = newNode;
}

/**
 * compare string(name) in ascending order
 */
int nameCompare(node* previous, node* current, node* newNode){
	return ((strncmp(previous->name,newNode->name,8) < 0 || strncmp(previous->name,newNode->name,8) == 0)&&
		(strncmp(newNode->name,current->name,8) < 0|| strncmp(previous->name,newNode->name,8) == 0));
}

/**
 * insert case3;
 * append node in middle/end
 */
void insertCase3(node* newNode, node* previous, node* current){
	if(current == NULL || nameCompare(previous,current,newNode)){
		previous->next = newNode;
		if(current != NULL)
			newNode->next = current;
		return;
	}
	insertCase3(newNode,previous->next,current->next);
}

/**
 * insert the node to LinkedList
 * in ascending order by name
 */
void insert(list* li, node* newNode){
	if(li->head == NULL)
		insertCase1(li,newNode);
	else if(strncmp(li->head->name,newNode->name,8) > 0)
		insertCase2(li,newNode);
	else
		insertCase3(newNode,li->head,li->head->next);
	li->count++;
}

/**
 * Search helpful function;
 * iterates
 */
node* _search(int actNum, node* previous){
	node* current = previous->next;
	return (current->accountNum == actNum)? previous: (current == NULL)? NULL: _search(actNum,current);}

/**
 * Search the client by his/her account number
 */
node* search(list*li, int actNum){
	if(empty(li))
		return NULL;
	node* previous = li->head;
	node* current = previous->next;
	if(previous->accountNum == actNum || current->accountNum == actNum)
		return previous;
	return _search(actNum, current);
}
/**
 * free memory from heap;
 * set selected node points to NULL
 */
void freeNode(node* deletedNode){
	free(deletedNode);
	deletedNode = NULL;
}

/**
 * delete case1;
 * if list is empty/The node is not found
 */
int deleteCase1(list *li, node* found){
	return (found == NULL || empty(li));
}

/**
 * delete case2;
 *  if the node is in head of the list
 */
int deleteCase2(node* head, int data){
	return (head->accountNum == data);
}

/**
 * delete case3;
 * if the node is in the middle/end of the list
 */
void deleteCase3(node *previous, node *current){
	previous->next = (current->next != NULL)? current->next: NULL;
	freeNode(current);
}

/**
 * delete node from L.L. O(n + 1) = O(n)
 * found->next is found current node
 * found is previous of found->next node
 */
void deleteNode(list *li, int data){
	node* found = search(li, data);
	if(deleteCase1(li,found->next))
		return;
	else if(deleteCase2(found,data)){
		li->head = found->next;
		freeNode(found);
	}else
		deleteCase3(found, found->next);
	li->count--;
}

/**
 * option menu display
 */
void menu(){
	printf("\n										1. Add an account \n");
	printf("										2. View current account \n");
	printf("										3. Close an account \n");
	printf("										4. Quit \n");
	printf("										5. Write account to a file \n\n");
}

/**
 * First main menu display
 */
void mainMenu(){
	printf("					Welcome to Banking Account System \n\n");
	menu();
	printf("Please choose: \n\n");
}

/**
 * convert name to upper case
 */
void toUpperCase(char *name){
	if(*name == '\0')
		return;
   *name = (char)toupper(*name);
   toUpperCase(name+1);
}

/**
 * initialize bank account;
 * random account number is limited to constant 4digits
 */
void addAccount(list* li){
	node *newNode = (node*)malloc(sizeof(node));
	printf("*)Enter your name: \n");
	scanf(" %[^\n]",newNode->name);
	printf("*)Enter balance: \n");
	scanf("%d",&newNode->balance);
	toUpperCase(newNode->name);
	newNode->accountNum = rand() % 9000+1000;
	newNode->next = NULL;
	insert(li,newNode);
}

/**
 * viewCurrentAccounts helpful function
 */
void _viewCurrentAccounts(node *ptr,int count){
	if(ptr != NULL){
		printf("%d.		%-20s		$%-20d		%-20d  \n",count,ptr->name,ptr->balance,ptr->accountNum);
		_viewCurrentAccounts(ptr->next,++count);
	}
}

/**
 * print current accounts in linked list
 */
void viewCurrentAccounts(list* li){
	if(empty(li)){
		printf("*There is no clients \n");
		return;
	}
	printf("*)Total clients: %d \n",getSize(li));
	printf("		Name: 				Balance: 			Account Number:   \n");
	int count = 1;
	_viewCurrentAccounts(li->head,count);
}

/**
 * close account by selected account number
 */
void closeAcount(list* li){
	if(empty(li)){
		printf("*)There is no clients \n");
		return;
	}
	int actNum;
	printf("*)Enter the account number that you would like to close \n");
	scanf("%d", &actNum);
	if(search(li,actNum) == NULL){
		printf("*) The account number is not in our system \n");
		return;
	}
	deleteNode(li,actNum);
	printf("*)account number: %d has been closed \n", actNum);
}

/**
 * quit bank
 * release LL
 */
int quitBank(int exit,list *li){
	printf("*)Quit \n");
	free(li);
	li = NULL;
	return 0;
}

/**
 * writeAccountsFile helpful function
 */
void _writeAccountsFile(node* ptr, FILE *writer,int count){
	if(ptr == NULL){
		fclose(writer);
		return;
	}
	fprintf(writer,"%d.		%-20s		$%-20d		%-20d  \n",count,ptr->name,ptr->balance,ptr->accountNum);
	_writeAccountsFile(ptr->next,writer,++count);
}

/**
 * Write accounts.txt file
 */
void writeAccountsFile(list *li){
	if(empty(li)){
		printf("*)There are't any clients in our system \n");
		return;
	}
	FILE *writer = fopen("accounts.txt","w+");
	fprintf(writer, "**)CS357 Bank Account Management \n*)Total Clients: %d \n\n" , getSize(li));
	fprintf(writer,"		Name: 				Balance: 			Account Number:   \n");
	int count = 1;
	_writeAccountsFile(li->head,writer,count);
	printf("*)account.txt is generated at this program's location");
}

/**
 * main function
 */
int main(int argc, char **argv){

	srand(time(NULL));
	list *li = (list*)malloc(sizeof(list));
	init(li);

	int input,exit = 1;

	mainMenu();
	while(exit){
		printf("\n**)Please choose the following option from 1-5. (Enter 6 for MAIN MENU) \n");
		scanf("%d", &input);
		switch(input){
			case 1:
				addAccount(li);
				break;
			case 2:
				viewCurrentAccounts(li);
				break;
			case 3:
				closeAcount(li);
				break;
			case 4:
				exit = quitBank(exit,li);
				break;
			case 5:
				writeAccountsFile(li);
				break;
			default:
				printf("**You have entered %d , please enter between 1-5 \n",input);
			}if(exit)menu();
		}
	return 0;
}
