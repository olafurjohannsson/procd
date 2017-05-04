
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
    printf("long max %ul\n", __LONG_MAX__);
    
    unsigned long c;
    c = (c << 8) | c;
    c = (c << 16) | c;
    c = (c << 32) | c;

    printf("%ul\n", c);

    return EXIT_SUCCESS;
}
