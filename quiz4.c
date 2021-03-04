#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp=fopen("fileName.txt", r);
if(fp==NULL)
    printf("file could not be opened");

int val=0;
fscanf(fp, "%d", &val);

if(
