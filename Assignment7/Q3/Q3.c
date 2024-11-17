#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    char *arr [] = {"circle.c", "rectangle.c", "square", "main.c", NULL};
    int ret;
    for(int i = 0; i<4; i++)
    {
     ret = fork();
     if(ret == 0)
     {
        int ret_exec = execlp("gcc", "gcc", "-c", arr[i], NULL);
        if (ret_exec < 0)
        {
            perror("Exec command failed(): ");
            _exit(7);
        }
        _exit(0);
     } 
    } int n;
    while (waitpid(-1, &n, 0) > 0)
    {
        printf("Exit status: %d\n", WEXITSTATUS(n));
    }
    char *argc[] = {"gcc", "-o", "program.out", "circle.o", "rectangle.o", "square.o", "main.o", NULL};
    ret = fork ();
    if (ret == 0)
    {
        int ret_exec = execvp(argc[0], argc);

        if(ret_exec < 0)
        {
            perror("Exec command failed(): ");
            _exit(8);
        }
        _exit(0)
    }    
    waitpid(-1, &n, 0);
    printf("Exit status of .out file: %d\n", WEXITSTATUS(n));

    //running program.out file
    char buffer[512];
    char *c_d = getcwd(buffer, sizeof(buffer));
    ret = fork();

    if(ret == 0)
    {
        int ret_exec = execl("./program.out", "program.out", NULL);
        if(ret_exec < 0)
        {
            perror("Exec command failed(): ");
            _exit(10);
        }
        _exit(0);
    }
    waitpid(-1, &n, 0);
    printf("Exit status of ./program.out file: %d\n", WEXITSTATUS(n));
    return 0;
}