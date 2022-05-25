#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



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


    //Creating fork()
    pid_t pid;
    pid = fork();
    //Handling possible error that pipe() would return
    if(pid<0){
        perror("Error is got about fork() function");
        exit(0);
    }

    if(pid == 0){
        fwrite(row,weigth*sizeof(char),1,fbridge);
        printf("Terminó la creación!\n");
    }else{
        wait(NULL);
        fseek(fbridge,0,SEEK_SET);
        fread(row,sizeof(char),weigth,fbridge);
        printf("Terminó la lectura!\n");
    }

    fclose(fbridge);
        
    return 0;
}