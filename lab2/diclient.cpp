#include "local.h"

using namespace std;
int main(int argc, char *args[])
{
    int readfd = open("test",O_RDWR, 0);
    int i;
    long sec, nansec;
    char buf[50][1024];
    struct timespec start = {0,0};	
	struct timespec end = {0,0};
    clock_gettime(CLOCK_MONOTONIC_RAW,&start);
    for (i=0;i<50;i++)
    {
        read(readfd,buf[i],64);
        write(readfd,buf[i],64);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW,&end);	
    sec = end.tv_sec - start.tv_sec;
    nansec = end.tv_nsec - start.tv_nsec;
    printf("time is %d seconds and %ld nanoseconds\n", sec, nansec);
    close(readfd);
}