#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iosfwd>
#include <string>
#include <vector>
#include <utility>

namespace procd {
    namespace {
        class FilePath
        {
            public:
            typedef std::string StringType;
            typedef StringType::value_type CharType;
            
            FilePath(const char *str)
            {

            }
        };

        class ProcInf
        {
            public:
                void test()
                {
                    FilePath fp("test");
                    printf("test123\n");
                }
        };


    }
}


int main(void) {
    procd::ProcInf pi;
    pi.test();
}