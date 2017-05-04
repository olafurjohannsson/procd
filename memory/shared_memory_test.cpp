


#include "shared_memory.h"


int main() {

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