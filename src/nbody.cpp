#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/mat.hpp>
#include "Body.h"
#include "script_reader.h"

const double G = 100.0;
const double S = 0.05;

boost::qvm::vec<double, 3> acceleration_gravity(Body& b1, Body& b2)
{
    boost::qvm::vec<double, 3> r = b2.r - b1.r;
    boost::qvm::vec<double, 3> numerator = G * b2.m * r;
    double denominator = pow((pow(boost::qvm::mag(r), 2) + pow(S, 2)), 1.5);
    return numerator / denominator;
}

boost::qvm::vec<double, 3> net_acceleration(Body& b, std::vector<Body>& system)
{
    boost::qvm::vec<double, 3> a = { 0, 0, 0 };
    int i = 0;

    for (std::vector<Body>::const_iterator it = std::begin(system);
        it != std::end(system); it++, i++)
    {
        if (b.r != system[i].r)
        {
            a += acceleration_gravity(b, system[i]);
        }
    }

    return a;
}

// velocity verlet integrator - step forward once by dt
void verlet_step(std::vector<Body>& system, const double& dt)
{
    int i = 0;

    for (std::vector<Body>::const_iterator it = std::begin(system);
        it != std::end(system); it++, i++)
    {
        boost::qvm::vec<double, 3> a1 = net_acceleration(system[i], system);
        system[i].r += system[i].v * dt + 0.5 * a1 * pow(dt, 2);
        boost::qvm::vec<double, 3> a2 = net_acceleration(system[i], system);
        system[i].v += 0.5 * (a1 + a2) * dt;
    }
}

void integrate(std::vector<Body>& system, double t, double dt, double save_dt,
    char* output_file_path)
{
    double t_current = 0;
    double t_save_current = 0;
    std::ofstream output_file(output_file_path);
    output_file << "t\tid\tm\trx\try\trz\n";
    output_file << std::scientific;
    int i = 0;

    while (t_current < t)
    {
        verlet_step(system, dt);

        if (t_save_current >= save_dt)
        {
            // save to file with format time \t id \t mass \t rx \t ry \t rz
            for (std::vector<Body>::const_iterator it = std::begin(system);
                it != std::end(system); it++, i++)
            {
                output_file << t_current << "\t"
                    << i + 1 << "\t"
                    << system[i].m << "\t"
                    << boost::qvm::A<0>(system[i].r) << "\t"
                    << boost::qvm::A<1>(system[i].r) << "\t"
                    << boost::qvm::A<2>(system[i].r) << "\n";
            }

            i = 0;
            t_save_current = 0;
        }
        else
        {
            t_save_current += dt;
        }

        t_current += dt;
    }
}

int main(int argc, char* argv[])
{
    if (argc == 3)
    {
        char* input_file_path = argv[1];
        char* output_file_path = argv[2];
        initial_state input_state = load_input_data(input_file_path);
        integrate(input_state.system, input_state.t, input_state.dt,
            input_state.save_interval, output_file_path);
    }
    else
    {
        std::cout << "error: takes two args: arg1 = input_file_path"
            << " arg2 = output_file_path" << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
