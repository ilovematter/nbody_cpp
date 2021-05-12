#include "Body.h"

Body::Body(double new_m, boost::qvm::vec<double, 3> new_r,
    boost::qvm::vec<double, 3> new_v)
{
    m = new_m;
    r = new_r;
    v = new_v;
}