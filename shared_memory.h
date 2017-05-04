#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>


// implementation file
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace root {
    class shared_memory {
        public:
        shared_memory();

        ~shared_memory();

        bool create(const std::string &name, uint32_t bytes);

        private:
        void *memory;
        uint32_t bytes;
    };
}

root::shared_memory::shared_memory()
    : memory(NULL),
      bytes(0)
      {

      }

root::shared_memory::~shared_memory()
{
    
}

bool root::shared_memory::create(const std::string &name, uint32_t bytes)
{
    uint32_t fd = open("test", O_RDONLY);
    void *memory = mmap(NULL, 
                        bytes, 
                        PROT_READ | PROT_WRITE, 
                        MAP_SHARED, 
                        fd, 
                        0);
    if (memory != (void*)-1 && memory != NULL)
    {
        this->memory = memory;
        return true;
    }
    
    return false;
}


int main(int argc, char **argv)
{
    root::shared_memory shm;

    if (shm.create("", 1024))
    {
        printf("success\n");
    }
    else {
        printf("fail\n");
    }
}


#endif