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

// C++
#include <iostream>
#include <string>
#include <numeric>
#include <future>
#include <thread>

#include <exception>
#ifdef macOS
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/shared_region.h>
#endif

#include <stddef.h>
#include <stdint.h>
#ifdef macOS
#include <sys/sysctl.h>
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



int main(int argc, char **argv)
{
    time_t t(0);

    
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
