#include <iostream>
#include "interpolation.hpp"

using namespace std;

int main() {
    // Fit Interpolation Model
    Interpolation model;
    RiftPoint p1 = { 0.0, 0.0, 0.0, 0.0 };
    RiftPoint p2 = { 1.0, 1.0, 2.0, 3.0 };
    RiftPoint p3 = { 5.0, 0.1, 0.2, 0.3 };
    RiftPoint p4 = { 10.0, 10.0, 10.0, 10.0 };
    vector<RiftPoint> points = { p1, p2, p3, p4 };
    model.fit(points);
    // Test Interpolation Model
    RiftPoint tp1 = model.evaluate(0);
    cout << tp1.time << "," << tp1.x << "," << tp1.y << "," << tp1.width
            << endl;
    RiftPoint tp2 = model.evaluate(0.2);
    cout << tp2.time << "," << tp2.x << "," << tp2.y << "," << tp2.width
            << endl;
    RiftPoint tp3 = model.evaluate(1.0);
    cout << tp3.time << "," << tp3.x << "," << tp3.y << "," << tp3.width
            << endl;
    RiftPoint tp4 = model.evaluate(4.0);
    cout << tp4.time << "," << tp4.x << "," << tp4.y << "," << tp4.width
            << endl;
    RiftPoint tp5 = model.evaluate(5.0);
    cout << tp5.time << "," << tp5.x << "," << tp5.y << "," << tp5.width
            << endl;
    RiftPoint tp6 = model.evaluate(9.0);
    cout << tp6.time << "," << tp6.x << "," << tp6.y << "," << tp6.width
            << endl;
    RiftPoint tp7 = model.evaluate(10.0);
    cout << tp7.time << "," << tp7.x << "," << tp7.y << "," << tp7.width
            << endl;
}
