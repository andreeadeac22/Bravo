#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <pqxx/pqxx>
#include <vector>
#include "spline.h"

// Holds data to represent state of rift.
struct RiftPoint {
    double time;
    double x;
    double y;
    double width;
};

// Class used to interpolate rift.
class Interpolation {
public:
    // The spline models.
    tk::spline s_x;
    tk::spline s_y;
    tk::spline s_w;

    // Fit interpolation model.
    void fit(std::vector<RiftPoint> points);

    // Evaluates the fitted model to find the rift at a certain time.
    RiftPoint evaluate(double time);
};

#endif // INTERPOLATION_H
