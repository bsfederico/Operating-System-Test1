#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    //Creating pipe
    int pipefd[2];
    int r;
    r = pipe(pipefd);
    //Handling possible error that pipe() would return
    if(r<0){
        perror("Error is got about pipe() function");
        exit(0);
    }
    //End of creating pipe

    //Creating fork()
    pid_t pid;
    pid = fork();
    //Handling possible error that pipe() would return
    if(pid<0){
        perror("Error is got about fork() function");
        exit(0);
    }
    // End of creating fork()

    //Handling processes that fork() function returned
    //Child
    char buffer[11];
    buffer[-1]=0;
    if(pid == 0){
        close(pipefd[0]);
        r = write(pipefd[1],"Hello world!",10);
        //If we get a error after executing of write() function
        if(r<0){
            perror("Error is got about write() function");
        }else if(r!=10){
            close(pipefd[1]);
            printf("The size sent is less than 10");
            //Si no se pudo escribir completo por qué tengo que bloquear la escribida?
            //Volver a llamar desplazando el puntero, yo creo que tocaría volver a llamar
            //write porque del write fue el que hubo error, hasta que se escriba todo
        }
    //Father    
    }else{
        close(pipefd[1]);
        r = read(pipefd[0],buffer,10);
        if(r<0){
            perror("Error is got about read() function");
        }else if(r!=10){
            close(pipefd[0]);
            printf("The size read is less than 10");
            //Volver a llamar desplazando el puntero, yo creo que tocaría volver a llamar
            //write porque del write fue el que hubo error, hasta que se escriba todo
        }
        printf("The value of buffer is: %s",buffer);
        close(pipefd[0]);
    }
    //End of handling processes that fork function returned

    return 0;
}