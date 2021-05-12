#include "script_reader.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <boost/qvm/vec.hpp>
#include <boost/algorithm/string.hpp>
#include "Body.h"

// load data from file path provided by command line arg
initial_state load_input_data(char* input_file_path)
{
    std::ifstream input_file(input_file_path, std::ios_base::in);

    if (!input_file.is_open())
    {
        std::cout << "error: could not open" << input_file_path << std::endl;
        exit(EXIT_FAILURE);
    }

    initial_state input_state;
    double m;
    boost::qvm::vec<double, 3> r, v;
    std::string line;
    std::vector<std::string> parsed_line;

    // add conditionals to replicated Julia code
    while (getline(input_file, line))
    {
        boost::algorithm::split(parsed_line, line, boost::is_any_of(" "));

        if (parsed_line[0] == "time")
        {
            input_state.t = atof(parsed_line[1].c_str());
        }
        else if (parsed_line[0] == "step_size")
        {
            input_state.dt = atof(parsed_line[1].c_str());
        }
        else if (parsed_line[0] == "save_interval")
        {
            input_state.save_interval = atof(parsed_line[1].c_str());
        }
        else if (parsed_line[0] == "body")
        {
            m =   atof(parsed_line[1].c_str());
            r = { atof(parsed_line[2].c_str()),
                  atof(parsed_line[3].c_str()),
                  atof(parsed_line[4].c_str()) };
            v = { atof(parsed_line[5].c_str()),
                  atof(parsed_line[6].c_str()),
                  atof(parsed_line[7].c_str()) };

            input_state.system.push_back(Body(m, r, v));
        }
        else
        {
            std::cout << "error: incorrect syntax: " << line << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return input_state;
}