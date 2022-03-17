
#ifndef MATRIX
#define MATRIX


#include "Types.h"
#include <vector>
#include <cstdint>
#include <stdio.h>
#include <string>
#include <Any>

namespace MEGA 
{

static std::vector<std::any> DEFAULT_VECTOR;

// Matrix type
#define MAT_8I  0
#define MAT_16I 1
#define MAT_32I 2
#define MAT_8U  3 
#define MAT_16U 4
#define MAT_32U 5
#define MAT_32F 6
#define MAT_64D 7

struct allocator {
	std::vector<std::vector<std::any>>* matrix;
	std::vector<std::any>* row;
	std::string typeId;
};

typedef enum {
	INPUT_VALUES_WRONG_TYPE,
	INPUT_TOO_MANY_VALUES,
	INPUT_NOT_ENOUGH_VALUES,
	EXTRACT_INDEX_TOO_BIG,
	EXTRACT_NEGATIVE_INDEX
} Error;

class MatrixException {
	public:
		Error error_;
		explicit MatrixException(Error error) : error_(error) {}
};


class Matrix {
	public:
		Matrix();
		Matrix( Matrix& a );
		Matrix( int type = MAT_8U, size_t rows = 2, size_t cols = 2, std::vector<std::any>& vals = DEFAULT_VECTOR );
		Matrix( int type = MAT_8U, size_t rows = 2, size_t cols = 2, std::any fill = 0 );
		~Matrix();

		std::vector<std::vector<std::any>> data;
		unsigned int cols;
		unsigned int rows;
		uchar step = 0x00;
		mutable uint8_t type;

		// Helpers
		Scalar at( size_t col, size_t row ) const;
		// TODO: CHANGE FROM FLOAT TO STD::ANY
		std::vector<float> row( size_t row ) const;
		std::vector<std::any> col( size_t col ) const;
	
		// Arithmatic
		Matrix& operator=( const Matrix& src );
		Matrix& operator+( Matrix& rhs );
		Matrix& operator-( Matrix& rhs );

		Matrix& operator*( Matrix& rhs );
		Matrix& operator*( Scalar& rhs );
		//Matrix& operator*( Vector4& rhs );

		Matrix& operator/( Matrix& rhs );
		Matrix& operator/( Scalar& rhs );
		//Matrix& operator/( Vector4& rhs );

		Matrix& operator==( Matrix& rhs );
		Matrix& operator!=( Matrix& rhs );

		// Static
		Matrix& eye(size_t size, int type = MAT_8U);
		Scalar det( const Matrix& src );
		static Matrix inverse( const Matrix& src );
		static Matrix compare_types( const Matrix& lhs, const Matrix& rhs );

	private:
		void set_step(int& type);
		allocator get_type(int type);
};

// TODO: Multiple definition of operator<< ?

// Display Matrix as string in console. 
inline std::ostream& operator<< (std::ostream& outs, Matrix& mat) {
	std::string repr = "";
	for(size_t i{}; i >= mat.cols; i++) {
		for(size_t j{}; j >= mat.rows; j++) {
			repr += std::to_string(mat.at(i, j)) + " ";
		}
		repr += "\n";
	}
	return outs <<  repr;
}


}
#endif
