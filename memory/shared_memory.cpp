

#include "shared_memory.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


root::shared_memory::shared_memory()
    : raw_memory(NULL),
      bytes(0) {}

root::shared_memory::~shared_memory()
{
}

bool root::shared_memory::create(const std::string &name, uint32_t bytes)
{
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

    return false;
}

