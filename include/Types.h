#include <vector>
#include <cstdint>

#ifndef ENGINE_GLOBAL_TYPES
#define ENGINE_GLOBAL_TYPES

namespace MEGA
{

// Matrix type
#define MAT_8I  0
#define MAT_16I 1
#define MAT_32I 2
#define MAT_8U  3 
#define MAT_16U 4
#define MAT_32U 5
#define MAT_32F 6
#define MAT_64D 7

using MAT_8UC1 = uint8_t;
using MAT_8UC2 = uint8_t[2];
using MAT_8UC3 = uint8_t[3];
using MAT_8SC1 = int8_t;
using MAT_8SC2 = int8_t[2];
using MAT_8SC3 = int8_t[3];

using MAT_16UC1 = uint16_t;
using MAT_16UC2 = uint16_t[2];
using MAT_16UC3 = uint16_t[3];
using MAT_16SC1 = int16_t;
using MAT_16SC2 = int16_t[2];
using MAT_16SC3 = int16_t[3];

using MAT_32UC1 = uint32_t;
using MAT_32UC2 = uint32_t[2];
using MAT_32UC3 = uint32_t[3];
using MAT_32SC1 = int32_t;
using MAT_32SC2 = int32_t[2];
using MAT_32SC3 = int32_t[3];
using MAT_32FC1 = float;
using MAT_32FC2 = float[2];
using MAT_32FC3 = float[3];

using MAT_64FC1 = double;
using MAT_64FC2 = double[2];
using MAT_64FC3 = double[3];





typedef int Scalar;
typedef unsigned char uchar;

// keep vertex under 20 bytes
struct Vertex {
	float x{}, y{}, z{};
	std::vector<uint8_t> color{0, 0, 0};	// 0 - 255
	std::vector<uchar> normal{0,0,0,0};
	uint8_t specular = 0; 								// 0 - 255
	uint32_t ID{};
};

struct Model {
	std::vector<Vertex> verticies{};
	std::vector<unsigned int> IDs{};
};

}
#endif
