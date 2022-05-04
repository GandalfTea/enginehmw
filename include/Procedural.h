
#ifndef PROCEDURAL_GENERATION_CORE
#define PROCEDURAL_GENERATION_CORE

#include <Matrix.h>
#include <Vector.h>
#include <Types.h>
#include <Object.h>
#include <GeometricPrimitives.h>

#include <random>
#include <functional>

namespace MEGA {

// Linear Interpolation

template <typename T = float>
inline T lerp ( const T& a, const T& b, const T& t) {
    //return a + t * (b - a);
    return a * ( 1 - t ) + b * t;     
}


// Fade Function
// TODO



typedef enum {
    MEGA_SILENT,
    MEGA_DEBUG
} RunOption;


// Perline Noise
class PerlinNoise {

    public:

        static const unsigned table_size = 512;
        static const unsigned table_mask = table_size-1;
        unsigned permutation_table[ table_size*2 ];
        Vector<F32C1> gradients[ table_size ];

        PerlinNoise( unsigned seed = 2022, RunOption option = MEGA_SILENT) {

           
            std::mt19937 generator(seed);                       // generatres preudo-random number
            std::uniform_real_distribution distribution;        // this function takes as argument a number and returns a random number

            auto dice = std::bind(distribution, generator);     // this binds the call of distribution to another call to generator. The value from generator
                                                                // is then passed into the distribution function and that returns the random value we will use

            if (option == MEGA_DEBUG) {
                std::cout << "\nInitialized mt19937 and real number distribution.\n" << std::endl;
                for (size_t i{}; i <= 5; i++) {
                    float a = dice();
                    std::cout << a << " -> " << 2*a-1 << std::endl;
                }
            }

            for( size_t i{}; i < table_size; i++) {
                // create vector from 2 random values of ( 2*dice()-1 )
                std::vector<F32C1> vals = { 2*dice()-1, 2*dice()-1 };
                //for (auto i : vals) std::cout << i << std::endl;
                Vector<F32C1> a (2, vals);
                // normalize the gradient by dividing it with the length of the vector
                for( auto i : a.data) {
                    i /= a.L;
                }

                gradients[i] = a;
                permutation_table[i] = i;
            }

            if (option == MEGA_DEBUG) {
                std::cout << "\nGenerated gradients and permutation table." << std::endl;
                for (size_t i{}; i <= 10; i++) {
                    std::cout << gradients[i];
                }
            }
            
            std::uniform_int_distribution distribution_int;
            auto int_dice = std::bind(distribution_int, generator);

            if (option == MEGA_DEBUG) {
                std::cout << "\nInt Dice initialized.\n" << std::endl;
                for (size_t i{}; i <= 5; i++) {
                    int a = int_dice();
                    std::cout << a << " -> " << (a & table_mask) << std::endl;
                }
            }

            // Create the permutation table
            for( size_t i = 0; i < table_size; i++) {
                std::swap(permutation_table[i], permutation_table[int_dice() & table_mask]);
            }
            // Extend the permutation table.
            for( size_t i = 0; i < table_size; i++) {
                permutation_table[ table_size + i] = permutation_table[i];
            }

            if (option == MEGA_DEBUG) {
                std::cout << "\nGenerated Full Permutation Table." << std::endl;
            }

        }

        int hash( const int& x, const int& y) const {
            return this->permutation_table[this->permutation_table[x] + y];
        }

        float eval( const float x, const float y , RunOption option = MEGA_SILENT) const {

            int x0 = ((int)std::floor(x)) & table_mask;
            int y0 = ((int)std::floor(y)) & table_mask;

            if (option == MEGA_DEBUG) {
                std::cout << "\nx0 and y0 init.\n" << std::endl;
                std::cout << x << " -> " << x0 << std::endl;
                std::cout << y << " -> " << y0 << std::endl;
            }

            int x1 = (x0 + 1) & table_mask;
            int y1 = (y0 + 1) & table_mask;

            if (option == MEGA_DEBUG) {
                std::cout << "\nx1 and y1 init.\n" << std::endl;
                std::cout << x0 << " -> " << x1 << std::endl;
                std::cout << y0 << " -> " << y1 << std::endl;
            }

            float tx = x - ((int)std::floor(x));
            float ty = y - ((int)std::floor(y));

            if (option == MEGA_DEBUG) {
                std::cout << "\nu and v init.\n" << std::endl;
                std::cout << x << " -> " << tx << std::endl;
                std::cout << y << " -> " << ty << std::endl;
            }

            //float u = fade(tx);
            //float v = fade(ty);

            float u = tx;
            float v = ty;

            const Vector<F32C1>& c00 = this->gradients[hash(x0, y0)];
            const Vector<F32C1>& c10 = this->gradients[hash(x1, y0)];
            const Vector<F32C1>& c01 = this->gradients[hash(x0, y1)];
            const Vector<F32C1>& c11 = this->gradients[hash(x1, y1)];

            if (option == MEGA_DEBUG) {
                std::cout << "\nCorner Vectors Initialized.\n" << std::endl;
                std::cout << c00;
                std::cout << c10;
                std::cout << c01;
                std::cout << c11;
            }

            float X0 = tx, X1 = tx - 1;
            float Y0 = ty, Y1 = ty - 1;

            if (option == MEGA_DEBUG) {
                std::cout << "\nX0, X1 and Y0, Y1 init.\n" << std::endl;
                std::cout << X0 << "    " << X1 << std::endl;
                std::cout << Y0 << "    " << Y1 <<  std::endl;
            }

            // Vectors going from the grid point to p
            std::vector<F32C1> vp00 = { X0, Y0 }; 
            std::vector<F32C1> vp10 = { X1, Y0 }; 
            std::vector<F32C1> vp01 = { X0, Y1 }; 
            std::vector<F32C1> vp11 = { X1, Y1 }; 

            Vector<F32C1> p00 = Vector<F32C1>(2, vp00);
            Vector<F32C1> p01 = Vector<F32C1>(2, vp01);
            Vector<F32C1> p10 = Vector<F32C1>(2, vp10);
            Vector<F32C1> p11 = Vector<F32C1>(2, vp11);

            if (option == MEGA_DEBUG) {
                std::cout << "\nVectors going from the grid to the point p.\n" << std::endl;
                std::cout << p00;
                std::cout << p10;
                std::cout << p01;
                std::cout << p11;
            }


            // linear interpolation
            float a = lerp<F32C1>( p00.dot(c00), p10.dot(c10), u);
            float b = lerp<F32C1>( p01.dot(c01), p11.dot(c11), u);
            float c = lerp<F32C1>( p00.dot(c00), p01.dot(c01), u);
            float d = lerp<F32C1>( p10.dot(c10), p11.dot(c11), u);

            if (option == MEGA_DEBUG) {
                std::cout << "\nInit a and b.\n" << std::endl;
                std::cout << a << std::endl;
                std::cout << b <<  std::endl;
                std::cout << c <<  std::endl;
                std::cout << d <<  std::endl;
            }

            float e = lerp(a, b, v);
            float f = lerp(c, d, v);
            
            return lerp(e, f, v);
        }
};





// Map generation
// TODO: Derrive from main Object Class after testing
class ProceduralTerrain {

    public:
        Model terrain;

    public:
        ProceduralTerrain() = delete;
        ~ProceduralTerrain() {}

        ProceduralTerrain( unsigned seed, size_t width = 1, size_t length = 1, size_t resolution = 40 ) {

            // create poly mesh
            terrain = Plane(width, length, resolution, resolution);
            noise = new PerlinNoise(seed);

            // create height map
            for(size_t i{}; i <= resolution; i++) {
                for(size_t j{}; j <= resolution; j++) {
                    float x = i / (double)resolution-1;  
                    float y = j / (double)resolution-1;  
                    heightmap.push_back(noise->eval(x, y));
                }
            }

            // apply height map     
            for(size_t i{}; i < terrain.vertices.size(); i++) {
                terrain.vertices[i].position[1] = heightmap[i];
                std::cout << terrain.vertices[i].position[1] << std::endl;
            }

        }


    private:
        PerlinNoise* noise;
        std::vector<double> heightmap;
};


}
#endif
