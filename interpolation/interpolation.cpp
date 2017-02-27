#include "interpolation.hpp"

using namespace std;

void Interpolation::fit(std::vector<RiftPoint> points) {
    int N = points.size();
    std::sort(points.begin(), points.end(),
            [](const RiftPoint &a, const RiftPoint &b)
            {
                return a.time < b.time;
            });
    std::vector<double> t(N), x1(N), y1(N), x2(N), y2(N);
    for (int i = 0; i < N; i++) {
        t[i] = points[i].time;
        x1[i] = points[i].x1;
        y1[i] = points[i].y1;
        x2[i] = points[i].x2;
        y2[i] = points[i].y2;
    }
    s_x1.set_points(t, x1);
    s_y1.set_points(t, y1);
    s_x2.set_points(t, x2);
    s_y2.set_points(t, y2);
}

RiftPoint Interpolation::evaluate(double time) {
    RiftPoint point;
    point.time = time;
    point.x1 = s_x1(time);
    point.y1 = s_y1(time);
    point.x2 = s_x2(time);
    point.y2 = s_y2(time);
    return point;
}
