

#include "shared_memory.h"

void *start_callback(void)
{
    void *p;

    return p;
}
void *stop_callback(void)
{
    void *p;

    return p;
}
struct meta
{
    enum
    {
        running,
        stopped,
        error,
        cancelled
    } state = meta::running;
    union {
        (fn_start *) = start_callback;
    } callback;
} metadata;
int main()
{

    if (metadata.state == meta::running)
    {
        print("started");
    }

    return 0;
    root::shared_memory shm;

    if (shm.create("", 1024))
    {
        print("works");
    }
    else
    {
        print("did not work");
    }

    return 0;
}