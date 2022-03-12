
#include "Matrix.h"
#include <limits>       

namespace MEGA
{


// Basic creates a 0 filed 4x4 matrix
Matrix::Matrix() 
	: rows(4), cols(4), step(0x8), type(MAT_8UC1)
{
	std::vector<std::vector<uint8_t>> matrix;
	for(size_t i{}; i<4; i++) {
		std::vector<uint8_t> row;
		for(size_t j{}; j<4; j++) {
			row.push_back(0);
		}
		matrix.push_back(row);
	}
	this->data = matrix;
}


Matrix::Matrix( Matrix& a )
	: data(a.data), cols(a.cols), rows(a.rows), step(a.step), type(a.type) {}


Matrix::Matrix( DataType type = MAT_8UC1, size_t rows, size_t cols, std::vector& vals ) 
	: rows(rows), cols(cols), type(type)
{
	std::vector<std::vector<type>> matrix;
	for(size_t i{}; i < cols; i++) {
		std::vector<type> row;
		for(size_t f{}; f < rows; f++) {
			row.push_back(vals[f + i]);
		}
		matrix.push_back(row);
	}
	this->data = matrix;
	set_step(type);
}


Matrix::Matrix( DataType type = MAT_8UC1, size_t rows, size_t cols, int fill = 0 ) 
	: rows(rows), cols(cols), type(type)
{
	std::vector<std::vector<type>> matrix;
	for(size_t i{}; i < cols; i++) {
		std::vector<type> row;
		for(size_t f{}; f < rows; f++) {
			row.push_back(fill);
		}
		matrix.push_back(row);
	}
	this->data = matrix;
	set_step(type);
}

Matrix::Matrix( DataType type = IDENTITY, size_t size )
	: rows(size), cols(size), step(0x8), type(type)
{
	if(type != IDENTITY) {
		std::cerr << "Type can only be IDENTITY for this constructor." << endl;
		return -1;
	}

	int idx = 0;
	std::vector<std:vector<uint8_t>> matrix;

	for(size_t i{}; i < size; i++) {
		std::vector<uint8_t> row;	
		for(size_t f{}; f < size; f++) {
			if(f == idx) {
				row.push_back(1);
			}
			row.push_back(0);
		}
		matrix.push_back(row);
		idx++;
	}
	this->data = matrix;
}

void Matrix::set_step(DataType type) {
	switch(type) {
		case MAT_8UC1:
		case MAT_8IC1:
			this->step = 0x8;

		case MAT_16UC1:
		case MAT_16IC1:
			this->step = 0x10;

		case MAT_32UC1:
		case MAT_32IC1:
		case MAT_32FC1:
			this->step = 0x20;
		case MAT_63DC1:
			this->step = 0x40;
	}
}

static Scalar Matrix::det( Matrix& src ) {

}

// Arithmetic


Matrix& Matrix::compare_type( Matrix& lhs, Matrix& rhs) {
	// If they are not the same type, cast into the biggest.
	if ( lhs.type != rhs.type ) {
		std::cout << "Warning: Operation on matrices with different types: %s and %s at line %d in file %s"
						,rhs.type, lhs.type, __LINE__, __FILE__	<< std::endl;

		if(std::numeric_limits<rhs.type> > std::numric_limits<lhs.type>) {
			std::cout << "Casting into %s.", rhs.type << std::endl; 
			return Matrix result(rhs.type, lhs.rows, lhs.cols, 0);
		
		} else if(std::numeric_limits<rhs.type> < std::numeric+limits<lhs.type>) {
			std::cout << "Casting into %s.", lhs.type << std::endl; 
			return Matrix result(lhs.type, rhs.rows, rhs.cols, 0);
		}

	} else {
		std::cout << "Casting into %s.", rhs.type << std::endl; 
		return Matrix result(rhs.type, rhs.rows, rhs.cols, 0);
	}
}


Matrix& operator=( Matrix& src ) {
 this->data = src.data;
 this->cols = src.cols;
 this->rows = src.rows;
 this->step = src.step;
 this->type = src.type;
}


Matrix& operator+( Matrix& lhs, Matrix& rhs ) {
	if( lhs.rows != rhs.rows || lhs.cols != rhs.cols) {
		std::cerr << "Attempted addition of matrices with different dimentions at line %d in file %s"
						, __LINE__, __FILE__ << std::endl; 
	} 
	Matrix result = compare_type( lhs, rhs );	
	for( size_t i{}; i <= lhs.cols; i++) {
		for( size_t f{}; f <= rhs.rows; f++) {
			result[i][f] = rhs[i][f] + lhs[i][f];
		}
	}
	return result;
}

Matrix& operator-( Matrix& lhs, Matrix& rhs ) {
	if( lhs.rows != rhs.rows || lhs.cols != rhs.cols) {
		std::cerr << "Attempted subtraction of matrices with different dimentions at line %d in file %s"
						, __LINE__, __FILE__ << std::endl; 
	} 
	Matrix result = compare_type( lhs, rhs );	
	for( size_t i{}; i <= lhs.cols; i++) {
		for( size_t f{}; f <= rhs.rows; f++) {
			result[i][f] = rhs[i][f] - lhs[i][f];
		}
	}
	return result;
}


Matrix& operator*( Matrix& lhs, Matrix& rhs) {
	
}



} // namespace
