#pragma once
#include <boost/qvm/vec.hpp>

class Body
{
public:
    double m;
    boost::qvm::vec<double, 3> r;
    boost::qvm::vec<double, 3> v;

    Body(double, boost::qvm::vec<double, 3>, boost::qvm::vec<double, 3>);
};