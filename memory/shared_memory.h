#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>


#ifdef __APPLE__
#define macOS
#endif



#ifdef macOS
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/mach_traps.h>
#include <mach/mach_init.h>
#endif

#define print(msg) printf(msg); printf("\n");

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

namespace root
{

class shared_memory
{
  public:
    shared_memory();

    ~shared_memory();

    bool create(const std::string &name, uint32_t bytes);

    bool map(size_t bytes);

    bool unmap();

    void *memory() const { return this->raw_memory; }

  private:
    void *raw_memory;
    uint32_t bytes;

    // it is not allowed to call a copy constructor or the equal(=) function
    // this is a C++11 feature, so compile with minimum -std=c++11
    shared_memory(const shared_memory&) = delete;
    void operator=(const shared_memory&) = delete;

    #ifdef macOS
    mach_port_t memory_object;
    #endif
};
}

#endif