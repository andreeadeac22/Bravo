#include <iostream>
#include "interpolation.hpp"

using namespace std;

void Interpolation::fit(std::vector<RiftPoint> points) {
    int N = points.size();
    std::sort(points.begin(), points.end(),
            [](const RiftPoint &a, const RiftPoint &b)
            {
                return a.time < b.time;
            });
    std::vector<double> t(N), x(N), y(N), w(N);
    for (int i = 0; i < N; i++) {
        t[i] = points[i].time;
        x[i] = points[i].x;
        y[i] = points[i].y;
        w[i] = points[i].width;
    }
    s_x.set_points(t, x);
    s_y.set_points(t, y);
    s_w.set_points(t, w);
}

RiftPoint Interpolation::evaluate(double time) {
    RiftPoint point;
    point.time = time;
    point.x = s_x(time);
    point.y = s_y(time);
    point.width = s_w(time);
    return point;
}
