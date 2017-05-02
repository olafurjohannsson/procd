#include "procd.c"
#include <string>
#include <vector>

namespace procd
{



class ProcessException : public std::exception
{
};

class Process
{
  public:
    std::vector<std::string> log;
    pid_t create()
    {
        pid = fork();
        //if (pid < 0)
        //log.push_back("Pid: " + std::to_string(pid))
        return this->pid;
    }

    pid_t get_pid()
    {
        return this->pid;
    }

    bool is_child()
    {
        return this->pid > 0;
    }

    bool is_parent()
    {
        return this->pid == 0;
    }

    void self()
    {
        DIR *dir;
        struct procent **pp;
        struct dirent *e;

        e = (struct dirent *)malloc(sizeof(struct dirent));

        dir = opendir("/proc");
        int i = 0;
        while ((e = readdir(dir)) != NULL)
        {
            struct procent *p = create_procent((pid_t)e->d_type, e->d_name);
            pp[i] = p;
        }

        closedir(dir);
    }

#ifdef macOS
    bool GetCPUTypeForProcess(pid_t pid, cpu_type_t *cpu_type)
    {
        size_t len = sizeof(*cpu_type);
        int result = sysctlbyname("sysctl.proc_cputype",
                                  cpu_type,
                                  &len,
                                  NULL,
                                  0);
        if (result != 0)
        {
            std::cout << "sysctlbyname("
                         "sysctl.proc_cputype"
                         ")\n";
            return false;
        }
        return true;
    }
#endif

  private:
    pid_t pid;
};

class ProcessManager
{
};

} // end procd namespace
