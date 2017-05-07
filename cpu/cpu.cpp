
namespace root
{

class cpu
{
    static inline void native_cpuid(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
    {
        asm volatile("cpuid"
                : "=a" (*eax),
                "=b" (*ebx),
                "=c" (*ecx),
                "=d" (*edx)
                : "" (*eax), "2" (*ecx)
                : "memory");
    }
};

}