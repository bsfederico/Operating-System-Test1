#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    FILE *fbridge;
    fbridge = fopen("bridge.bin","w+b");
    if(fbridge==NULL){
        printf("There is a error about openning bridge.b");
        exit(0);
    }

    int weigth = 1000;
    char *row = malloc(weigth*sizeof(char));

    for(int i=0;i<weigth;i++){
        *(row+i) = 'Z';
    }

    fwrite(row,weigth*sizeof(char),1,fbridge);
    
    printf("Terminó la creación!\n");

    fclose(fbridge);
    system("gcc cfile.c -o cfile");
    char *args[]={"./cfile",NULL};
    execv(args[0],args);
    
    return 0;
}