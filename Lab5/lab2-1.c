#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10
#define LENGTH 20
#define ENTRY struct entry

int counter=0;

struct entry{
	char names[LENGTH];
	ENTRY *next;
	int partySize;
};

ENTRY *head = NULL;
ENTRY *tail = NULL;


void insert();
void removeParty(int size);
void list();
void checkSize();
int checkDuplicate();





int main(){
	while(1){
		int input=-1;
		scanf("%d", &input);
		if(input==0){
			return 0;
		}
		else if(input==1){
			insert();
			
		}
		else if(input==2){
			int removeMe=0;
			printf("Please input party size to remove\n");
			scanf("%d\n", &removeMe);
			removeParty(removeMe);
			
		}
		else if(input==3){
			list();
			
		}
		else if(input==4){
			checkSize();
		}
	}
}

void insert(){
	char n[LENGTH];
	ENTRY *p;
	p=head;
	int s=0;
	if(counter<9){
		printf("Please insert party name and size\n");
		scanf("%s %d", n, &s);
		if(checkDuplicate(n)==1){
			strcpy(p->names, n);
			p->partySize=s;
			counter++;
			p=p->next;
			return;
		}
		else{
			printf("Someone already took that name!\n");
			return;
		}
	}		
	else{
		printf("Sorry, the list is full");
		return;
	}
	
}

void removeParty(int size){
	ENTRY *p;
	ENTRY *q;
	p=q=head;
	while(p !=NULL && p->partySize<size){
		q=p;
		p=p->next;
	}
	if(p==NULL || p->partySize != size){
		printf("Not found");
		return;
	}
	if(p == head)
		head=p->next;
	else	
		q->next=p->next;	
}

void list(){
	ENTRY *p;
	while(p !=NULL){
		if(p->partySize != 0)
		printf("Name %s Size %d\n", p->names, p->partySize);
		p=p->next;
	}
}

void checkSize(){
	int x=0;
	printf("Size to check for?\n");
	scanf("%d", &x);
	ENTRY *p;
	while(p != NULL){
		if(p->partySize<=x)
			printf("Name %s Size %d\n", p->names, p->partySize);
		p=p->next;		
	}
}

int checkDuplicate(char n[LENGTH]){
	ENTRY *p;
	while(p!=NULL){
		//printf("%s", n);
		if(strcmp(n, p->names)==0)
			return 0;
		p=p->next;
 	}
	return 1;
}
