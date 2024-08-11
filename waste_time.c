#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// runs for 10 minutes but does nothing
void waste_time()
{
    time_t start = time(NULL);
    while (time(NULL) - start < 600)
    {
    }
}

int main(int argc, char *argv[])
{
    waste_time();

    return 0;
}