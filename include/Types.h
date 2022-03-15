#include <vector>


#ifndef ENGINE_GLOBAL_TYPES
#define ENGINE_GLOBAL_TYPES

namespace MEGA
{

typedef int Scalar;
typedef unsigned char uchar;

// keep vertex under 20 bytes
struct Vertex {
	float x{}, y{}, z{};
	std::vector<uint8_t> color(0, 0, 0);	// 0 - 255
	Vector4 normal(0,0,0,0);
	uint8_t specular = 0; 								// 0 - 255
	uint32_t ID{};
}

struct Model {
	std::vector<Vertex> verticies{};
	std::vector<unsigned int> IDs{};
}

}
#endif
