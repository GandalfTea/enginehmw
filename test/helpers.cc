
#include <cmath>

bool compare_float( float x, float y, float epsilon = 0.01f) {
    if( fabs(x - y) < epsilon)
        return true;
    return false;
}
