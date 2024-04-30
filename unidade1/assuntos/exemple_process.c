#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void){
    
    printf("(fora) PID do processo pai = %d\n", getpid());

    int pid = fork();

    if(pid < 0){
        printf("erro ao criar o processo filho!");
    }else if(pid == 0){
        printf("(dentro) PID do processo pai = %d\n", getppid());
        printf("processo filho criado com sucesso!\n");
        printf("PID do processo filho = %d", getpid());
    }
    
    return 0;
}