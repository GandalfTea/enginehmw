

#ifndef MEGA_GEOMETRIC_PRIMITIVES
#define MEGA_GEOMETRIC_PRIMITIVES

#include <cmath>
#include <Object.h>

namespace MEGA {





Model Plane( uint32_t width  = 1,
             uint32_t height = 1,
             uint32_t stepW  = 40,
             uint32_t stepH  = 40 ) 
{
    Model ret;
    float invW = 1.f / stepW;
    float invH = 1.f / stepH;

    std::vector<Vertex> vertices_buffer;
    for( size_t j = 0; j <= stepH; ++j) {
        for( size_t i = 0; i <= stepW; ++i) {
            Vertex a;
            a.position[0] = width * (i * invW - 0.5);
            a.position[1] = 0.0f;
            a.position[2] = height * (j * invH - 0.5);
            vertices_buffer.push_back( a );
        }
    }
    ret.vertices = vertices_buffer;
    ret.vertnum = ret.vertices.size();

    std::vector<uint32_t> quads_buffer;
    for(size_t j = 0; j < stepH; ++j) {
        for( size_t i = 0; i < stepW; ++i) {
            quads_buffer.push_back( j * (stepW+1) + i);
            quads_buffer.push_back( j * (stepW+1) + i + 1);
            quads_buffer.push_back( (j+1) * (stepW+1) + i + 1);
            quads_buffer.push_back( (j+1) * (stepW+1) + i);
        }
    }
    ret.triangles = quads_buffer;  
    return ret;
}


Model Cone( uint32_t resolution,
            uint32_t radius,
            uint32_t height );



} // namespace
#endif
