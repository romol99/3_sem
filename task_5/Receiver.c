#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <fcntl.h>

int Wait = 0;//флаг ожидания бита
int SymWait = 0;//фдаг ожидания записи байта

void StopWait(int nsig)
{
   Wait = 0;
}

void SymGet(int nsig)
{
   SymWait=0;
}

int main(void)
{
    pid_t pid;
    uint8_t d, curr;
    signal(SIGUSR1, StopWait);//присваиваем сигналам функции
    signal(SIGUSR2, SymGet);
    int fd, sum;
    fd = open("old.txt", O_RDONLY);//открываем чтения файла
    printf("Sender pid: %d\n", getpid());
    printf("Reciever pid?\n");
    scanf("%d", &pid);
    while(1)
    {
        sum = read(fd, &d, 1);
        if(sum == 0)
        {
            kill(pid, SIGINT);
            break;
        }
        printf("Sender: %d\n", d);
        for(int i = 0; i < 8; i++)
        {
            curr = d % 2;
            if(curr == 1)
                kill(pid, SIGUSR1);
            else
                kill(pid, SIGUSR2);
            d = d / 2;
            Wait = 1;
            while(Wait);
        }
        SymWait = 1;
        while(SymWait);
    }
    close(fd);
    return 0;
}
