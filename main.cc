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
#include <time.h>

// C++
#include <iostream>
#include <string>
#include <numeric>
#include <future>
#include <thread>
#include <exception>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/shared_region.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/sysctl.h>


/*
    Process Daemon

    Daemon that monitors processes and their threads, network traffic, file system events and users/groups.

    Implementation is macOS and GNU/Linux.

    It forks 4 dedicated processes to monitor the listed events, it uses signals and shared memory for communication.
*/

const unsigned verbose = 0;
const unsigned process_pool_count = 4;
const char *shm = "/dev/shm/";

#include "process/process.h"


int main(void)
{
    time_t t(0);
    std::cout << "started \n";
    
    //if (argc > 1)



    procd::Process p;
    pid_t pid = p.create();
    cpu_type_t cp;
    
    if (p.is_child()) {
        std::cout << "i am child, my pid is " << (pid == p.get_pid() ? "same" : "notsame") << ", " << p.get_pid() << std::endl;
        std::cout << "cpy type " << p.GetCPUTypeForProcess(pid, &cp);
    }
    else if (p.is_parent())
    {
        std::cout << "i am parent, my pid is " << (pid == p.get_pid() ? "same" : "notsame") << ", " << p.get_pid() << std::endl;
        std::cout << "cpy type " << p.GetCPUTypeForProcess(pid, &cp);
    }
    
    return 0;
}
