//
// Created by ken on 29.07.16.
//
#include <simgrid/msg.h>
#include "my_structures.h"
#include "myfunc_list.h"


XBT_LOG_NEW_DEFAULT_CATEGORY(job_requester, "messages specific for cm");

int job_requester(int argc, char* argv[]){
	/**
		@type simgrid process (run on all tiers)
		Every @timeout checks an amount of free cores.
		If this amount is bigger than some quantity then this process sends a job request
		 to scheduler to get new batch of jobs. 		 
	*/
    std::string host_name = MSG_host_get_name(MSG_host_self());
    std::string CERN = "CERN";
    msg_task_t task = NULL;
    double timeout = 1000;
    long freeCoreAmount;
    int fullCoreAmount = MSG_host_get_core_number(MSG_host_self());
    MSG_process_sleep(0.01);

    while (1){
        freeCoreAmount = fullCoreAmount - xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error") -
                xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");

        MSG_sem_acquire(sem_requester);
        if (GLOBAL_QUEUE->empty()){
            MSG_sem_release(sem_requester);
            break;
        }

        if (freeCoreAmount > 0){
            JobBatchRequest* jobRequest = new JobBatchRequest;
            jobRequest->coreAmount = freeCoreAmount;

            task = MSG_task_create("request", 0.0, MESSAGES_SIZE, jobRequest);

            plusLinkCounter(host_name, CERN);
            msg_error_t err = MSG_task_send(task, "scheduler");

            switch(err){
                case MSG_OK:
                    minusLinkCounter(host_name, CERN);
                    break;
                case MSG_TRANSFER_FAILURE:
                    minusLinkCounter(host_name, CERN);
                    MSG_task_destroy(task);
                    task = NULL;
                    break;
                case MSG_HOST_FAILURE:
                    MSG_task_destroy(task);
                    task = NULL;
                    break;
            }
        }else MSG_sem_release(sem_requester);

        MSG_process_sleep(timeout);
    }
    return 0;
}


