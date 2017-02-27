//
// Created by ken on 23.05.16.
//

#include <simgrid/msg.h>
#include <yaml-cpp/yaml.h>
#include <chrono>
#include "my_structures.h"
#include "myfunc_list.h"

int scheduler(int argc, char *argv[]);
int tier1(int argc, char *argv[]);
int evil(int argc, char* argv[]);
int initialize_file_labels(int argc, char* argv[]);
int delete_unpopular_file(int argc, char* argv[]);
int killer(int argc, char* argv[]);
int tracer(int argc, char* argv[]);


msg_sem_t sem_requester;

// Config variables
std::string path_to_output;
std::string current_model;
std::string jobs_file;
std::string input_files_file;

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_app_masterworker, "Messages specific for this msg example");

/**
	This function starts and ends simulation workflow.
	`main` accepts following arguments:
	@--cfg=tracing:yes  enables tracing functionality of simgrid
	@--cfg=tracing/platform:yes  enables platform tracing functionality of simgrid
	@--cfg=tracing/filename:<path to trace file>  defines path to file simgrid writes to
	@--cfg=maxmin/concurrency_limit:100000	maximum number of simultaneously running processes in simulation 
	@--cfg=storage/max_file_descriptors:220000"	 maximum number of simultaneously open (and not closed) file descriptors
	
*/
int main(int argc, char *argv[]){

    YAML::Node config = YAML::LoadFile("config.yml");

    const std::string model = config["model"].as<std::string>();
    const std::string platform = config["platform"].as<std::string>();
    const std::string deployment = config["deployment"].as<std::string>();
    path_to_output = config["out.txt"].as<std::string>();
    jobs_file = config["jobs"].as<std::string>();
    input_files_file = config["input"].as<std::string>();

    auto t1 = std::chrono::high_resolution_clock::now();

    MSG_init(&argc, argv);

    MSG_create_environment(platform.c_str());

    declare_trace_variables();

    sem_requester = MSG_sem_init(1);

    MSG_function_register("evil", evil);
    MSG_function_register("scheduler", scheduler);
    MSG_function_register("tier1", tier1);
    MSG_function_register("killer", killer);
    MSG_function_register("tracer", tracer);

    MSG_function_register("initialize", initialize_file_labels);
    MSG_function_register("delete_unpop_file", delete_unpopular_file);
    MSG_launch_application(deployment.c_str());
    current_model = (char*) model.c_str();

    msg_error_t res = MSG_main();
    XBT_INFO("Simulation time %f", MSG_get_clock());

    fclose(FP);
    MSG_sem_destroy(sem_requester);
    MSG_sem_destroy(sem_link);

    // Clear info of name node

    /*std::map<string, FileData*>::iterator file_itr = name_node->begin();
    while (file_itr != name_node->end()){
        delete (*file_itr).second;
        name_node->erase(file_itr);
        file_itr++;
    }*/

    // delete global_queue and name_node
    delete GLOBAL_QUEUE;
    //delete name_node;

    auto t2 = std::chrono::high_resolution_clock::now();
    XBT_INFO("Real time of simulation: %ld seconds", std::chrono::duration_cast<std::chrono::seconds>(t2-t1).count());


    return res != MSG_OK;
}
