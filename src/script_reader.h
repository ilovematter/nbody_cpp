#pragma once
#include <vector>
#include "Body.h"

struct initial_state
{
    std::vector<Body> system;
    double t, dt, save_interval;
};

initial_state load_input_data(char* input_file_path);