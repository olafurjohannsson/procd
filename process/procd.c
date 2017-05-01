#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
//#include <pthread.h>
//#include <err.h>
#include <time.h>


#define DEBUG 1

struct procent {
   pid_t pid;
   char name[128];
};

struct procent *create_procent(pid_t pid, const char *name)
{
    struct procent *pent;
    pent = (struct procent*)malloc(sizeof(struct procent));
    strcpy(pent->name, name);
    pent->pid = pid;
    return pent;
}

void get_date(char *date)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(date, 128, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

struct proc_info {
    
    unsigned id;
    const char *name;
    time_t timestamp;
    
};



void read_dir()
{
    DIR *dir;
    struct procent **pp;
    struct dirent *e;

    e = (struct dirent*)malloc(sizeof(struct dirent));
    

    dir = opendir("/proc");
    int i = 0;
    while ((e = readdir(dir)) != NULL)
    {
        struct procent *p = create_procent((pid_t)e->d_type, e->d_name);
        pp[i] = p;
    }

    closedir(dir);
    
    
}
// lsof
// sysctl -a
// ps ax
struct {
    pid_t pid;
} fork_t;


void start_daemon()
{
    // Process ID and Session ID
    pid_t pid, sid;
    
    
    
#ifdef DEBUG
    char buffer[128];
    get_date(buffer);
    printf("procdaemon started [%s] \n", buffer);
#endif
    
    
    
    if ((pid = fork()) == 0)
    {
        printf("pid if %d\n", pid);
    }
    else if (pid > 0) // exit parent process
        exit(EXIT_SUCCESS);
    else // error
        exit(EXIT_FAILURE);
    
    // change file mode mask
    umask(0);
    
    // create SID for child process
    sid = setsid();
    if (sid == -1) {
        printf("sid fail");
        exit(EXIT_FAILURE);
    }
    
    // change cwd
    if ((chdir("/Users/olafurj")) < 0) {
        printf("chdir fail\n");
        exit(EXIT_FAILURE);
    }
    
#ifndef DEBUG
    // close std fds
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
#endif
    printf("before while\n");
    // daemon init
    
    read_dir();
    
    while (1) {
        printf("1\n");
        
        exit(EXIT_SUCCESS);
    }
}
/*
int main(void)
{

}
 */
