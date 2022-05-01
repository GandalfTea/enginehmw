
#ifndef MEGA_IO_CORE
#define MEGA_IO_CORE

#include <Object.h>
#include <fstream>
#include <string>

namespace MEGA {

/* 
For now, only .OBJ ASCII format files will be supported.
This is because of time constraints, in the future binary
file formats will make this more efficient.           */

Model loadModel( const char* path ) {
    std::ifstream load_model( path, std::ios::in );
    if( load_model.fail() ) {
        // throw error
    }
        
    // buffers
    std::vector<F32C1> vert_buffer{};
    std::vector<U32C1> face_buffer{};

    std::string type;
    while( load_model>>type ) {

        switch( *type.c_str() ) {
            case '#': {
                // skip comments
                std::getline(load_model, type);
                continue;
            }
            case 'v': {
                float x{}, y{}, z{};
                load_model >> x >> y >> z;
                vert_buffer.insert( vert_buffer.end(), { x, y, z });
                break;
            }
            case 'f': {
                uint32_t a{}, b{}, c{};
                load_model >>a >> b >> c;
                face_buffer.insert( face_buffer.end(), { a, b, c });
                break;
            }
        /*    case "vt": {
                // texture 
            }
            case "vn": {
                // normals
            }
            case 'l': {
                // line
            }*/   
        }
    }

    Model ret;
    Matrix<F32C1> verts(3,  vert_buffer.size()/3, vert_buffer);
    Matrix<U32C1> faces(3,  face_buffer.size()/3, face_buffer);

    ret.vertices = verts;
    ret.triangles = faces;

    return ret;
}



} // namespace
#endif
