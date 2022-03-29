
#include <Procedural.h>

int main() {

    try {
        MEGA::PerlinNoise a;
        double result = a.eval(2.2456874, 1.5489);
        for( size_t i=0; i <=  512; i++) {
            for( size_t j=0; j <=  512; j++) {
                float x = i / float(512-1) * 10;
                float y = j / float(512-1) * 10;
                double result = a.eval(x, y);
                std::cout << result << " ,  ";
            }
        }
    } catch( int Error ) {
        std::cout << Error << std::endl;
    }
    return 0;
}
