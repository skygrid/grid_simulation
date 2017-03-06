//
// Created by ken on 02.12.16.
//
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <cstring>
#include <boost/algorithm/string.hpp>
#include "my_structures.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(yaml, "messages specific for yaml");

static jobType charToEnum(std::string sval);
inline double find_first_date(YAML::Node& root);
std::list<Job*>* GLOBAL_QUEUE;
std::map<std::string, InputFile*>* FILES_DATABASE;
double first_date;

string typesStr[] = {"USER", "DATASTRIPPING", "MERGE", "MCStripping", "DATARECONSTRUCTION", "TURBO",  "MCRECONSTRUCTION", "WGPRODUCTION", "MCMERGE", "UNKNOWN",
                     "MCSIMULATION", "TEST"};

namespace YAML {
	/**
		Jobs from 'jobs.yml' will be cast to cpp object 
		by decode(const Node& node, Job& job) function. 
	*/
    template<>
    struct convert<Job> {

        static bool decode(const Node& node, Job& job) {
            if(!node.IsMap()) {
                //XBT_INFO("xx");
                return false;
            }
            std::string::size_type sz;

            try {
                job.EndExecTime = node["EndExecTime"].as<double>();
                job.Federation = node["Federation"].as<std::string>();
                job.InputFiles = node["InputFiles"].as<std::vector<std::string>>();
                job.JobGroup = node["JobGroup"].as<std::string>();
                job.JobType = node["JobType"].as<std::string>();
                job.LocalJobID = node["LocalJobID"].as<std::string>();
                job.OutputFiles = node["OutputFiles"].as<std::vector<std::string>>();
                job.RescheduleTime = node["RescheduleTime"].as<std::string>();
                //job.Site = node["Site"].as<std::string>();
                job.StartExecTime = node["StartExecTime"].as<double>();
                job.Status = node["Status"].as<std::string>();
                job.SubmissionTime = std::stol(node["SubmissionTime"].as<std::string>(), &sz) - first_date;
                job.SystemPriority = node["SystemPriority"].as<float>();
                job.TotalCPUTime = node["TotalCPUTime(s)"].as<double>() * 1000000000; // Gigaflops
                job.UserPriority = node["UserPriority"].as<float>();
                job.WallClockTime = node["WallClockTime(s)"].as<double>();
            }catch (YAML::BadConversion& badConversion){
                return false;
            }
            return true;
        }
    };
    template <>
    struct convert<InputFile> {
    /**
		Input datasets from 'bkk.yml' will be cast to cpp object 
		by decode(const Node& node, InputFile& input) function.
	*/

        static bool decode(const Node& node, InputFile& input) {
            if(!node.IsMap()) {
                return false;
            }
            try {
                std::string::size_type sz;
                input.events = node["#events"].as<std::string>();
                input.BKKPath = node["BKKPath"].as<std::vector<std::string>>();
                input.DataQuality = node["DataQuality"].as<std::string>();
                input.Replica = node["Replica"].as<std::string>();
                input.RunNumber = node["RunNumber"].as<std::string>();

                if (node["Size"].as<std::string>().compare("None") != 0){
                    input.Size = node["Size"].as<double>();
                } else input.Size = 0;

                input.Storages = node["Storages"].as<std::vector<std::string>>();

            }catch (YAML::BadConversion& badConversion){
                return false;
            }
            return true;
        }
    };
}

static int parse_input_data(){
	/**
		Parse `bkk` file and store Inputfile object into 
		FILES_DATABASE.

        Parameters:
        -----------
        None
	*/

    FILES_DATABASE = new std::map<std::string, InputFile*>;
    YAML::Node root = YAML::LoadFile(input_files_file);

    for (auto it = root.begin(); it != root.end(); ++it) {
        try {
            InputFile* infl = new InputFile;
            *infl = it->second.as<InputFile>();
            std::string key = it->first.as<std::string>();
            infl->name = key;
            FILES_DATABASE->insert(make_pair(key, infl));
        }catch (YAML::TypedBadConversion<InputFile>& badConversion){
            XBT_INFO("INVALID INPUT DATA:YAML");
        }catch (YAML::InvalidNode& node){
            XBT_INFO("INVALID NODE:YAML");
        }
    }
    return 0;
}

static int parse_jobs() {
	/**
		Parses 'jobs.yml', cast to cpp object and push into job_queue.
        
        Parameters:
        -----------
        None		
	*/
    size_t num_bad_conversion = 0;
    size_t invalid_node_num = 0;
    size_t normal_node = 0;

    GLOBAL_QUEUE = new std::list<Job*>;
    XBT_INFO(jobs_file.c_str());
    YAML::Node root = YAML::LoadFile(jobs_file);

    first_date = find_first_date(root);

    for (auto it = root.begin(); it != root.end(); ++it) {
        try {
            Job* job = new Job;
            *job = it->second.as<Job>();
            long key = it->first.as<long>();
            job->JobId = key;
            job->type = charToEnum(job->JobType);
            GLOBAL_QUEUE->push_back(job);
            ++normal_node;
        }catch(YAML::TypedBadConversion<Job>& badConversion) {
            num_bad_conversion++;
            //XBT_INFO("INVALID NODE %s", it->first.as<std::string>().c_str());
        }catch (YAML::InvalidNode& node){
            //XBT_INFO("INVALID NODE %s", it->first.as<std::string>().c_str());
            //XBT_INFO("INVALID NODE %zd", ++invalid_node_num);
            invalid_node_num++;
        }
    }

    XBT_INFO("Normal: %zd\tBadConversion: %zd\tInvalid_num: %zd", normal_node, num_bad_conversion, invalid_node_num);

    return 0;
}

inline double find_first_date(YAML::Node& root){
    return root.begin()->second["SubmissionTime"].as<double>();
}

static jobType charToEnum(std::string& sval)
{
	/**
		takes string @sval and cast it to job TYPE (Reconctruction, Monte-Carlo, etc.).
		Parameters:
        -----------
        @sval -- string representation of type
        @return enum jobType		
	*/

    for (int enumInt = USER; enumInt != LAST; enumInt++)
        if (boost::iequals(sval, typesStr[enumInt])){
            jobType type = static_cast<jobType>(enumInt);
            return type;
        }
    return UNKNOWN;
}

int input(){
	/**
		Starts parsing 'jobs.yml' and 'bkk.yml'
	*/
    parse_jobs();
    parse_input_data();
    return 0;
}
