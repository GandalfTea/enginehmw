

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
             uint32_t stepH  = 40,
             bool bCollision = false ) 
{
    Model ret;
    ret.step = stepW;
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

    // Compute Collision
    if( bCollision ) {
        for(size_t j = 0; j < stepH; j += 8) {
            for( size_t i = 0; i < stepW; i += 8) {
                ret.collision.push_back( ret.vertices[j * (stepW+1) + i]);
                ret.collision.push_back( ret.vertices[j * (stepW+1) + i + 8]);
                ret.collision.push_back( ret.vertices[(j+8) * (stepW+1) + i + 8]);
                ret.collision.push_back( ret.vertices[(j+8) * (stepW+1) + i]);
            }
        }
    }
    return ret;
}

#define PI 3.14

/* Build a Sphere Primitive of size and resolution */
Model Sphere( uint32_t radius = 1.0f,
              uint32_t sector_resolution = 36,
              uint32_t stack_resolution = 18,
              bool collision = false)
{
    Model ret;
    float x, y, z, xy;  //position, xy is r*cos(u), to not recompute it every time
    float nx, ny, nz;   // normal
    
    float sector_step = 2*PI / sector_resolution;
    float stack_step = PI / stack_resolution;
    float sector_angle, stack_angle;
    float inv_len = 1.0f / radius;

    // vertices and normals
    for( size_t i{}; i <= stack_resolution; ++i ) {
        stack_angle = PI / 2 - i * stack_step;
        xy = radius * cos(stack_angle);
        z = radius * sin(stack_angle);
            
        for( size_t j{}; j <= sector_resolution; ++j ) {
            sector_angle = j * sector_step;

            x = xy * cos( sector_angle );
            y = xy * sin( sector_angle );
            Vertex a;
            a.position[0] = x;
            a.position[1] = y;
            a.position[2] = z;
            ret.vertices.push_back(a);

            Normal b;
            b.x = x * inv_len;
            b.y = y * inv_len;
            b.z = z * inv_len;
            ret.normals.push_back(b);
        } 
    }

    // quads
    int k1, k2;
    for( size_t i{}; i < sector_resolution; ++i ) {
        k1 = i * (sector_resolution + 1);
        k2 = k1 + sector_resolution + 1;

        for( size_t j{}; j < sector_resolution; ++j, ++k1, ++k2 ) {
            if( i != 0 ) {
                ret.triangles.push_back(k1);
                ret.triangles.push_back(k2);
                ret.triangles.push_back(k1+1);
            }
            if( i != (stack_resolution-1) ) {
                ret.triangles.push_back(k1+1);
                ret.triangles.push_back(k2);
                ret.triangles.push_back(k2+1);
            }
        }
    }
    return ret;
}


Model Cube( uint32_t width,
            uint32_t height,
            uint32_t steps,
            bool collision ) 
{
 
   
}


Model Cone( uint32_t resolution,
            uint32_t radius,
            uint32_t height );



} // namespace
#endif
