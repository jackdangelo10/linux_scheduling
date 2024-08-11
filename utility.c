#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

// set scheduling policy and priority given a PID, policy value, and priority value
void scheduling_utility(int pid, int policy, int priority)
{
    struct sched_param param;
    param.sched_priority = priority;
    if (sched_setscheduler(pid, policy, &param) < 0)
    {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }
    // SCHED_FIFO, SCHED_RR, SCHED_RR2, SCHED_OTHER
    printf("PID: %d, Policy: %s, Priority: %d\n", pid, (policy == SCHED_FIFO) ? "SCHED_FIFO" : (policy == SCHED_RR) ? "SCHED_RR" : (policy == SCHED_RR2) ? "SCHED_RR2" : "SCHED_OTHER", priority);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <PID> <Policy> <Priority>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pid = atoi(argv[1]);
    // check for valid pid
    if (pid < 0)
    {
        fprintf(stderr, "Invalid PID\n");
        exit(EXIT_FAILURE);
    }

    // SCHED_FIFO, SCHED_RR, SCHED_RR2
    int policy = (strcmp(argv[2], "SCHED_FIFO") == 0) ? SCHED_FIFO : (strcmp(argv[2], "SCHED_RR") == 0) ? SCHED_RR : (strcmp(argv[2], "SCHED_RR2") == 0) ? SCHED_RR2 : SCHED_OTHER;
    // check for valid policy
    if (policy != SCHED_FIFO && policy != SCHED_RR && policy != SCHED_RR2 && policy != SCHED_OTHER)
    {
        fprintf(stderr, "Invalid Policy\n");
        exit(EXIT_FAILURE);
    }
    // check for valid priority
    int priority = atoi(argv[3]);
    if (priority < sched_get_priority_min(policy) || priority > sched_get_priority_max(policy))
    {
        fprintf(stderr, "Min Priority: %d, Max Priority: %d\n", sched_get_priority_min(policy), sched_get_priority_max(policy));
        fprintf(stderr, "Invalid Priority\n");
        exit(EXIT_FAILURE);
    }

    scheduling_utility(pid, policy, priority);

    return 0;
}