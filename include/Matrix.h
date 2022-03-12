

#include <vector>

#ifndef MATRIX
#define MATRIX

namespace MEGA 
{

typedef int Scalar;

// maybe use typedef ? 
template <uint8_t MAT_8UC1>
template <uint16_t MAT_16UC1>
template <uint32_t MAT_32UC1>
template <int8_t MAT_8IC1>
template <int16_t MAT_16IC1>
template <int32_t MAT_32IC1>
template <float MAT_32FC1>
template <double MAT_64DC1>

typedef enum {
	MAT_8UC1,
	MAT_16UC1,
	MAT_32UC1,
	MAT_8IC1,
	MAT_16IC1,
	MAT_32IC1,
	MAT_32FC1,
	MAT_64DC1,
	IDENTITY
} DataType;


class Matrix {
	public:
		Matrix();
		Matrix( Matrix& a );
		Matrix( DataType type = MAT_8UC1, size_t rows, size_t cols, std::vector& vals );
		Matrix( DataType type = MAT_8UC1, size_t rows, size_t cols, int fill = 0 );
		Matrix( DataType type = IDENTITY, size_t size );
		~Matrix();

		std::vector<std::vector<DataType>> data;
		int cols;
		int rows;
		uint32_t step = 0x00;
		mutable DataType type;

		// Helpers
		static Scalar det( Matrix& src );
		static Matrix inverse( Matrix& src );
		operator std::string() const;

		// Arithmatic
		Matrix& operator=( Matrix& src );
		Matrix& operator+( Matrix& lhs, Matrix& rhs );
		Matrix& operator-( Matrix& lhs, Matrix& rhs );

		Matrix& operator*( Matrix& lhs, Matrix& rhs );
		Matrix& operator*( Scalar& lhs, Matrix& rhs );
		Matrix& operator*( Matrix& lhs, Scalar& rhs );
		Matrix& operator*( Vector4& lhs, Matrix& rhs );
		Matrix& operator*( Matrix& lhs, Vector4& rhs );

		Matrix& operator/( Matrix& lhs, Matrix& rhs );
		Matrix& operator/( Scalar& lhs, Matrix& rhs );
		Matrix& operator/( Matrix& lhs, Scalar& rhs );
		Matrix& operator/( Vector4& lhs, Matrix& rhs );
		Matrix& operator/( Matrix& lhs, Vector4& rhs );

		Matrix& operator==( Matrix& lhs, Matrix& rhs );
		Matrix& operator!=( Matrix& lhs, Matrix& rhs );

	private:
		void set_step(DataType& type);
		Matrix& compare_types( Matrix& lhs, Matrix& rhs );
};


}
#endif
