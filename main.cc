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
#include <sys/file.h>
#include <sys/mman.h>
#include <limits.h>


#ifdef macOS

#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/shared_region.h>
#endif



#include "ug.c"

/*
   Process Daemon

   Daemon that monitors processes and their threads, network traffic, file system events and users/groups.

   Implementation is macOS and GNU/Linux.

   It forks 4 dedicated processes to monitor the listed events, it uses signals and shared memory for communication.
   */

unsigned verbose = 0;
const unsigned process_pool_count = 4;
const char *shm = "/dev/shm/";



void usage(char **argv) {
    printf("%s: \n\t-v (verbose)\n\t-p (process pool count)\n", argv[0]);
}

void signal_handler(int sig)
{
    printf("sig %d\n", sig);
}

struct monitor_t {
    uint32_t id;

};

// the 4 processes, can use clone() or fork()
void init_process_monitor(void);
void init_network_monitor(void);
void init_filesys_monitor(void);
void init_usrgrps_monitor(void);

union conv32 {
    uint32_t u32;
    float    f32;
};

float conv(uint32_t t)
{
    return ((union conv32){.u32 = t}).f32;
}

static int show_cpuinfo(struct seq_file *m, void *v)
{
    struct cpuinfo_x86 *c;
    unsigned cpu;
    int i;
    return 0;
}
static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
        unsigned int *ecx, unsigned int *edx)
{
    asm volatile("cpuid"
            : "=a" (*eax),
            "=b" (*ebx),
            "=c" (*ecx),
            "=d" (*edx)
            : "" (*eax), "2" (*ecx)
            : "memory");
}
// see
// http://lxr.free-electrons.com/source/arch/x86/kernel/cpu/proc.c#L85
float get_cpu_clock_speed()
{
    FILE* fp;
    char buffer[1024];
    size_t bytes_read;
    char *match;
    float clock_speed;

    fp = fopen("/proc/cpuinfo", "r");
    bytes_read = fread(buffer, 1, sizeof buffer, fp);
    fclose(fp);
    if (bytes_read == 0)
    {
        return 0;
    }
    buffer[bytes_read] = '\0';
    match = strstr(buffer, "cpu MHz");
    if (match == NULL)
        return 0;
    sscanf(match, "cpu MHz         : %f", &clock_speed);
    return clock_speed;
}

int main(int argc, char **argv)
{

    // log time of app start in logfile /var/procd/procd.log
    // parse input, such as (push_interval, verbosity)
    // create struct procd_t that is the daemon structure
    // create struct procd_net_t (network process listener)
    // create struct procd_proc_t (process info listener)
    // create struct procd_fs_t (filesystem listener)
    // keep the three struct in an array
    // use shared memory or memory mapped file for communications
    // add signals for state
    // start by create process info listener
    pid_t pid;    

    switch ((pid = fork())) {
        case 0:
            printf("parent\n");
            exit(0);
            break;
        default:
            printf("child %d\n", pid);
            break;
    }
    printf("%f\n", get_cpu_clock_speed());

    sleep(1);
    exit(0);


    /*


       unsigned int a,b,c,d;
       native_cpuid(&a, &b, &c, &d);
       if (a >= 16)
       native_cpuid(&a, &b, &c, &d);
       printf("%d MHz, %d, %d\n", a, b, c);


       char *ccc = (char*)(intptr_t)a;
       printf("ccc %p\n", ccc);
       uint32_t t = 123;
       float f = conv(t);

       printf("int %d, fl %.6f\n", t, f);
       f = get_cpu_clock_speed();
       printf("CPU clock speed: %4.0f MHz\n", f);
       return 0;
       */
    /*

       return 0;
       uid_t uid = getuid();
       printf("uid %d\n", uid);
       uid_t uid2 = userid_from_name("olafurj");
       printf("uid %d\n", uid2);
       const char *name = username_from_userid(uid2);
       printf("name %s\n", name);

       int lim = pathconf("/", _PC_NAME_MAX);
       printf("limit %d\n", lim);
       long page_size = sysconf(_SC_PAGE_SIZE);
       printf("pagesize %d\n", page_size);
       return 0;
       */
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
