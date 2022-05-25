#include <stdio.h>
#include <stdlib.h>


int main(){
    FILE *fbridge;
    fbridge = fopen("bridge.bin","r+b");
    if(fbridge==NULL){
        printf("There is a error about openning bridge.b");
        exit(0);
    }

    int weigth = 100000000;
    char *row = malloc(weigth*sizeof(char));

    fread(row,sizeof(char),weigth,fbridge);
    
    printf("Terminó la lectura!\n");

    fclose(fbridge);

    return 0;
}