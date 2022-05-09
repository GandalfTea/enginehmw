
#include <GeometricPrimitives.h>
#include <Visualisation.h>
//#include <IO.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace MEGA;

int main(int argc, char** argv) {
    //Model a = Plane(1, 5, 60, 200);
    Model a = Sphere(1, 200, 200);
    //Model a = loadModel("../models/demo/teapot.obj");
    viewModel(a, MEGA_TRIANGLES, argc, argv);
}
