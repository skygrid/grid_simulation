//
// Created by ken on 02.12.16.
//

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <cstring>

using std::cout;

struct Job{
    long JobId;
    double EndExecTime;
    std::string Federation;
    std::vector<std::string> InputFiles;
    std::string JobGroup;
    std::string JobType;
    long LocalJobID;
    std::vector<std::string> OutputFiles;
    std::string RescheduleTime;
    std::string Site;
    double StartExecTime;
    std::string Status;
    long SubmissionTime;
    float SystemPriority;
    double TotalCPUTime; // in seconds
    float UserPriority;
    double WallClockTime;
};
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
}



int parse_input_data() {
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
