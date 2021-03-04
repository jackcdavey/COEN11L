#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4
#define LENGTH 1024

int count=0;
int front=0;
int back=0;
char names[SIZE][LENGTH];
char symptoms[SIZE][LENGTH];

void insert();
void print();
void next();

int main(){
	while(1){
		char input='z';
		scanf("%c", &input);
	
		switch(input){
			case 'q':
				return 0;
			case 'a':
				insert();
				break;
			case 'l':
				print();
				break;
			case 'n':
				next();
				break;
			}
		}
}

void insert(){
	char n[LENGTH];
	char s[LENGTH];
	int compare=1;
	if(count<SIZE){
		printf("Please enter your name: ");
		scanf("%s", n);
		for(int i=front; i<=count; i++){
			if(i>=SIZE&& back<front){
				i=0;
			}
			if(strcmp(n, names[i])==0){
				printf("Sorry that name is taken \n");
				//insert();
				return;
			}
		}
	
	strcpy(names[back], n);
	printf("Please insert your symptom: ");
	scanf("%s", s);
	strcpy(names[back], n);
	strcpy(symptoms[back], s);
	back++;
	count++;
	return;
	}
	else{
		printf("Sorry the list is full\n");
	}
}

void print(){
	printf("Here's a list of names and symptoms: \n");
	for(int i=0; i<count; i++){
		if(front+i>=SIZE && back<front){
			for(int j=0; j<=back; j++){//second for loop is unecessary but ehhhhhhh
				printf("%s has: %s \n", names[j], symptoms[j]);
			}
			return;
		}
		printf("%s has: %s \n", names[front+i], symptoms[front+i]);
	}
	return;
}

void next(){
	if(count>0){
	printf("%s is next. Their symptom is: %s\n", names[front], symptoms[front]);
	count--;
	if(front<SIZE){	
		front++;
	}else{
		front=0;
	}
	return;
	}
	else{
		printf("Nobody is in the list\n");
		return;
	}
}

