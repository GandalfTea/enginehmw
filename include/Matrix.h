
#include "Types.h"
#include <vector>
#include <cstdint>
#include <stdio.h>
#include <Map>
#include <Any>

#ifndef MATRIX
#define MATRIX

namespace MEGA 
{

// Matrix type
#define MAT_8U  0 
#define MAT_16U 1
#define MAT_32U 2
#define MAT_8I  3
#define MAT_16I 4
#define MAT_32I 5
#define MAT_32F 6
#define MAT_64D 7

struct allocator {
	std::vector<std::vector<std::any>> matrix;
	std::vector<std::any> row;
	std::string typeId;
};

class MatrixException {
	public:
		typedef enum {
			INPUT_VALUES_WRONG_TYPE,
			INPUT_TOO_MANY_VALUES,
			INPUT_NOT_ENOUGH_VALUES,

		} Error;
		Error error_;
		explicit MatrixException(Error error) : error_(error) {}
};


class Matrix {
	public:
		Matrix();
		Matrix( Matrix& a );
		Matrix( int type = MAT_8U, size_t rows = 2, size_t cols = 2, std::vector<std::any>& vals );
		Matrix( int type = MAT_8U, size_t rows = 2, size_t cols = 2, int fill = 0 );
		//Matrix( int type = IDENTITY, size_t size = 2 );
		~Matrix();

		std::vector<std::vector<std::any>> data;
		unsigned int cols;
		unsigned int rows;
		uchar step = 0x00;
		mutable uint8_t type;

		// Helpers
		static Scalar det( Matrix& src );
		static Matrix inverse( Matrix& src );
		operator std::string() const;
	
		// Arithmatic
		Matrix& operator=( Matrix& src );
		Matrix& operator+( Matrix& rhs );
		Matrix& operator-( Matrix& rhs );

		Matrix& operator*( Matrix& rhs );
		Matrix& operator*( Scalar& rhs );
		Matrix& operator*( Vector4& rhs );

		Matrix& operator/( Matrix& rhs );
		Matrix& operator/( Scalar& rhs );
		Matrix& operator/( Vector4& rhs );

		Matrix& operator==( Matrix& rhs );
		Matrix& operator!=( Matrix& rhs );

		// Static
		static Matrix& eye(size_t size, int type = MAT_8U);

	private:
		void set_step(int& type);
		Matrix& compare_types( Matrix& lhs, Matrix& rhs );
		allocator get_type(int type);
};




}
#endif
