//
// Created by ken on 11.09.16.
// Each function here corresponds to matching strategy applied in scheduler
//

using namespace std;
#include <simgrid/msg.h>
#include <vector>
#include "myfunc_list.h"
#include "my_structures.h"


XBT_LOG_NEW_DEFAULT_CATEGORY(matcher, "messages specific for matcher");

std::vector<Job*>* matcher_calibration(long amountRequestedJob, const std::string& hostname)
{
	/**
		@type function
		Match jobs according to historical base.
		(Because we know where job will be running and when it will be scheduled)
	*/
    int amount_of_matched_jobs = 0;
    double clock = MSG_get_clock();

    std::vector<Job*>* jobBatch = new std::vector<Job*>;
    std::list<Job*>* local_queue = GLOBAL_QUEUE;

    if (local_queue->size() < amountRequestedJob){
        amountRequestedJob = local_queue->size();
    }

    for (auto job = local_queue->begin(); job != local_queue->end() && amount_of_matched_jobs < amountRequestedJob && (*job)->SubmissionTime < clock;) {
        auto job_ptr = *job;
        if (job_ptr->Federation.compare(hostname) == 0){
            job_ptr->startSchedulClock = MSG_get_clock();
            job_ptr->StartExecTime = 0;
            job_ptr->EndExecTime = 0;
            job_ptr->successExecuted = 0;

            jobBatch->push_back(*job);
            amount_of_matched_jobs++;

            job = GLOBAL_QUEUE->erase(job);
        } else{
            job++;
        }
    }

    return jobBatch;
}


std::vector<Job*>* matcher(long amountRequestedJob){
	/**
		This matching function describes strategy when :
		scheduler takes jobs from a queue according to their position in it (FIFO).
		@amountRequestedJob parameters tells how much jobs will be taken from the queue.
		@return a vector (batch) of jobs
	*/

    int amount_of_matched_jobs = 0;


    vector<Job*>* jobBatch = new vector<Job*>;
    std::list<Job*>* local_queue = GLOBAL_QUEUE; // create_current_queue();

    if (local_queue->size() < amountRequestedJob){
        amountRequestedJob = local_queue->size();
    }

    for (auto i = local_queue->begin(); i != local_queue->end() && amount_of_matched_jobs < amountRequestedJob ;) {
        (*i)->startSchedulClock = MSG_get_clock();
        (*i)->StartExecTime = 0;
        (*i)->EndExecTime = 0;
        (*i)->successExecuted = 0;

        jobBatch->push_back(*i);
        amount_of_matched_jobs++;

        i = GLOBAL_QUEUE->erase(i);
    }
    //delete local_queue;
    return jobBatch;
}

std::vector<Job*>* matcher_DAM(long amountRequestedJob, const string& host){
	/**
		This matching function describes strategy when :
		scheduler takes job from a queue when job meets two conditons:
			1) Host requesting a job has input data for this job
			2) The earliest jobs take priority over latest ones
		@amountRequestedJob parameter tells how much jobs will be taken from the queue.
		@host parameter is name of host jobs will be sent to.
		@return a vector (batch) of jobs
	*/

    /*int amount_of_matched_jobs = 0;

    vector<Job*>* jobBatch = new vector<Job*>;
    std::list<Job*>* local_queue = GLOBAL_QUEUE;        //create_current_queue();

    if (amountRequestedJob > local_queue->size()){
        amountRequestedJob = local_queue->size();
    }


    for (auto i = local_queue->begin(); i != local_queue->end() && amount_of_matched_jobs < amountRequestedJob;) {
        auto current_i = i;
        check_files_availability(*i);
        if ((*i)->type == MCSIMULATION){
            (*i)->startSchedulClock = MSG_get_clock();
            (*i)->successExecuted = 0;
            amount_of_matched_jobs++;
            jobBatch->push_back(*i);

            i = GLOBAL_QUEUE->erase(i);
            continue;
        }
        string dataLocations[] = {(*i)->dataLocHost1, (*i)->dataLocHost2, (*i)->dataLocHost3, (*i)->dataLocHost4,
                                  (*i)->dataLocHost5, (*i)->dataLocHost6, (*i)->dataLocHost7, (*i)->dataLocHost8,
                                  (*i)->dataLocHost9, (*i)->dataLocHost10};
        string storage_types[] = {(*i)->storageType1, (*i)->storageType2, (*i)->storageType3, (*i)->storageType4,
                                 (*i)->storageType5, (*i)->storageType6, (*i)->storageType7, (*i)->storageType8,
                                 (*i)->storageType9, (*i)->storageType10};

        int n = (int) (*i)->InputFiles;

        for (int j = 0; j < n; ++j) {
            if ((!dataLocations[j].compare(host) && !storage_types[j].compare("1"))){
                (*i)->startSchedulClock = MSG_get_clock();
                jobBatch->push_back(*i);

                amount_of_matched_jobs++;
                i = GLOBAL_QUEUE->erase(i);
                break;
            }
        }
        if (current_i == i) i++;

    }

    // If there are no more matched jobs, but job queue still exists
    long remained_jobs = amountRequestedJob - amount_of_matched_jobs;
    if ((remained_jobs > 0) && !local_queue->empty()){
        remained_jobs = (remained_jobs < local_queue->size()) ? remained_jobs : (remained_jobs - local_queue->size());

        vector<Job*>* batch = matcher(remained_jobs);

        unsigned long additional_batch_size= batch->size();

        for (unsigned long i = 0; i < additional_batch_size; ++i) {
            jobBatch->push_back(batch->at(i));
        }
        delete batch;
    }
    //delete local_queue;
    return jobBatch;*/
}

std::vector<Job*>* matcher_tier2(long amountRequestedJob, const string& host){
	/**
		This matching function describes strategy for TIER-2 hosts. 
		Only `MCSIMULATION` jobs are sent to TIER2. 
		@amountRequestedJob parameter tells how much jobs should be taken from the queue.
		@host parameter is name of host jobs will be sent to.
		@return a vector (batch) of jobs
	*/
		
    int amount_of_matched_jobs = 0;

    vector<Job*>* jobBatch = new vector<Job*>;
    std::list<Job*>* local_queue = GLOBAL_QUEUE;        //create_current_queue();

    if (amountRequestedJob > local_queue->size()){
        amountRequestedJob = local_queue->size();
    }

    for (auto i = local_queue->begin(); i != local_queue->end() && amount_of_matched_jobs < amountRequestedJob;) {
        check_files_availability(*i);
        if ((*i)->type == MCSIMULATION){
            (*i)->startSchedulClock = MSG_get_clock();
            (*i)->successExecuted = 0;
            amount_of_matched_jobs++;
            jobBatch->push_back(*i);

            i = GLOBAL_QUEUE->erase(i);
        } else i++;
    }
    return jobBatch;
}

std::list<Job*>* create_current_queue(){
	/** In the current realization of program, jobs (from all workflow) are stored in the `jobs.yml`.
		This function creates a queue which contains only jobs have pushed by now.
		@return std::list of Job pointers. 
	*/
    std::list<Job*>* local_queue = new list<Job*>;

    for (auto it = GLOBAL_QUEUE->begin(); it != GLOBAL_QUEUE->end(); ++it) {
        if ((*it) && ((*it)->SubmissionTime < MSG_get_clock())){
            local_queue->push_back(*it);
        }else break;
    }

    return local_queue;
}

void check_files_availability(Job* jobInfo){
    /*string input_file_path;
    msg_file_t file;


    jobInfo->InputFiles =

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
        MSG_file_close(file);*/
}

