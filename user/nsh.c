#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
 
void execPipe(char*argv[],int argc);
int fork1(void);
void panic(char*);
 
#define MAXARGS 10
#define MAXWORD 30
#define MAXLINE 100
 
int getcmd(char *buf, int nbuf)
{
    fprintf(2, "@ ");
    memset(buf, 0, nbuf);
    gets(buf, nbuf);
    if (buf[0] == 0)
        return -1;
    return 0;
}
char whitespace[] = " \t\r\n\v";
char args[MAXARGS][MAXWORD];
 
void parsebuf(char *cmd, char* argv[],int* argc)
{
    for(int i=0;i<MAXARGS;i++){
        argv[i]=&args[i][0];
    }
    int i = 0; 
    int j = 0;
    for (; cmd[j] != '\n' && cmd[j] != '\0'; j++)
    {
        while (strchr(whitespace,cmd[j])){
            j++;
        }
        argv[i++]=cmd+j;
        while (strchr(whitespace,cmd[j])==0){
            j++;
        }
        cmd[j]='\0';
    }
    argv[i]=0;
    *argc=i;
}
 
void runcmd(char*argv[],int argc)
{
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],"|")){
            execPipe(argv,argc);
        }
    }
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],">")){
            close(1);
            open(argv[i+1],O_CREATE|O_WRONLY);
            argv[i]=0;
        }
        if(!strcmp(argv[i],"<")){
            close(0);
            open(argv[i+1],O_RDONLY);
            argv[i]=0;
        }
    }
    exec(argv[0], argv);
}
 
void execPipe(char*argv[],int argc){
    int i=0;
    for(;i<argc;i++){
        if(!strcmp(argv[i],"|")){
            argv[i]=0;
            break;
        }
    }
    int fd[2];
    pipe(fd);
    if(fork()==0){
        close(1);
        dup(fd[1]);
        close(fd[0]);
        close(fd[1]);
        runcmd(argv,i);
    }else{
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        runcmd(argv+i+1,argc-i-1);
    }
}
int main()
{
    char buf[MAXLINE];
    while (getcmd(buf, sizeof(buf)) >= 0)
    {
        if (fork1() == 0)
        {
            char* argv[MAXARGS];
            int argc=-1;
            parsebuf(buf, argv,&argc);
            runcmd(argv,argc);
        }
        wait(0);
    }
    exit(0);
}
void
panic(char* s)
{
  fprintf(2, "%s\n", s);
  exit(-1);
}
int
fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}