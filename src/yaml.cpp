//
// Created by ken on 02.12.16.
//

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <cstring>
#include <boost/algorithm/string.hpp>
#include "my_structures.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(yaml, "messages specific for yaml");

static jobType charToEnum(string sval);
std::list<Job*>* GLOBAL_QUEUE;
std::map<std::string, InputFile*>* FILES_DATABASE;

string typesStr[] = {"USER", "DATASTRIPPING", "MERGE", "MCStripping", "DATARECONSTRUCTION", "TURBO",  "MCRECONSTRUCTION", "WGPRODUCTION", "MCMERGE", "UNKNOWN",
                     "MCSIMULATION", "TEST"};

namespace YAML {
    template<>
    struct convert<Job> {

        static bool decode(const Node& node, Job& job) {
            if(!node.IsMap()) {
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
                job.Site = node["Site"].as<std::string>();
                job.StartExecTime = node["StartExecTime"].as<double>();
                job.Status = node["Status"].as<std::string>();
                job.SubmissionTime = std::stol(node["SubmissionTime"].as<std::string>(), &sz) - 1475269223;
                job.SystemPriority = node["SystemPriority"].as<float>();
                job.TotalCPUTime = node["TotalCPUTime(s)"].as<double>();
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

    FILES_DATABASE = new map<std::string, InputFile*>;
    YAML::Node root = YAML::LoadFile("InputData/bkk2.yml");

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

    GLOBAL_QUEUE = new std::list<Job*>;
    YAML::Node root = YAML::LoadFile("InputData/jobs2.yml");
    int i = 0;
    for (auto it = root.begin(); it != root.end(); ++it) {
        try {
            Job* job = new Job;
            *job = it->second.as<Job>();
            long key = it->first.as<long>();
            job->JobId = key;
            job->type = charToEnum(job->JobType);
            GLOBAL_QUEUE->push_back(job);
        }catch(YAML::TypedBadConversion<Job>& badConversion) {
            //XBT_INFO("INVALID JOB");
        }catch (YAML::InvalidNode& node){
            //XBT_INFO("INVALID NODE %s", it->first.as<std::string>().c_str());
        }

    }

    return 0;
}

static jobType charToEnum(string sval)
{

    for (int enumInt = USER; enumInt != LAST; enumInt++)
        if (boost::iequals(sval, typesStr[enumInt])){
            jobType type = static_cast<jobType>(enumInt);
            return type;
        }
    return UNKNOWN;
}

int input(){
    parse_jobs();
    parse_input_data();
    return 0;
}
