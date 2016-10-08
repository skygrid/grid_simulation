//
// Created by ken on 10.09.16.
//

#include <vector>
#include <list>
#include "messages.h"
#include "myfunc_list.h"
using namespace std;

XBT_LOG_NEW_DEFAULT_CATEGORY(ll, "messages specific for ll");

list<Job*>* create_current_queue(){
    std::list<Job*>* local_queue = new list<Job*>;

    for (auto it = global_queue.begin(); it != global_queue.end(); ++it) {
        if ((*it) && ((*it)->submissionTime < MSG_get_clock())){
            local_queue->push_back(*it);
        }else break;
    }

    return local_queue;
}
