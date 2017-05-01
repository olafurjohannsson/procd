
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/shared_region.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/sysctl.h>


namespace procd {
    
    namespace {
        
        bool GetCPUTypeForProcess(pid_t pid, cpu_type_t* cpu_type) {
            size_t len = sizeof(*cpu_type);
            int result = sysctlbyname("sysctl.proc_cputype",
                                      cpu_type,
                                      &len,
                                      NULL,
                                      0);
            if (result != 0) {
                return false;
            }
            return true;
        }
        
        
        
        
        
        int main(void) {
            
            return 0;
        }
        
    }
    
}
