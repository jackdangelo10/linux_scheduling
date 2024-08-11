#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/kernel.h>

#define __NR_sched_set_time_boost 451

int main(int argc, char *argv[])
{
    // take in a PID and print its scheduling policy, priority, and time slice
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pid = atoi(argv[1]);
    // check for valid pid
    if (pid < 0)
    {
        fprintf(stderr, "Invalid PID\n");
        exit(EXIT_FAILURE);
    }

    // print policy and priority
    struct sched_param param;
    int policy = sched_getscheduler(pid);
    if (policy < 0)
    {
        perror("sched_getscheduler");
        exit(EXIT_FAILURE);
    }
    if (sched_getparam(pid, &param) < 0)
    {
        perror("sched_getparam");
        exit(EXIT_FAILURE);
    }
    // SCHED_RR2, SCHED_RR, SCHED_FIFO, SCHED_OTHER
    printf("PID: %d, Policy: %s, Priority: %d\n", pid, (policy == SCHED_FIFO) ? "SCHED_FIFO" : (policy == SCHED_RR) ? "SCHED_RR" : (policy == SCHED_RR2) ? "SCHED_RR2" : "SCHED_OTHER", param.sched_priority);

    // print initial time slice
    struct timespec interval;
    int time_status = sched_rr_get_interval(pid, &interval);
    if (time_status < 0)
    {
        perror("sched_rr_get_interval");
        printf("Error message: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Time quantum: %ld seconds, %ld nanoseconds\n", interval.tv_sec, interval.tv_nsec);

    printf("Enabling timeslice boost\n");
    syscall(__NR_sched_set_time_boost, 0);

    struct timespec new_interval;
    time_status = sched_rr_get_interval(pid, &new_interval);
    if (time_status < 0)
    {
        perror("sched_rr_get_interval");
        printf("Error message: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Time quantum: %ld seconds, %ld nanoseconds\n", new_interval.tv_sec, new_interval.tv_nsec);
    if (new_interval.tv_nsec == 2 * interval.tv_nsec)
    {
        printf("Enabling Test passed: Time slice doubled\n");
    }
    else
    {
        printf("Enabling Test failed: Time slice not doubled\n");
    }

    return 0;
}