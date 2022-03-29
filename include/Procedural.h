
#ifndef PROCEDURAL_GENERATION_CORE
#define PROCEDURAL_GENERATION_CORE

#include <Matrix.h>
#include <Vector.h>
#include <Types.h>

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



// Perline Noise
class PerlinNoise {

    public:

        static const unsigned table_size = 512;
        static const unsigned table_mask = table_size-1;
        unsigned permutation_table[ table_size*2 ];
        Vector<F32C1> gradients[ table_size ];

        PerlinNoise( unsigned seed = 2022) {

           
            std::mt19937 generator(seed);                       // generatres preudo-random number
            std::uniform_real_distribution distribution;        // this function takes as argument a number and returns a random number

            auto dice = std::bind(distribution, generator);     // this binds the call of distribution to another call to generator. The value from generator
                                                                // is then passed into the distribution function and that returns the random value we will use

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
            
            std::uniform_int_distribution distribution_int;
            auto int_dice = std::bind(distribution_int, generator);

            // Create the permutation table
            for( size_t i = 0; i < table_size; i++) {
                std::swap(permutation_table[i], permutation_table[int_dice() & table_mask]);
            }
            // Extend the permutation table.
            for( size_t i = 0; i < table_size; i++) {
                permutation_table[ table_size + i] = permutation_table[i];
            }
        }

        int hash( const int& x, const int& y) const {
            return this->permutation_table[this->permutation_table[x] + y];
        }

        float eval( const float x, const float y ) const {
            int x0 = ((int)std::floor(x)) & table_mask;
            int y0 = ((int)std::floor(y)) & table_mask;

            int x1 = (x0 + 1) & table_mask;
            int y1 = (y0 + 1) & table_mask;

            float tx = x - ((int)std::floor(x));
            float ty = x - ((int)std::floor(y));

            //float u = smoothstep(tx);
            //float v = smoothstep(ty);

            float u = tx;
            float v = ty;

            const Vector<F32C1>& c00 = this->gradients[hash(x0, y0)];
            const Vector<F32C1>& c10 = this->gradients[hash(x1, y0)];
            const Vector<F32C1>& c01 = this->gradients[hash(x0, y1)];
            const Vector<F32C1>& c11 = this->gradients[hash(x1, y1)];

            float X0 = tx, X1 = tx - 1;
            float Y0 = ty, Y1 = ty - 1;

            // Vectors going from the grid point to p
            std::vector<F32C1> vp00 = { X0, Y0 }; 
            std::vector<F32C1> vp10 = { X1, Y0 }; 
            std::vector<F32C1> vp01 = { X0, Y1 }; 
            std::vector<F32C1> vp11 = { X1, Y1 }; 

            Vector<F32C1> p00 = Vector<F32C1>(2, vp00);
            Vector<F32C1> p01 = Vector<F32C1>(2, vp01);
            Vector<F32C1> p10 = Vector<F32C1>(2, vp10);
            Vector<F32C1> p11 = Vector<F32C1>(2, vp11);

            // linear interpolation
            float a = lerp<F32C1>( p00.dot(c00), p10.dot(c10), u);
            float b = lerp<F32C1>( p01.dot(c01), p11.dot(c11), u);
            
            return lerp(a, b, v);
        }
};

// Map generation


}
#endif
