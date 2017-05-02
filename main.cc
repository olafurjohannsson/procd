// C
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
#include <signal.h>
#include <ctype.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sysctl.h>
#include <stddef.h>
#include <stdint.h>

#ifdef macOS
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/shared_region.h>
#endif

#ifdef macOS
#endif

/*
    Process Daemon

    Daemon that monitors processes and their threads, network traffic, file system events and users/groups.

    Implementation is macOS and GNU/Linux.

    It forks 4 dedicated processes to monitor the listed events, it uses signals and shared memory for communication.
*/

unsigned verbose = 0;
const unsigned process_pool_count = 4;
const char *shm = "/dev/shm/";

#include "process/process.h"


void usage(char **argv) {
    printf("%s: \n\t-v (verbose)\n\t-p (process pool count)\n", argv[0]);
}

void signal_handler(int sig)
{
    printf("sig %d\n", sig);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    time_t t(0);


    // Use POSIX API, not System V
    // use shm or mmap file for ipc (can also use boost) but that means C++
    // use pcap for net sniffing
    // use inotify for file system events
    // 



    // 4 child processes
    // data collection process that reads from shared memory
    // networking data producer process (pcap)
    // file system data producer process (inotify)
    // process data producer process (procfs)
    // 1 main process that is a daemon and manages the 4 procs
    // signals can also be used for state (SIGUSR1, SIGUSR2)

    
    {
        int r;
        const char *memname = "sample";
        const size_t region_size = sysconf(_SC_PAGE_SIZE);
        printf("region_size %d, page size %d\n", region_size, _SC_PAGE_SIZE);

        int shm_fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
        if (shm_fd == -1)
            perror("shm_fd");

        r = ftruncate(shm_fd, region_size);
        if (r != 0)
            perror("ftruncate");
        
        {
            off_t offset = 0;
            void *p = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, offset);
            if (p == MAP_FAILED)
                perror("mmap");
            close (shm_fd);

            pid_t pid = fork();
            if (pid == 0) {
                u_long *d = (u_long*)p;
                *d = 0xdeadbeef;
                printf("parent exiting\n");
                exit(0);
            }
            else {
                int status;
                waitpid(pid, &status, 0);
                printf("child wrote %#lx\n", *(u_long*)p);
            }

            r = munmap(p, region_size);
            if (r != 0)
                perror("munmap");

            r = shm_unlink(memname);
            if (r != 0)
                perror("shm_unlink");

        }

    }

    return 0;

    int pagesize, fd;
    char *data;
    printf("opening\n");
    fd = open("/dev/shm/foo", O_RDONLY);
    printf("fd %s\n", fd);
    pagesize = getpagesize();
    printf("pagesize %d\n", pagesize);
    data = (char*)mmap((caddr_t)NULL, pagesize, PROT_READ, MAP_SHARED, fd, pagesize);


    return 0;


    if (argc > 1)
    {
        char c;
        while ( (c = getopt(argc, argv, "vp")) != -1 ) {
            printf("%c\n", c);
            continue;
            switch ( c ) {
                case 'v':
                    verbose = 1;
                    break;
                default:
                    usage(argv);
                    break;
            }
        }
    }
    else
        usage(argv);

    signal(SIGINT, signal_handler); // ctrl c
    
    return 0;
}
