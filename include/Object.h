
#include <Types.h>
#include <Vector.h>
#include <Matrix.h>
#include <Coordinates.h>

#ifndef OBJECT_BASE_CLASS
#define OBJECT_BASE_CLASS

namespace MEGA 
{


// keep vertex under 20 bytes
// The vertex structure needs to be sequential. The struct format has this property
struct Vertex {
    //Vector<F32C1> position;	
    float position[3];
	//Vector<F32C1> normal;
	//Vector<F32C1> texture_coords;
	//uint8_t specular = 0; 								// 0 - 255
	//uint32_t ID{};
};

struct Triangle {
    float vertex_normals[3][3];
    float s[3], t[3];
    int vertex_idx[3];
    // maybe store a pointer instead of index
};

struct Mesh {
    int material_index;
    int num_triangles;
    int* triangle_indices;
};

struct Material {
    float ambient[4], diffuse[4], specular[4], emissive[4];
    float shininess;
    uint16_t texture;
    char* path;
};

struct Model {
    Matrix<F32C1> vertices;
    Matrix<U32C1> triangles;
    Matrix<F32C1> normals;
    std::vector<Mesh> meshes;
    uint64_t vertnum;
    uint64_t polynum;
};


// Object Base Class
class Object {

	protected:
		Model model;
		Material material;
		Vector<F32C1> coords;
		Quaternion direction;

	public:
		Object();
		Object( const Object& obj );
		Object( Model model, Material material, Vector<F32C1>& coordinates, Quaternion direction);
		~Object();

		virtual void render();
		virtual void move(Matrix<F64C1>& Rt);
		virtual void setStatic( bool set) const;

        // OpenGL functions
        void Draw();

		bool changedSinceLastFrame = false;
		mutable bool Static = false;

	private:

        // OpenGL functions
        unsigned int VAO, VBO, EBO;
        void setupMesh();

		const uint16_t world_id;
		static uint32_t next_id;
};



}
#endif
