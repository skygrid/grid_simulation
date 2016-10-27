//
// Created by ken on 11.09.16.
//
using namespace std;
#include <simgrid/msg.h>
#include <vector>
#include "myfunc_list.h"
#include "messages.h"


XBT_LOG_NEW_DEFAULT_CATEGORY(matcher, "messages specific for matcher");


void check_files_availability(Job* jobInfo){
    string input_file_path;
    msg_file_t file;

    string* dataLocations[] = {&jobInfo->dataLocHost1, &jobInfo->dataLocHost2, &jobInfo->dataLocHost3, &jobInfo->dataLocHost4,
                               &jobInfo->dataLocHost5, &jobInfo->dataLocHost6, &jobInfo->dataLocHost7, &jobInfo->dataLocHost8,
                               &jobInfo->dataLocHost9, &jobInfo->dataLocHost10};
    string* storageTypes[] = {&jobInfo->storageType1, &jobInfo->storageType2, &jobInfo->storageType3, &jobInfo->storageType4,
                             &jobInfo->storageType5, &jobInfo->storageType6, &jobInfo->storageType7, &jobInfo->storageType8,
                             &jobInfo->storageType9, &jobInfo->storageType10};

    int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);

    for (int i = 0; i < n; ++i) {
        if (!(*dataLocations[i]).compare("0")){
            continue;
        }

        input_file_path = "/" + *dataLocations[i] + *storageTypes[i] + "/" + jobInfo->inputFileName;
        if (!input_file_path.compare("/CNAF1/dataset66")){
            XBT_INFO("sigein");
        }
        file = MSG_file_open(input_file_path.c_str(), NULL);
        if (MSG_file_get_size(file) == 0){
            *dataLocations[i] = "0";
        }
        MSG_file_close(file);
    }
}

vector<Job*>* matcher(long amountRequestedJob){

    int amount_of_matched_jobs = 0;


    vector<Job*>* jobBatch = new vector<Job*>;
    std::list<Job*>* local_queue = global_queue;//create_current_queue();

    if (local_queue->size() < amountRequestedJob){
        amountRequestedJob = local_queue->size();
    }

    for (auto i = local_queue->begin(); i != local_queue->end() && amount_of_matched_jobs < amountRequestedJob ;) {
        (*i)->startSchedulClock = MSG_get_clock();
        (*i)->stExecClock = 0;
        (*i)->endExecClock = 0;
        (*i)->successExecuted = 0;

        jobBatch->push_back(*i);
        amount_of_matched_jobs++;

        i = global_queue->erase(i);
    }
    //delete local_queue;
    return jobBatch;
}

vector<Job*>* matcher_DAM(long amountRequestedJob, const string host){

    int amount_of_matched_jobs = 0;

    vector<Job*>* jobBatch = new vector<Job*>;
    std::list<Job*>* local_queue = global_queue;//create_current_queue();

    if (amountRequestedJob > local_queue->size()){
        amountRequestedJob = local_queue->size();
    }


    for (auto i = local_queue->begin(); i != local_queue->end() && amount_of_matched_jobs < amountRequestedJob ;) {
        check_files_availability(*i);
        if ((*i)->type == MCSIMULATION){
            (*i)->startSchedulClock = MSG_get_clock();
            (*i)->successExecuted = 0;
            amount_of_matched_jobs++;
            jobBatch->push_back(*i);

            i = global_queue->erase(i);
            continue;
        }
        string dataLocations[] = {(*i)->dataLocHost1, (*i)->dataLocHost2, (*i)->dataLocHost3, (*i)->dataLocHost4,
                                  (*i)->dataLocHost5, (*i)->dataLocHost6, (*i)->dataLocHost7, (*i)->dataLocHost8,
                                  (*i)->dataLocHost9, (*i)->dataLocHost10};
        string storage_types[] = {(*i)->storageType1, (*i)->storageType2, (*i)->storageType3, (*i)->storageType4,
                                 (*i)->storageType5, (*i)->storageType6, (*i)->storageType7, (*i)->storageType8,
                                 (*i)->storageType9, (*i)->storageType10};

        int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);

        for (int j = 0; j < n; ++j) {
            if ((!dataLocations[j].compare(host) && !storage_types[j].compare("1"))){
                (*i)->startSchedulClock = MSG_get_clock();
                jobBatch->push_back(*i);

                amount_of_matched_jobs++;
                i = global_queue->erase(i);
                break;
            }
        }

    }

    // If there are no more matched jobs, but job queue still exists
    long remained_jobs = amountRequestedJob - amount_of_matched_jobs;
    if ((remained_jobs > 0) && (local_queue->size() > 0)){
        remained_jobs = (remained_jobs < local_queue->size()) ? remained_jobs : (remained_jobs - local_queue->size());

        vector<Job*>* batch = matcher(remained_jobs);

        unsigned long additional_batch_size= batch->size();

        for (unsigned long i = 0; i < additional_batch_size; ++i) {
            jobBatch->push_back(batch->at(i));
        }
        delete batch;
    }
    //delete local_queue;
    return jobBatch;
}


list<Job*>* create_current_queue(){
    std::list<Job*>* local_queue = new list<Job*>;

    for (auto it = global_queue->begin(); it != global_queue->end(); ++it) {
        if ((*it) && ((*it)->submissionTime < MSG_get_clock())){
            local_queue->push_back(*it);
        }else break;
    }

    return local_queue;
}