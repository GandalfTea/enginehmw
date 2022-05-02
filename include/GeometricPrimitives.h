

#ifndef MEGA_GEOMETRIC_PRIMITIVES
#define MEGA_GEOMETRIC_PRIMITIVES

#include <cmath>
#include <Object.h>

namespace MEGA {





Model Plane( uint32_t width = 1,
             uint32_t height = 1,
             uint32_t stepW = 40,
             uint32_t stepH = 40 ) 
{
    Model ret;
    float invW = 1.f / stepW;
    float invH = 1.f / stepH;

    std::vector<float> vertices_buffer(3*(stepH+1)*(stepW+1));

    for( size_t j = 0; j <= stepH; j++) {
        for( size_t i = 0; i <= stepW; i++) {
            vertices_buffer.push_back( width * (i * invW - 0.5));
            vertices_buffer.push_back( 0 );
            vertices_buffer.push_back( height * (j * invH - 0.5));
        }
    }

    Matrix<F32C1> vert_buffer(3, vertices_buffer.size()/3, vertices_buffer);
    ret.vertices = vert_buffer;


    std::vector<uint32_t> quads_buffer;
    try {
        for(size_t j = 0, k = 0; j < stepH; ++j) {
            for( size_t i = 0; i < stepW; ++i) {
                quads_buffer.push_back( j * (stepW) + i + 1);
                quads_buffer.push_back( j * (stepW) + i);
                quads_buffer.push_back( (j+1) * (stepW+1) + i + 1);
                quads_buffer.push_back( (j+1) * (stepW+1) + i);
                k += 4;
            }
        }
        Matrix<U32C1> q_buffer(4, stepW*stepH, quads_buffer);
        ret.triangles = q_buffer;  
    }catch( MatrixException exception) {
        std::cout << exception << std::endl;
    }
    return ret;
}


Model Cone( uint32_t resolution,
            uint32_t radius,
            uint32_t height );
{}



} // namespace
#endif
