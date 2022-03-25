
#ifndef MATRIX
#define MATRIX

#include "Types.h"
#include <stdio.h>
#include <string>
#include <iostream>

namespace MEGA 
{

template <typename T>
struct allocator {
	std::vector<std::vector<T>> matrix;
	std::vector<T> row;
	std::string typeId;
};

// ERROR HANDLING
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

inline std::ostream& operator<< (std::ostream& outs, MatrixException& err) {
		if( err.error_ == INPUT_VALUES_WRONG_TYPE)
			outs << "INPUT_VALUES_WRONG_TYPE";
		else if( err.error_ == INPUT_TOO_MANY_VALUES)
			outs << "INPUT_TOO_MANY_VALUES";
		else if( err.error_ == INPUT_NOT_ENOUGH_VALUES)
			outs << "INPUT_NOT_ENOGUH_VALUES";
		else if( err.error_ == EXTRACT_INDEX_TOO_BIG)
			outs << "EXTRACT_INDEX_TOO_BIG";
		else if( err.error_ == EXTRACT_NEGATIVE_INDEX)
			outs << "EXTRACT_NEGATIVE_INDEX";
	return outs;
}



template<class T>
class Matrix {
	public:
		Matrix();
		Matrix( Matrix& a );
		Matrix( size_t rows = 2, size_t cols = 2, size_t channels, std::vector<T>& vals = DEFAULT_VECTOR );
		Matrix( size_t rows = 2, size_t cols = 2, std::vector<T>& vals = DEFAULT_VECTOR );
		Matrix( size_t rows = 2, size_t cols = 2, T& fill = 0 );
		~Matrix();

		std::vector<std::vector<T>> data;
		uint16_t cols;
		uint16_t rows;
		uchar step = 0x00;
		mutable uint8_t type;

		// Helpers
		MatrixType at( size_t col, size_t row ) const;
		std::vector<MatrixType> row( size_t row ) const;
		std::vector<MatrixType> col( size_t col ) const;
	
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
		allocator get_allocator(auto& value);
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
