//
// Created by ken on 02.12.16.
//

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <cstring>
#include <boost/algorithm/string.hpp>
#include "messages.h"

using std::cout;

namespace YAML {
    template<>
    struct convert<Job> {

        static bool decode(const Node& node, Job& job) {
            if(!node.IsMap()) {
                return false;
            }
            std::string::size_type sz;

            job.EndExecTime = node["EndExecTime"].as<double>();
            job.Federation = node["Federation"].as<std::string>();
            job.InputFiles = node["InputFiles"].as<std::vector<std::string>>();
            job.JobGroup = node["JobGroup"].as<std::string>();
            job.JobType = node["JobType"].as<std::string>();
            job.LocalJobID = std::stol(node["LocalJobID"].as<std::string>(), &sz);
            job.OutputFiles = node["OutputFiles"].as<std::vector<std::string>>();
            job.RescheduleTime = node["RescheduleTime"].as<std::string>();
            job.Site = node["Site"].as<std::string>();
            job.StartExecTime = node["StartExecTime"].as<double>();
            job.Status = node["Status"].as<std::string>();
            job.SubmissionTime = std::stol(node["SubmissionTime"].as<std::string>(), &sz);
            job.SystemPriority = node["SystemPriority"].as<float>();
            job.TotalCPUTime = node["TotalCPUTime(s)"].as<double>();
            job.UserPriority = node["UserPriority"].as<float>();
            job.WallClockTime = node["WallClockTime(s)"].as<double>();

            return true;
        }
    };
    template <>
    struct convert<InputFile> {

        static bool decode(const Node& node, InputFile& job) {
            if(!node.IsMap()) {
                return false;
            }
            std::string::size_type sz;
            cout << node["'#events'"];
            job.events = std::stol(node["#events"].as<std::string>(), &sz);
            job.BKKPath = node["BKKPath"].as<std::vector<std::string>>();
            job.DataQuality = node["DataQuality"].as<std::string>();
            job.Replica = node["Replica"].as<std::string>();
            job.RunNumber = node["RunNumber"].as<double>();
            job.Size = node["Size"].as<double>();
            job.Storages = node["Storages"].as<std::vector<std::string>>();
            return true;
        }
    };
}

int parse_input_data(){
    YAML::Node root = YAML::LoadFile("/home/ken/ClionProjects/yaml/input_data.yaml");
    std::vector<InputFile*> v;
    for (auto it = root.begin(); it != root.end(); ++it) {
        InputFile* id = new InputFile;
        *id = it->second.as<InputFile>();
        std::string key = it->first.as<std::string>();
        id->name = key;
        v.push_back(id);
    }
    return 0;
}

int parse_jobs() {
    YAML::Node root = YAML::LoadFile("/home/ken/ClionProjects/yaml/job.yaml");

    std::vector<Job*> v;
    for (auto it = root.begin(); it != root.end(); ++it) {
        Job* job = new Job;
        *job = it->second.as<Job>();
        long key = it->first.as<long>();
        job->JobId = key;
        v.push_back(job);
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