
#include <GeometricPrimitives.h>
#include <Visualisation.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace MEGA;

int main(int argc, char** argv) {
    //Model a = Plane();
    Model a = Sphere(1, 30, 30);
    viewModel(a, MEGA_TRIANGLES, argc, argv);
}
