
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

inline float smoothstep( const float& t) {
    return t * t * (3 - 2 * t);
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
                for (size_t i{}; i <= 20; i++) {
                    float a = dice();
                    std::cout << a << " -> " << 2*a-1 << std::endl;
                }
            }

            for( size_t i{}; i < table_size; ++i) {
                // create vector from 2 random values of ( 2*dice()-1 )
                std::vector<F32C1> vals = { 2*dice()-1, 2*dice()-1, 2*dice()-1 };
                Vector<F32C1> a (3, vals);
                // normalize the gradient by dividing it with the length of the vector
                for( auto i : a.data) { i /= a.L; }

                gradients[i] = a;
                permutation_table[i] = i;
            }

            if (option == MEGA_DEBUG) {
                std::cout << "\nGenerated gradients and permutation table." << std::endl;
                for (size_t i{}; i <= 20; i++) {
                    std::cout << gradients[i];
                }
            }
            
            std::uniform_int_distribution distribution_int;
            auto int_dice = std::bind(distribution_int, generator);

            if (option == MEGA_DEBUG) {
                std::cout << "\nInt Dice initialized.\n" << std::endl;
                for (size_t i{}; i <= 20; i++) {
                    int a = int_dice();
                    std::cout << a << " -> " << (a & table_mask) << std::endl;
                }
            }

            // Create the permutation table
            for( size_t i = 0; i < table_size; ++i) {
                std::swap(permutation_table[i], permutation_table[int_dice() & table_mask]);
            }
            // Extend the permutation table.
            for( size_t i = 0; i < table_size; ++i) {
                permutation_table[ table_size + i] = permutation_table[i];
            }

            if (option == MEGA_DEBUG) {
                std::cout << "\nGenerated Full Permutation Table." << std::endl;
            }

        }

        int hash( const int& x, const int& y, const int &z) const {
            return this->permutation_table[this->permutation_table[this->permutation_table[x] + y]];
        }

        float eval( const float x, const float y, const float z, RunOption option = MEGA_SILENT) const {

            int x0 = ((int)std::floor(x)) & table_mask;
            int y0 = ((int)std::floor(y)) & table_mask;
            int z0 = ((int)std::floor(z)) & table_mask;

            if (option == MEGA_DEBUG) {
                std::cout << "\nx0 and y0 init.\n" << std::endl;
                std::cout << x << " -> " << x0 << std::endl;
                std::cout << y << " -> " << y0 << std::endl;
                std::cout << z << " -> " << z0 << std::endl;
            }

            int x1 = (x0 + 1) & table_mask;
            int y1 = (y0 + 1) & table_mask;
            int z1 = (z0 + 1) & table_mask;

            if (option == MEGA_DEBUG) {
                std::cout << "\nx1 and y1 init.\n" << std::endl;
                std::cout << x0 << " -> " << x1 << std::endl;
                std::cout << y0 << " -> " << y1 << std::endl;
                std::cout << z0 << " -> " << z1 << std::endl;
            }

            float tx = x - ((int)std::floor(x));
            float ty = y - ((int)std::floor(y));
            float tz = z - ((int)std::floor(z));

            if (option == MEGA_DEBUG) {
                std::cout << "\nu and v init.\n" << std::endl;
                std::cout << x << " -> " << tx << std::endl;
                std::cout << y << " -> " << ty << std::endl;
                std::cout << z << " -> " << tz << std::endl;
            }

            //float u = fade(tx);
            //float v = fade(ty);

            // TODO: Smoothstep
            float u = smoothstep(tx);
            float v = smoothstep(ty);
            float w = smoothstep(tz);

            const Vector<F32C1>& c000 = this->gradients[hash(x0, y0, z0)];
            const Vector<F32C1>& c100 = this->gradients[hash(x1, y0, z0)];
            const Vector<F32C1>& c010 = this->gradients[hash(x0, y1, z0)];
            const Vector<F32C1>& c110 = this->gradients[hash(x1, y1, z0)];

            const Vector<F32C1>& c001 = this->gradients[hash(x0, y0, z1)];
            const Vector<F32C1>& c101 = this->gradients[hash(x1, y0, z1)];
            const Vector<F32C1>& c011 = this->gradients[hash(x0, y1, z1)];
            const Vector<F32C1>& c111 = this->gradients[hash(x1, y1, z1)];

            if (option == MEGA_DEBUG) {
                std::cout << "\nCorner Vectors Initialized.\n" << std::endl;
                std::cout << c000;
                std::cout << c100;
                std::cout << c010;
                std::cout << c110;

                std::cout << c001;
                std::cout << c101;
                std::cout << c011;
                std::cout << c111;
            }

            float X0 = tx, X1 = tx - 1;
            float Y0 = ty, Y1 = ty - 1;
            float Z0 = tz, Z1 = tz - 1;

            if (option == MEGA_DEBUG) {
                std::cout << "\nX0, X1 and Y0, Y1 init.\n" << std::endl;
                std::cout << X0 << "    " << X1 << std::endl;
                std::cout << Y0 << "    " << Y1 <<  std::endl;
                std::cout << Z0 << "    " << Z1 <<  std::endl;
            }

            // Vectors going from the grid point to p
            std::vector<F32C1> vp000 = { X0, Y0, Z0 }; 
            std::vector<F32C1> vp100 = { X1, Y0, Z0 }; 
            std::vector<F32C1> vp010 = { X0, Y1, Z0 }; 
            std::vector<F32C1> vp110 = { X1, Y1, Z0 }; 

            std::vector<F32C1> vp001 = { X0, Y0, Z1 }; 
            std::vector<F32C1> vp101 = { X1, Y0, Z1 }; 
            std::vector<F32C1> vp011 = { X0, Y1, Z1 }; 
            std::vector<F32C1> vp111 = { X1, Y1, Z1 }; 

            Vector<F32C1> p000 = Vector<F32C1>(3, vp000);
            Vector<F32C1> p010 = Vector<F32C1>(3, vp010);
            Vector<F32C1> p100 = Vector<F32C1>(3, vp100);
            Vector<F32C1> p110 = Vector<F32C1>(3, vp110);

            Vector<F32C1> p001 = Vector<F32C1>(3, vp001);
            Vector<F32C1> p011 = Vector<F32C1>(3, vp011);
            Vector<F32C1> p101 = Vector<F32C1>(3, vp101);
            Vector<F32C1> p111 = Vector<F32C1>(3, vp111);

            if (option == MEGA_DEBUG) {
                std::cout << "\nVectors going from the grid to the point p.\n" << std::endl;
                std::cout << p000;
                std::cout << p100;
                std::cout << p010;
                std::cout << p110;

                std::cout << p001;
                std::cout << p101;
                std::cout << p011;
                std::cout << p111;
            }


            // linear interpolation
            float a = lerp<F32C1>( p000.dot(c000), p100.dot(c100), u);
            float b = lerp<F32C1>( p010.dot(c010), p110.dot(c110), u);
            float c = lerp<F32C1>( p001.dot(c001), p101.dot(c101), u);
            float d = lerp<F32C1>( p011.dot(c011), p111.dot(c111), u);

            if (option == MEGA_DEBUG) {
                std::cout << "\nInit a and b.\n" << std::endl;
                std::cout << a << std::endl;
                std::cout << b <<  std::endl;
                std::cout << c <<  std::endl;
                std::cout << d <<  std::endl;
            }

            float e = lerp(a, b, v);
            float f = lerp(c, d, v);
            
            return lerp(e, f, w);
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

        ProceduralTerrain( unsigned seed, size_t width = 1, size_t length = 1, size_t resolution = 40, float smoothness = 50.0f, bool spicy = false ) {

            // create poly mesh
            terrain = Plane(width, length, resolution, resolution);
            noise = new PerlinNoise(seed);

            // create height map
            size_t layers = 5;
            float max_value = 0;
            for(size_t i{}; i <= resolution; ++i) {
                for(size_t j{}; j <= resolution; ++j) {
                    float fractal = 0;
                    float amplitude = 1;
                    float x = i * 1/ smoothness; 
                    float y = j * 1/ smoothness; 
                    if( spicy ) {
                        for( size_t k{}; k < layers; ++k ) {
                            fractal += (1 + noise->eval(x, y, 0)) * 0.3 * amplitude;
                            x *= 2;
                            y*= 2;
                            amplitude *= 0.5;
                        }
                        if( fractal > max_value) max_value = fractal;
                        heightmap.push_back(fractal);
                    } else {
                        heightmap.push_back( (noise->eval(x, y, 0)+1) * 0.5);
                    }
                }
            }

            // apply height map     
            for(size_t i{}; i < terrain.vertices.size(); i++) {
                terrain.vertices[i].position[1] = heightmap[i];
            }

            // Recompute normals
            terrain.normals.clear();
            for( size_t i{}; i < terrain.triangles.size(); i += 4) {
                Vertex V1 = terrain.vertices[ terrain.triangles[i] ];
                Vertex V2 = terrain.vertices[ terrain.triangles[i+1] ];
                Vertex V3 = terrain.vertices[ terrain.triangles[i+2] ];
                Vertex V4 = terrain.vertices[ terrain.triangles[i+3] ];

                //Normal normal = { computeNormal( V1.position[0], V1.position[1], V1.position[2],
                //                                  V2.position[0], V2.position[1], V2.position[2],
                //                                  V3.position[0], V3.position[1], V3.position[2],
                //                                  V4.position[0], V4.position[1], V4.position[2])};
                
                terrain.normals.push_back( computeNormal(V4.position[0], V4.position[1], V4.position[2],
                                                         V1.position[0], V1.position[1], V1.position[2],
                                                         V2.position[0], V2.position[1], V2.position[2]));

                terrain.normals.push_back( computeNormal(V1.position[0], V1.position[1], V1.position[2],
                                                         V2.position[0], V2.position[1], V2.position[2],
                                                         V3.position[0], V3.position[1], V3.position[2]));

                terrain.normals.push_back( computeNormal(V2.position[0], V2.position[1], V2.position[2],
                                                         V3.position[0], V3.position[1], V3.position[2],
                                                         V4.position[0], V4.position[1], V4.position[2]));

                terrain.normals.push_back( computeNormal(V3.position[0], V3.position[1], V3.position[2],
                                                         V4.position[0], V4.position[1], V4.position[2],
                                                         V1.position[0], V1.position[1], V1.position[2]));

            }
        }

    private:
        PerlinNoise* noise;
        std::vector<double> heightmap;
};


}
#endif
