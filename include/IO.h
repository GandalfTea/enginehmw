
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

typedef enum {
    LOAD_FAILED,
    LOAD_COMPLETED,
    LOAD_INTERRUPTED,
    LOAD_INITIALIZING,
    LOAD_MATRIX_EXCEPTION,
    LOADING
} LoadStatus;

bool loadModel( const char* path , Model& ret, LoadStatus& status=LOAD_INITIALIZING ) {
    status = LOAD_INITIALIZING;
    std::ifstream load_model( path, std::ios::in );
    if( load_model.fail() ) {
        status = LOAD_FAILED;
        return false;
    }
        
    // buffers
    std::vector<F32C1> vert_buffer{};
    std::vector<U32C1> face_buffer{};

    std::string type;
    while( load_model>>type ) {
    
        status = LOADING;

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

            // If the .obj file has more handlers that are not yet supported
            default: {
                status = LOAD_INTERRUPTED;
                return false;
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

    try {
        Matrix<F32C1> verts(3,  vert_buffer.size()/3, vert_buffer);
        Matrix<U32C1> faces(3,  face_buffer.size()/3, face_buffer);
    catch( MatrixException exception ) {
        status = LOAD_MATRIX_EXCEPTION;
        return false;
    }

    ret.vertices = verts;
    ret.triangles = faces;

    status = LOAD_COMPLETED;
    return true;
}


/*
Create a new world object.
This automatically loads the model, applies the translation, scaling and 
the rotation matricesand caches the material in order to render it.             */

// adding materials will be a separate Object member function

#ifndef DEFAULT_COORDS
    #define DEFAULT_COORDS Vector<F32C1>(3);
#endif

#ifndef DEFAULT_DIRECTION
    #define DEFAULT_DIRECTION Quanternion(0.0, 0.0, 0.0, 0.0);
#endif



void checkStatus( const LoadStatus& status ) {
   LoadStatus buffer = status;
    while( status != LOAD_COMPLETE && status != LOAD_FAILED  && status != LOAD_MATRIX_EXCEPTION) {
       if( buffer != status ) {
           std::cout << "Loading model status : ";
           if( status == LOAD_INITIALIZING ) std::cout << "Initializing" << std::endl; 
           if( status == LOADING ) std::cout << "Loading..." << std::endl; 
           buffer = status;
        }
    }
    if( status == LOAD_COMPLETE ) std::cout << "\nFinished." << std::endl;
    if( status == LOAD_FAILED ) std::cout << "\nFailed." << std::endl;
    if( status == LOAD_MATRIX_EXCEPTION ) std::cout << "\nFailed. Matrix Exception" << std::endl;
    if( status == INTERRUPTED ) std::cout << "\nLoading was interrupted. The current .obj parser does not support some features of the requested file." << std::endl;
}



Object createObject( const char* model_path, Vector<F32C1> coords = DEFAULT_COORDS, 
                     Quaternion direction = DEFAULT_DIRECTION, bool Static = true ) 
{
    // TODO: one model had more meshes 
    Object ret;
    Model model_buffer;
    LoadStatus status;

    // have a separate thread that checks the status of the loading
    std::thread loader( loadModel( model_path, model_buffer, status ));
    std::thread status( checkStatus( status ));

    status.join();
    loader.join();

    ret.model = model_buffer;
    ret.coordinates = coords;
    ret.direction = direction;
    if(Static) ret.setStatic(true);
    return ret;    
}



} // namespace
#endif
