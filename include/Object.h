
#include "Vector4.h"
#include "Matrix.h";

#ifndef OBJECT_BASE_CLASS
#define OBJECT_BASE_CLASS

namespace MEGA 
{

// Primitive encalulation
// keep vertex under 20 bytes
struct Vertex {
	float x, y, z;
	std::vector<uint8_t> color(0, 0, 0);	// 0 - 255
	Vector4 normal(0,0,0,0);
	uint8_t specular = 0; 								// 0 - 255
}

struct Model {
	std::vector<Vertex> verticies{};
	std::vector<unsigned int> IDs{};
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
		void setStatic( bool set) const;

		bool changedSinceLastFrame = false;
		mutable bool Static = false;

	private:
		const uint16_t world_id;
		static uint32_t next_id;
};

}
#endif
