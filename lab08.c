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
void delHist();
void saveList();
void loadList();


typedef struct Node{
    char url[50];
    struct Node* next;
    struct Node* prev;
}Node;

Node histIter();

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
    back(&current);
    back(&current);
    back(&current);
    back(&current);
    back(&current);
    delHist(current, &tail);
    printBack(tail, current);
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

void delHist(Node *current, Node** tail){//Recursively delete & free nodes after current
    Node *temp=current->next;
    if(temp->next)
        delHist(temp->next, &*tail);
    temp->next=NULL;
    temp=NULL;
    free(temp);
    *tail=current;
}

void saveList(Node *head, Node *current, FILE *outfp){
    Node *s = head;
    if ((outfp= fopen("data.txt", "wb")) == NULL)
        printf("cannot open the file data.txt\n");
    while(s!=NULL){
        int i=0;
        if(s==current)
            fwrite(&i, sizeof(int), 1, outfp);//First element written is the position of the current url
        s=s->next;
        i++;
    }
    *s=*head;
    while(s!=NULL){
        fwrite(&s, sizeof(Node), 1, outfp);
    }
}

void loadList(FILE *infp){
    Node *head=NULL;
    Node *tail=NULL;
    Node *current=NULL;
    Node *s;
    int currURLPos;
    fread(&currURLPos, sizeof(int), 1, infp);
    int nodeNum=0;
    while(fread(&s, sizeof(s), 1, infp)){
        histIter(infp, s, nodeNum, currURLPos);
        nodeNum++;
    }
}

Node histIter(FILE *infp, Node *s, int isCurr, int currURLPos){
    Node *currNode=s;
    if(isCurr==currURLPos){
        //do something idek what he wants bc you cant return multiple types??
    }
        return *s;
}

