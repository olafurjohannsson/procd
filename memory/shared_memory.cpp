

#include "shared_memory.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


root::shared_memory::shared_memory()
    : raw_memory(NULL),
      bytes(0) {
#ifdef macOS
    mach_port_t named_right;
    mach_vm_size_t size = 1024;
    // it's not 64 bit only, even though the name suggests it
    kern_return_t kr = mach_make_memory_entry_64(
        mach_task_self(),
        &size,
        0, // address,
        MAP_MEM_NAMED_CREATE | VM_PROT_READ | VM_PROT_WRITE,
        &named_right,
        MACH_PORT_NULL); // parent
    this->memory_object = kr != KERN_SUCCESS ? MACH_PORT_NULL : named_right;
#endif

      }

root::shared_memory::~shared_memory()
{
}

bool root::shared_memory::create(const std::string &name, uint32_t bytes)
{
    #ifdef macOS
        kern_return_t kr = mach_vm_map(
            mach_task_self(),
            reinterpret_cast<mach_vm_address_t*>(this->raw_memory),
            bytes,
            0,
            VM_FLAGS_ANYWHERE,
            memory_object,
            0,
            0,
            VM_PROT_READ | VM_PROT_WRITE,
            VM_PROT_WRITE | VM_PROT_READ | VM_PROT_IS_MASK,
            VM_INHERIT_NONE);
        return kr == KERN_SUCCESS;
    #elif
    uint32_t fd = open("test", O_CREAT | O_RDWR, 0777);
    void *memory = mmap(NULL,
                        bytes,
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        fd,
                        0);
    if (memory != (void *)-1 && memory != NULL)
    {
        this->raw_memory = memory;
        return true;
    }
    else if (memory == MAP_FAILED)
    {
        handle_error("mmap");
    }

    #endif
    return false;
}

