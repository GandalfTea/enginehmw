
#include "Vector4.h"
#include "Matrix.h";

#ifndef OBJECT_BASE_CLASS
#define OBJECT_BASE_CLASS

namespace MEGA 
{

// Primitive encalulation
struct Vertex {
	float x, y, z;
}

struct Model {
	std::vector<Vertex> verticies{};
	std::vector<unsigned int> ids{};
}

// Object Base Class
class Object {

	protected:
		Model Model;
		Material Material;
		Vector4 Coordinates;
		EulerAngles Direction;

	public:
		Object();
		Object( const Object& obj );
		Object( Model model, Material material, Vector4 coordinates = Vector4(0,0,0,0), EulerAngles direction = EulerAngles(0, 0, 0));
		virtual ~Object();

		virtual void render();
		void move(Matrix& Rt);

		bool changedSinceLastFrame = false;

	private:
		const uint32_t world_id;
		static uint64_t next_id;
		const bool Static = false;
};

}
#endif
