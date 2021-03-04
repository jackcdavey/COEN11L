#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 50
#define LENGTH 1024

int count=0;
int front=0;
int back=0;
int peopleRejected=0;

char names[SIZE][LENGTH];
char symptoms[10][LENGTH]={"cough", "fever", "congestion", "headache", "blind", "dead", "seizure", "mute", "deaf", "tired"};
char genders[2][LENGTH]={"male", "female"};
int dataCodes[SIZE][2];
double symptomWait[10][2];
double genderWait[2][2];

void insert();
void print();
void next();

int main(){
	srand(time(NULL));
	for(int i=0; i<10000; i++){
		int r=rand()%100;
		if(r>50){
			insert();
		}
		else
			next();
	}


	/*
	printf("Press a to insert, l to print, n for next, and q for quit\n");
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
	*/
	//printf("number of males served: %f with total wait time of %f", genderWait[0][1], genderWait[0][0]);
	for(int i=0; i<10; i++){
		printf("%d cases of %s treated with an average wait time of %f\n", (int)(symptomWait[i][1]), symptoms[i], symptomWait[i][0]/symptomWait[i][1]);
	}
	for(int i=0; i<2; i++){
		printf("%d %s served with an average wait time of %f\n", (int)(genderWait[i][1]), genders[i], genderWait[i][0]/genderWait[i][1]); 
	}
	printf("%d people were rejected", peopleRejected);
}

void insert(){
	if(count<SIZE){
	int ranGen= rand()%2;
	int ranSym = rand()%10;
	dataCodes[back][0]=ranGen;
	dataCodes[back][1]=ranSym;
	//printf("%s %s\n", genders[dataCodes[back][0]], symptoms[dataCodes[back][1]]);
	if(back<SIZE-1)
	back++;
	else 
	back=0;
	count++;
	return;
	}else
		peopleRejected++;
	return;
	/*
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
	*/
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
	double d=10.0+rand()%291;
	d=d/10;
	genderWait[dataCodes[front][0]][0]+=d;
	genderWait[dataCodes[front][0]][1]++;
	symptomWait[dataCodes[front][1]][0]+=d;	
	symptomWait[dataCodes[front][1]][1]++;
	if(count>0){
	//	printf("A %s is next. Their symptom is: %s\n", genders[dataCodes[front][0]], symptoms[dataCodes[front][1]]);
	//	printf("List size is now %d\n", count);
		if(rand()%101>85){
			if(back<SIZE-1)
				back++;
			else
				back=0;

			dataCodes[back][0]=dataCodes[front][0];
			dataCodes[back][1]=dataCodes[front][1];		




			if(front<SIZE-1)
				front++;
			else
				front=0;
			return;
		}
		count--;
		if(front<SIZE-1){	
			front++;
		}else{
			front=0;
		}
		return;
	}
	else{
	//	printf("Nobody is in the list\n");
		return;
	}
	
}

