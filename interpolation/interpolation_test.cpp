#include <iostream>
#include "interpolation.hpp"

using namespace std;

int main() {
    // Fit Interpolation Model
    Interpolation model;
    RiftPoint p1 = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    RiftPoint p2 = { 1.0, 1.0, 2.0, 3.0, 4.0 };
    RiftPoint p3 = { 5.0, 0.1, 0.2, 0.3, 0.4 };
    RiftPoint p4 = { 10.0, 10.0, 10.0, 10.0, 10.0 };
    vector<RiftPoint> points = { p1, p2, p3, p4 };
    model.fit(points);
    // Test Interpolation Model
    RiftPoint tp1 = model.evaluate(0);
    cout << tp1.time << "," << tp1.x1 << "," << tp1.y1 << "," << tp1.x2 << ","
            << tp1.y2 << endl;
    RiftPoint tp2 = model.evaluate(0.2);
    cout << tp2.time << "," << tp2.x1 << "," << tp2.y1 << "," << tp2.x2 << ","
            << tp2.y2 << endl;
    RiftPoint tp3 = model.evaluate(1.0);
    cout << tp3.time << "," << tp3.x1 << "," << tp3.y1 << "," << tp3.x2 << ","
            << tp3.y2 << endl;
    RiftPoint tp4 = model.evaluate(4.0);
    cout << tp4.time << "," << tp4.x1 << "," << tp4.y1 << "," << tp4.x2 << ","
            << tp4.y2 << endl;
    RiftPoint tp5 = model.evaluate(5.0);
    cout << tp5.time << "," << tp5.x1 << "," << tp5.y1 << "," << tp5.x2 << ","
            << tp5.y2 << endl;
    RiftPoint tp6 = model.evaluate(9.0);
    cout << tp6.time << "," << tp6.x1 << "," << tp6.y1 << "," << tp6.x2 << ","
            << tp6.y2 << endl;
    RiftPoint tp7 = model.evaluate(10.0);
    cout << tp7.time << "," << tp7.x1 << "," << tp7.y1 << "," << tp7.x2 << ","
            << tp7.y2 << endl;
}
