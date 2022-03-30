
#include "Types.h"
#include "Vector.h"
#include "Matrix.h";

#ifndef OBJECT_BASE_CLASS
#define OBJECT_BASE_CLASS

namespace MEGA 
{


// keep vertex under 20 bytes
// The vertex structure needs to be sequential. The struct format has this property
struct Vertex {
    Vector<F32C1> position;	
	Vector<F32C1> normal;
	Vector<F32C1> texture_coords;
	uint8_t specular = 0; 								// 0 - 255
	uint32_t ID{};
};

struct Model {
    Matrix<F32C1> vertices;
    Matrix<F32C1> normals;
	Matrix<U16C1> verticesIDs{};
	Matrix<U16C1> normalsIDs{};
};


// Object Base Class
class Object {

	protected:
		Model Model;
		Material Material;
		Vector<F32C1> Coordinates;
		//EulerAngles Direction;

	public:
		virtual Object();
		virtual Object( const Object& obj );
		virtual Object( Model model, Material material, Vector& coordinates);
		virtual ~Object();

		virtual void render();
		virtual void move(Matrix& Rt);
		virtual void setStatic( bool set) const;

        // OpenGL functions
        void Draw( Shader &shader );

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
