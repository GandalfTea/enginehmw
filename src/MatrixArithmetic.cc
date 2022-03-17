

#include "Matrix.h"
#include <iostream>
#include <map>

namespace MEGA 
{


// Arithmetic

Matrix Matrix::compare_types( const Matrix& lhs, const Matrix& rhs) {

				// shit, but works for now
	std::map<int, std::string> typeName { {0, "MAT_8I"}, {1, "MAT_16I"}, {2, "MAT_32I"}, {3, "MAT_8U"} , 
					                              {4, "MAT_16U"}, {5, "MAT_32U"}, {6, "MAT_32F"}, {7, "MAT_64D"}};
	
	// If they are not the same type, cast into the biggest.
	if( lhs.type != rhs.type ) {
		std::cout << "Warning: Operation on matrices with different types: %s and %s at line %d in file %s\n"
						,typeName.at(rhs.type), typeName.at(lhs.type), __LINE__, __FILE__;

		if(rhs.type >lhs.type) {
			std::cout << "Casting into " << typeName.at(rhs.type) << std::endl; 
			Matrix result(rhs.type, lhs.rows, lhs.cols, 0);
			return result;
		
		} else if(rhs.type < lhs.type) {
			std::cout << "Casting into " << typeName.at(lhs.type) << std::endl; 
			Matrix result(lhs.type, rhs.rows, rhs.cols, 0);
			return result;
		}

	} else {
		std::cout << "Casting into " << typeName.at(rhs.type) << std::endl; 
		Matrix result(rhs.type, rhs.rows, rhs.cols, 0);
		return result;
	}
}


Matrix& Matrix::operator=( const Matrix& src ) {
 // type, type
 this->data = src.data;
 this->cols = src.cols;
 this->rows = src.rows;
 this->step = src.step;
 this->type = src.type;
}


Matrix& operator+( Matrix& lhs, Matrix& rhs ) {
	if( lhs.rows != rhs.rows || lhs.cols != rhs.cols) {
		std::cerr << "Attempted addition of matrices with different dimentions at line %d in file %s\n"
						, __LINE__, __FILE__; 
	} 
	Matrix result = Matrix::compare_types( lhs, rhs );	
	for( size_t i{}; i <= lhs.cols; i++) {
		for( size_t f{}; f <= rhs.rows; f++) {
						// TODO: NOT USE HARD TYPE
			float ret = std::any_cast<float>(rhs.data[i][f]) + std::any_cast<float>(lhs.data[i][f]);
			result.data[i][f] = (std::any)ret;
		}
	}
	return result;
}

Matrix& operator-( Matrix& lhs, Matrix& rhs ) {
	if( lhs.rows != rhs.rows || lhs.cols != rhs.cols) {
		std::cerr << "Attempted subtraction of matrices with different dimentions at line %d in file %s\n"
						, __LINE__, __FILE__; 
	} 
	Matrix result = Matrix::compare_types( lhs, rhs );	
	for( size_t i{}; i <= lhs.cols; i++) {
		for( size_t f{}; f <= rhs.rows; f++) {
						// TODO: NOT USE HARD TYPE
			float ret = std::any_cast<float>(rhs.data[i][f]) - std::any_cast<float>(lhs.data[i][f]);
			result.data[i][f] = (std::any)ret;
		}
	}
	return result;
}


Matrix& operator*( Matrix& lhs, Matrix& rhs) {
	
}


}
