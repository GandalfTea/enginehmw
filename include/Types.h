#include <vector>
#include <cstdint>

#ifndef ENGINE_GLOBAL_TYPES
#define ENGINE_GLOBAL_TYPES

namespace MEGA
{

/*

MATRIX TYPE FORMAT: (Type) + (Depth) + (Number of channels)

*/

enum MatrixType {
    MAT_U8C1, MAT_U8C2, MAT_U8C3,
    MAT_S8C1, MAT_S8C2, MAT_S8C3,
    MAT_U16C1, MAT_U16C2, MAT_U16C3,
    MAT_S16C1, MAT_S16C2, MAT_S16C3,
    MAT_U32C1, MAT_U32C2, MAT_U32C3,
    MAT_S32C1, MAT_S32C2, MAT_S32C3,
    MAT_F32C1, MAT_F32C2, MAT_F32C3,
    MAT_F64C1, MAT_F64C2, MAT_F64C3
};

using U8C1 = uint8_t;
using U8C2 = uint8_t[2];
using U8C3 = uint8_t[3];
using S8C1 = int8_t;
using S8C2 = int8_t[2];
using S8C3 = int8_t[3];

using U16C1 = uint16_t;
using U16C2 = uint16_t[2];
using U16C3 = uint16_t[3];
using S16C1 = int16_t;
using S16C2 = int16_t[2];
using S16C3 = int16_t[3];

using U32C1 = uint32_t;
using U32C2 = uint32_t[2];
using U32C3 = uint32_t[3];
using S32C1 = int32_t;
using S32C2 = int32_t[2];
using S32C3 = int32_t[3];
using F32C1 = float;
using F32C2 = float[2];
using F32C3 = float[3];

using F64C1 = double;
using F64C2 = double[2];
using F64C3 = double[3];





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
