#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_TASKS 2  /* the maximal number of taskes in the system */

/* task control block -
 * holding all task relevant informations
*/
struct pcb{
    int pid;                   /* ID of the process */
    int period;                  /* task period */
    int attached;              /* 1 if attached to tasklist, else 0 */
    int *function;             /* pointer to the task function */
};

static struct pcb tasklist[MAX_TASKS];
int task0();
int task1();

int task_attach(int pid, int period, void *function)
{
    int ret = -1;
    if(tasklist[pid].attached != 1)
    {
        printf("attach task pid %d\n", pid);
        tasklist[pid].period = period;
        tasklist[pid].function = function;
        tasklist[pid].attached = 1;
        ret = 0;
    }
    return ret;
}

int task_detach(int pid)
{
    tasklist[pid].attached = 0;
    printf("detach task pid %d\n", pid);
    return 0;
}

/*
 * implementation of a scheduler
 */
int scheduler()
{
    int i = 0;
    int j = 10;
    int elapsedTime = 10; // timer tick
    void (*p)(void);

    while(j!='c')
    {
        printf("elapsedTime= %d\n",elapsedTime);
        for(i = 0; i < MAX_TASKS; i++)
        {
            if(tasklist[i].attached == 1){
                if((elapsedTime >= tasklist[i].period)&&(elapsedTime%(tasklist[i].period) == 0))
                {
                    p = (void *)tasklist[i].function;
                    (*p)();
                }
            }
        }
        elapsedTime+=10;
        j--;
    }

    return 0;
}

int FindFile(int argc, char *argv[])
{
    DIR *dp;
    struct dirent *dirp;
    int ret = 0;

    if (argc != 3)
    {
        printf("usage: ./Exe_Name dir_name file_name");
        ret = 0;
    }

    if ((dp = opendir(argv[1])) == NULL)
    {
        printf("can't open %s", argv[1]);
        ret = -1;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if(!strcmp(dirp->d_name,argv[2])){
            printf("File %s found\n", dirp->d_name);
            ret = 0;
            break;
        }else{
            ret = 1;
        }
    }

    return ret;
    closedir(dp);
}

int task0()
{
    printf("[dbg msg] Execute task0\n"); /* message printed on the board */
    return 0;
}

int task1()
{
    int ret = 0;
    char *argv[] = {".\\FindFile","H:\\personale\\esercizi_c\\TestScheduler\\","EXAMPLE.txt", (char *) NULL };

    ret = FindFile(3, &argv[0]);
    if(ret!=0){
        printf("Execute task1: file not found\n");
    }

    return 0;
}

int main()
{
    /*
     * test run here
     * */
    printf("scheduler \n");
    task_attach(0, 10, task0);
    task_attach(1, 30,  task1);
    scheduler();
    task_detach(0);
    task_detach(1);
    return 0;
}
