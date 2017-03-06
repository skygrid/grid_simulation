//
// Created by ken on 24.05.16.
// Purpose: scheduler function
//

#include <stdio.h>
#include <simgrid/msg.h>
#include "my_structures.h"
#include "myfunc_list.h"
#include <vector>

using namespace std;

XBT_LOG_NEW_DEFAULT_CATEGORY(scheduler, "messages specific for scheduler");

int scheduler(int argc, char* argv[]){
	/**
		This function describes scheduler process. 
		Main goal of scheduler is to listen "scheduler" mailbox 
		and wait for job requests from other hosts.
		After accepting job request, scheduler call `matching` function and get a std::vector of job.
		And as a result, it sends a batch of jobs to requesting host. 
		Scheduler process can live anywhere.

        Simgrid process parameters:
        ---------------------------
        None
	*/
    input();
    XBT_INFO("length of queus is %zd", GLOBAL_QUEUE->size());
    string mailbox = "scheduler";
    msg_task_t task = NULL; msg_task_t taskB = NULL;

    while (1){
        int res = MSG_task_receive(&task, mailbox.c_str());
        // Anomalies
        if (res == MSG_OK){

            if (!strcmp(MSG_task_get_name(task), "finalize")){
                MSG_task_destroy(task);
                break;
            }

            //XBT_INFO("Get job request from %s", MSG_host_get_name(MSG_task_get_source(task)));
            JobBatchRequest* batchRequest = (JobBatchRequest*) MSG_task_get_data(task);
            std::vector<Job*>* batch;

            std::string request_host = MSG_host_get_name(MSG_task_get_source(task));

            batch = matcher_calibration(batchRequest->coreAmount, request_host);
            // Tier2 matcher
//            int tier2 = !strncmp(request_host.c_str(), "Tier2", 5); // is tier2 or not
//
//            if (tier2){
//                batch = matcher_tier2(batchRequest->coreAmount, request_host);
//            }
//            else if (!current_model.compare("simple")){
//                batch = matcher(batchRequest->coreAmount);
//                //batch = matcher_calibration(batchRequest->coreAmount, request_host);
//            }else {
//                batch = matcher_DAM(batchRequest->coreAmount, request_host);
//            }

            taskB = MSG_task_create("", batch->size(), MESSAGES_SIZE, batch);
            //Add new user to link
            //plusLinkCounter(MSG_host_get_name(MSG_host_self()), MSG_host_get_name(MSG_task_get_source(task)));

            switch(MSG_task_send(taskB, request_host.c_str())){
                case MSG_OK:
                    MSG_sem_release(sem_requester);
                    //minusLinkCounter(MSG_host_get_name(MSG_host_self()), MSG_host_get_name(MSG_task_get_source(task)));
                    //XBT_INFO("Send %d jobs after matching to %s", batch->jobsAmount ,MSG_host_get_name(MSG_task_get_source(task)));
                    break;
                case MSG_TRANSFER_FAILURE:
                    //rescheduling(batchRequest->coreAmount);
                    MSG_task_destroy(taskB);
                    taskB = NULL;
                    break;
                case MSG_HOST_FAILURE:
                    MSG_task_destroy(taskB);
                    taskB = NULL;
                    break;
            }

            MSG_task_destroy(task);
            task = NULL;

            delete batchRequest;

        }else if (res == MSG_TRANSFER_FAILURE){
            XBT_INFO("transfer failure occur");
        }
    }
}
