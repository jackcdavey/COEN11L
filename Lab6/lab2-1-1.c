#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10
#define LENGTH 20
#define ENTRY struct entry
#define LIST struct list

int counter=0;
int listNum=-1;

struct entry{
	char names[LENGTH];
	ENTRY *next;
	int partySize;
	int partyList;
};


struct list{
	ENTRY *head;
	ENTRY *tail;
};

LIST lists[5];

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
	p=lists[listNum].head;
	int s=0;
	
		if(counter<9){
			printf("Please insert party name and size\n");
			scanf("%s %d", n, &s);
			if(checkDuplicate(n)==1){	
				p=(ENTRY *)malloc(sizeof(ENTRY));
				if(s<3){
					listNum=0;
					p->partyList=0;	
				}
				else if(s<5){
					listNum=1;
					p->partyList=1;
				}
				else if(s<7){
					listNum=2;
					p->partyList=2;
				}
				else{
					listNum=3;
					p->partyList=3;
				}
			
			if(lists[listNum].head ==NULL){
				lists[listNum].head=p;
				lists[listNum].tail=p;
			}else{
				lists[listNum].tail->next=p;
				lists[listNum].tail=p;
			}
			strcpy(p->names, n);
			p->partySize=s;
			counter++;
			p->next=NULL;
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
	if(size<3)
		listNum=0;
	else if(size<5)
		listNum=1;
	else if(size<7)
		listNum=2;
	else
		listNum=3;
	ENTRY *p;
	ENTRY *q;
	listNum=0;
	
	
	while(listNum<4){
		p=q=lists[listNum].head;
		if(lists[listNum].head != NULL){
	
		while(p !=(ENTRY *)NULL && p->partySize>size){
			q=p;
			p=p->next;
		}

		if(p==(ENTRY *)NULL){
			listNum++;
		}
		else if(p == lists[listNum].head){
			lists[listNum].head=p->next;
			return;
		}
		else if(p==lists[listNum].tail){
			lists[listNum].tail=q;
			q->next=NULL;
			return;	
		}
		else{	
			q->next=p->next;
			return;
		}
		
		}
		listNum++;
		free(p);
	}
	printf("\n Nothing to delete!");
	return;	
}


void list(){
	////Use for loop to traverse array
	////Obtain group size of each list element, assign to variable, then print with other info
	ENTRY *p;
	listNum=0;
	int hasPrinted=0;
	while(listNum<4){
	if(lists[listNum].head == (ENTRY *)NULL&& listNum>=3&&hasPrinted==0){
		printf("empty");
		return;
	}
	p=lists[listNum].head;
	while(p !=(ENTRY *)NULL){
		//if(p->partySize != 0)
		printf("Name %s Size %d List %d\n", p->names, p->partySize, p->partyList);
		p=p->next;
		hasPrinted=1;
	}
	listNum++;
	}
}

void checkSize(){
	int x=0;
	listNum=-1;
	printf("Size to check for?\n");
	scanf("%d", &x);

	if(x<3)
		listNum=0;
	else if(x<5)
		listNum=1;
	else if(x<7)
		listNum=2;
	else 
		listNum=3;
	ENTRY *p;
	while(listNum>=0){
	p=lists[listNum].head;
	while(p != (ENTRY *)NULL){
		
		if(p->partySize<=x)
			printf("Name %s Size %d\n", p->names, p->partySize);
		p=p->next;		
	}
	listNum--;
	}
}

int checkDuplicate(char n[LENGTH]){
	ENTRY *p;
	listNum=0;
	while(listNum<4){
	p=lists[listNum].head;
	while(p!=(ENTRY *)NULL){
		//printf("%s", n);
		if(strcmp(n, p->names)==0)
			return 0;
		p=p->next;
 	}
	listNum++;
	}
	return 1;
}
