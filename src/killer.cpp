//
// Created by ken on 09.10.16.
//

#include <simgrid/msg.h>
#include <string>
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(killer, "messages specific for killer");

int killer(int argc, char* argv[]){
    const std::string hosts[8] = {"CERN-PROD", "INFN-T1", "IN2P3-CC", "NRC-KI-T1", "pic", "RAL-LCG2", "FZK-LCG2", "NIKHEF-ELPROD"};
    const std::string tier2s[] = {"FR-IN2P3-CPPM", "FR-GRIF", "FR-IN2P3-LAPP", "UK-SouthGrid",
      "FR-IN2P3-LPC", "DE-DESY-LHCB", "IT-INFN-T2", "T2-LATINAMERICA", "PL-TIER2-WLCG",
    "RO-LCG", "RU-RDIG", "ES-LHCb-T2", "CH-CHIPP-CSCS", "UK-London-Tier2", "UK-NorthGrid", "UK-ScotGrid"};

    const double timeout = 1000;
    msg_task_t task = NULL;
    msg_task_t scheduler_task = NULL;

    while (TRUE){
        if (GLOBAL_QUEUE->empty()){
            MSG_process_sleep(100.);

            // Kill tier0 and tier1s
            for (int i = 0; i < 8; ++i) {
                task = MSG_task_create("finalize", 0, MESSAGES_SIZE, NULL);
                //MSG_task_send(task, hosts[i].c_str());
                MSG_task_dsend(task, hosts[i].c_str(), xbt_free_f);
            }

            // kill tier2s
            int tier2_count  = sizeof(tier2s) / sizeof(tier2s[0]);
            for (int j = 0; j < tier2_count; ++j) {
                task = MSG_task_create("finalize", 0, MESSAGES_SIZE, NULL);
                //MSG_task_send(task, tier2s[j-1].c_str());
                MSG_task_dsend(task, tier2s[j].c_str(), xbt_free_f);
            }

            scheduler_task = MSG_task_create("finalize", 0, MESSAGES_SIZE, NULL);
            MSG_task_send(scheduler_task, "scheduler");
            break;
        }
        MSG_process_sleep(timeout);
    }
    return 0;
}

int die_or_not(){
    MSG_sem_acquire(sem_requester);
    if (GLOBAL_QUEUE->empty()){
        MSG_process_kill(MSG_process_self());
        MSG_sem_release(sem_requester);
    }
    MSG_sem_release(sem_requester);
    return 0;
}