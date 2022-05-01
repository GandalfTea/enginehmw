
#include <IO.h>
#include <Object.h>
#include <iostream>
#include <cassert>
#include <chrono>
#include "./helpers.cc"

using namespace std;
using namespace std::chrono;
using namespace MEGA;


int main() {

    std::cout << "\nTesting .obj model loader into active memory: ";
    Model a = loadModel("../models/demo/teapot.obj");

    // Random Sample Vertices
    assert( compare_float( a.vertices.at(0, 0), -3.000000, 0.0005 )); 
    assert( compare_float( a.vertices.at(0, 1),  1.800000, 0.0005 )); 
    assert( compare_float( a.vertices.at(0, 2),  0.0, 0.0005 )); 

    assert( compare_float( a.vertices.at(1, 0), -2.991600, 0.0005 )); 
    assert( compare_float( a.vertices.at(1, 1),  1.800000, 0.0005 )); 
    assert( compare_float( a.vertices.at(1, 2), -0.081000 ,0.0005 )); 

    assert( compare_float( a.vertices.at(2, 0), -2.991600, 0.0005 )); 
    assert( compare_float( a.vertices.at(2, 1),  1.800000, 0.0005 )); 
    assert( compare_float( a.vertices.at(2, 2),  0.081000 ,0.0005 )); 

    assert( compare_float( a.vertices.at(30, 0), -2.920870, 0.0005 )); 
    assert( compare_float( a.vertices.at(30, 1),  1.908779, 0.0005 )); 
    assert( compare_float( a.vertices.at(30, 2),  0.189000, 0.0005 )); 

    // Random Sample Faces
    assert( a.triangles.at(0, 0) == 2909);
    assert( a.triangles.at(0, 1) == 2921);
    assert( a.triangles.at(0, 2) == 2939);

    assert( a.triangles.at(1, 0) == 2939);
    assert( a.triangles.at(1, 1) == 2931);
    assert( a.triangles.at(1, 2) == 2909);

    assert( a.triangles.at(2, 0) == 2869);
    assert( a.triangles.at(2, 1) == 2877);
    assert( a.triangles.at(2, 2) == 2921);

    std::cout << " Pass" << std::endl;

    // TODO: Load big boi model

    std::cout << "Testing .obj big model loader. 8.5 million vertices in ";
    auto start = high_resolution_clock::now();

    Model b = loadModel("../models/demo/bow.obj");

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << " ms" << endl << std::endl;

    return 0;
}
