#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>

uint8_t d = 0;//создаём двоичное число
uint8_t Power = 1;//степень двойки
int i = 0;
int Stopper = 1;//флаг того, что файл передан
pid_t pid;

void One(int nsig)//учитывает единицу в двоичной записи
{
    d += Power;
    i++;
    Power *= 2;
    kill(pid, SIGUSR1);
}

void Zero(int nsig)//учитывает ноль в двоичной записи
{
    i++;
    Power *= 2;
    kill(pid, SIGUSR1);
}

void stop(int nsig)//останавливает передачу файла
{
    Stopper = 0;
}

int main()
{
    printf("Receiver pid: %d\n", getpid());//Выдаёт номер получателя
    printf("What sender?\n");//запрашивает номер отправителя
    scanf("%d", &pid);
    signal(SIGUSR1, One);//присваем сигналам функции
    signal(SIGUSR2, Zero);
    signal(SIGINT, stop);
    int fd;
    fd = open("new", O_CREAT|O_WRONLY, 0666);//создаём файл, куда будем записывать файл
    while(Stopper)//пока происходит передача
    {
        while(i < 8)
            if(Stopper == 0)
                break;
        if(Stopper == 0)
                break;
        i = 0;
        Power = 1;
        printf("d = %d\n",d);
        write(fd, &d, 1);
        d = 0;
        kill(pid, SIGUSR2);
    }
    close(fd);
    return 0;
}
