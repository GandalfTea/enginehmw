
#include <iostream>
#include <Vector.h>
#include <vector>

using namespace MEGA;
using namespace std;

void test_vector();

int main() {
    test_vector();
    return 0;
}


void test_vector() {
    
    vector<float> vals { 0.01, 0.5642, 0.31345, 1.0 };
    Vector<F32C1> a (4, vals);

    cout << a << endl;

    auto b = a.T();
    cout << b << endl;
    
}
