
#include <Procedural.h>

int main() {

    try {
        MEGA::PerlinNoise a;
        for( size_t i = 0; i < 512; i++){
            for( size_t j = 0; j < 512; j++) {
                float x = i / float(512-1) * 10;
                float y = j / float(512-1) * 10;
                std::cout << a.eval(x, y) << " , ";
            }
        }
    } catch( int Error ) {
        std::cout << Error << std::endl;
    }
    return 0;
}
