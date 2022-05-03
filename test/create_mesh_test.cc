
#include <GeometricPrimitives.h>
#include <Visualisation.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace MEGA;

int main(int argc, char** argv) {
    Model a = Plane();
    viewModel(a, argc, argv);
}
