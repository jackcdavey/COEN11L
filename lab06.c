#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10

void CreateNode();
void printForward();
void printBack();
void back();
void forward();
void printCurrentAddress();


typedef struct Node{
    char url[50];
    struct Node* next;
    struct Node* prev;
}Node;


int main(){
    Node *head=NULL;
    Node *tail=NULL;
    Node *current=NULL;
    
    //Test script
    CreateNode(&head, &current, &tail, "Zeroth.com");
    CreateNode(&head, &current, &tail, "First.com");
    CreateNode(&head, &current, &tail, "Second.com");
    printCurrentAddress(current); //Current url shoudld be ^
    CreateNode(&head, &current, &tail, "Third.com");
    CreateNode(&head, &current, &tail, "Fourth.com");
    CreateNode(&head, &current, &tail, "Fifth.com");
    CreateNode(&head, &current, &tail, "Sixth.com");
    CreateNode(&head, &current, &tail, "Seventh.com");
    CreateNode(&head, &current, &tail, "Eighth.com");
    CreateNode(&head, &current, &tail, "Nineth.com");
    CreateNode(&head, &current, &tail, "Tenth.com");
    CreateNode(&head, &current, &tail, "Eleventh.com"); //<--- Should not be added, limit is 10
    printForward(head, current);
    printBack(tail, current);
    back(&current);
    back(&current);
    printForward(head, current);
    printBack(tail, current);
    printCurrentAddress(current);// Should be eighth.com
    CreateNode(&head,&current, &tail, "Yahoo.com");
    CreateNode(&head, &current, &tail, "TenthButVisitedAfterYahoo.com");
    printForward(head, current);
    //printf("head is %s", head->url);
    //printf("Tail is %s", tail->url);
}

void printCurrentAddress(Node *current){
    
    printf("The current URL is: %s\n", current->url);
}

void CreateNode(Node** head, Node** current, Node** tail, const char *address){
    int nodeCount = 0;
    Node *newNode, *c, *p, *count;
    newNode = (Node*)malloc(sizeof(Node));
    p=*head;
    count=*head;
    c=*current;

    while(count){//Count elements in the list
        nodeCount++;
        count=count->next;
    }

    if(nodeCount>N && c==*tail){//Check to see if the list is full
        printf("List is full\n");
        return;
    }

    if(!newNode) //Break if memory is not available
        return;

    strcpy(newNode->url, address);
    newNode->prev = *current;
    newNode->next = NULL;
    if(p==NULL){
        *head=newNode;
        //printf("adding head %s", newNode->url);
        
    }
    if(c){
        c->next = newNode;
        //newNode->prev = c;
    }
    *current = newNode;
    *tail = newNode;
    //printf("num elements is %d", nodeCount);
}



void back(Node** current){
    Node *s;
    s = *current;
    if(s->prev)
        *current=s->prev;
}

void forward(Node** current){
    Node *s;
    s = *current;
    if(s->next)
        *current=s->next;
}

void printForward(Node *head, Node *current){
    Node *p = head;
    if(!p){
        printf("History is empty \n");
        return;
    }
    printf("Oldest --> Newest\n");
    printf("-----------------\n");
    while(p){
        if(p==current)
            printf("%s *\n", p->url);
        else
            printf("%s \n", p->url);
        p=p->next;
    }
    printf("\n");
}

void printBack(Node *tail, Node *current){
    Node *p = tail;
    if(!p){
        printf("History is empty \n");
        return;
    }
    printf("Newest --> Oldest\n");
    printf("-----------------\n");
    while(p){
        if(p==current)
            printf("%s *\n", p->url);
        else
            printf("%s \n", p->url);
        p=p->prev;
    }
    printf("\n");
        
}
