//
// Created by ken on 04.08.16.
//
#include <simgrid/msg.h>

int pileOfFailedTask(){
    msg_sem_t sem = MSG_sem_init(1);
    MSG_sem_acquire(sem);

    MSG_sem_release(sem);
}
