//
// Created by ken on 05.12.16.
//
#include <map>
#include <string>
#include "my_structures.h"

static std::map<std::string, InputFile*> input_files;

inline std::map<std::string, InputFile*> get_input_files(){

    return input_files;
}