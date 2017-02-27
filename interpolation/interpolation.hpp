#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <pqxx/pqxx>
#include <vector>
#include "spline.h"

// Holds data to represent state of rift.
struct RiftPoint {
    double time;
    // Two points on edges of rift
    double x1;
    double y1;
    double x2;
    double y2;
};

// Class used to interpolate rift.
class Interpolation {
public:
    // The spline models.
    tk::spline s_x1;
    tk::spline s_y1;
    tk::spline s_x2;
    tk::spline s_y2;

    // Fit interpolation model.
    void fit(std::vector<RiftPoint> points);

    // Evaluates the fitted model to find the rift at a certain time.
    RiftPoint evaluate(double time);
};

#endif // INTERPOLATION_H
