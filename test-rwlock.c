#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "tbb_rwlock.h"

const int NREADER = 6;
const int NWRITER = 2;

const int NTHR = NREADER + NWRITER;
const int NITER = 1000000;

static tbb_rwlock_t rwlock;
static volatile uint32_t shared[3];

static volatile uint32_t wflag;
// Wait on a flag.
void wait_flag(volatile uint32_t *flag, uint32_t expect) {
    __sync_fetch_and_add((uint32_t *)flag, 1);
    while (*flag != expect);
}

/* Debug purpose */
// static int wcnt;
void *writer(void *dummy) {
    int i;
    wait_flag(&wflag, NTHR);

    for (i = 0; i < NITER; i++) {
        tbb_start_write(&rwlock);
        /*wcnt++;*/
        shared[0]++;
        shared[1]++;
        shared[2]++;
        /*wcnt--;*/
        tbb_end_write(&rwlock);
    }
    return NULL;
}

// static int rcnt;
void *reader(void *dummy) {
    int i;
    wait_flag(&wflag, NTHR);

    for (i = 0; i < NITER; i++) {
        tbb_start_read(&rwlock);
        /*rcnt++;*/
        uint32_t val = shared[0];
        if ((shared[1] != val) || (shared[2] != val)) {
            printf("ERROR");
            exit(1);
        }
        /*rcnt--;*/
        tbb_end_read(&rwlock);
    }
    return NULL;
}

int main(int argc, const char *argv[])
{
    pthread_t thr_reader[NREADER], thr_writer[NWRITER];
    int i;

    // Start thread
    for (i = 0; i < NREADER; i++) {
        if (pthread_create(&thr_reader[i], NULL, reader, NULL) != 0) {
            perror("thread creating failed");
        }
    }
    for (i = 0; i < NWRITER; i++) {
        if (pthread_create(&thr_writer[i], NULL, writer, NULL) != 0) {
            perror("thread creating failed");
        }
    }

    // join thread
    for (i = 0; i < NREADER; i++)
        pthread_join(thr_reader[i], NULL);
    for (i = 0; i < NWRITER; i++)
        pthread_join(thr_writer[i], NULL);

    /*printf("\n\n");*/

    return 0;
}
