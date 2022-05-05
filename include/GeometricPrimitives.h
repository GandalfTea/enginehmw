

#ifndef MEGA_GEOMETRIC_PRIMITIVES
#define MEGA_GEOMETRIC_PRIMITIVES

#include <cmath>
#include <Object.h>

namespace MEGA {


Vector<F32C1> computeNormal( Vector<F32C1>& a, Vector<F32C1>& b, Vector<F32C1>& c ) {
    Vector<F32C1> norm = (a - b) % (b - c);    
    norm.normalize();
    return norm;
}

// Temporary before using the Vector class
Normal computeNormal( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    float nX = (y2-y1)*(z3-z1) - (z2-z1)*(y3-y1);
    float nY = (z2-z1)*(x3-x1) - (x2-x1)*(z3-z1);
    float nZ = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
    float L  = sqrt( nX*nX + nY*nY + nZ*nZ ); 
    Normal normal;
    normal.x = -nX / L;
    normal.y = -nY / L;
    normal.z = -nZ / L;
    return normal;
}

// Non-Planar quad, for the procedurally generated terrain 
Normal computeNormal( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4) {
    float nX1 = (y2-y1)*(z3-z1) - (z2-z1)*(y3-y1);
    float nY1 = (z2-z1)*(x3-x1) - (x2-x1)*(z3-z1);
    float nZ1 = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
    float L1  = sqrt( nX1*nX1 + nY1*nY1 + nZ1*nZ1 ); 
    float ret[3];
    ret[0] = nX1 / L1;
    ret[1] = nY1 / L1;
    ret[2] = nZ1 / L1;

    float nX2 = (y3-y2)*(z4-z2) - (z3-z2)*(y4-y2);
    float nY2 = (z3-z2)*(x4-x2) - (x3-x2)*(z4-z2);
    float nZ2 = (x3-x2)*(y4-y2) - (y3-y2)*(x4-x2);
    float L2  = sqrt( nX2*nX2 + nY2*nY2 + nZ2*nZ2 ); 

    Normal normal;
    normal.x = -(ret[0] + (nX2 / L2))/2;
    normal.y = -(ret[1] + (nY2 / L2))/2;
    normal.z = -(ret[2] + (nZ2 / L2))/2;

    //std::cout << ret[0] << " " << ret[1] << " " << ret[2] << std::endl;
    return normal;
}


Model Plane( uint32_t width  = 1,
             uint32_t height = 1,
             uint32_t stepW  = 40,
             uint32_t stepH  = 40 ) 
{
    Model ret;
    float invW = 1.f / stepW;
    float invH = 1.f / stepH;

    // Compute Vertices
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

    // Compute Quads
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

    // Compute Normals

    for( size_t i{}; i < ret.triangles.size(); i += 4) {
        Vertex V1 = ret.vertices[ ret.triangles[i] ];
        Vertex V2 = ret.vertices[ ret.triangles[i+1] ];
        Vertex V3 = ret.vertices[ ret.triangles[i+2] ];

        Normal normal = { computeNormal( V1.position[0], V1.position[1], V1.position[2],
                                          V2.position[0], V2.position[1], V2.position[2],
                                          V3.position[0], V3.position[1], V3.position[2] )};
        ret.normals.push_back(normal);
    }
    return ret;
}


Model Cone( uint32_t resolution,
            uint32_t radius,
            uint32_t height );



} // namespace
#endif
