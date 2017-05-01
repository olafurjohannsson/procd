


#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>

#include <fstream>
#include <limits>
#include <iostream>
#include <vector>
#include <numeric>
#include <list>
#include <string>

FILE* OpenFile(const std::string &filename, const char* mode)
{
    FILE *result = NULL;
    do {
        result = fopen(filename.c_str(), mode);
    } while (!result && errno == EINTR);
    return result;
}

bool CloseFile(FILE* file)
{
    return file == NULL || fclose(file) == 0;
}

bool ReadFileToString(const std::string &path, std::string *contents, size_t max_size)
{
    if (contents)
        contents->clear();

    FILE *file = OpenFile(path, "rb");
    if (!file)
        return false;

    char buffer[1<<16];
    size_t len;
    size_t size = 0;
    bool read_status = true;

    while ((len = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (contents)
            contents->append(buffer, std::min(len, max_size - size));
        if ((max_size - size) < len) {
            read_status = false;
            break;
        }

        size += len;
    }

    read_status = read_status && !ferror(file);
    CloseFile(file);

    return read_status;
}

namespace procd
{

    class Process
    {
        
        public:
    
            Process(); 

            int GetCPU(pid_t pid);
            float GetMemory(pid_t pid);
            
            void test();

        private:
            const char *procPath = "/proc";
    };

};
