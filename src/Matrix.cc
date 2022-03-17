
#include "Matrix.h"
#include <typeinfo>

#include <iostream>

namespace MEGA
{


/*
	Allocator used in Matrix constructors.

	In memory, all matrix values are stored as std::any;
	Type checking is done when constructing the matrix and casting is done when extracting from matrix.

	TODO: Find way of storing correct type in memory. If not, then store uchar raw bits for less memory usage.

	uint32_t - j
	uint16_t - t
	uint8_t  - h
	int32_t  - i
	int16_t  - s
	int8_t   - a
	float    - f
	double   - d
*/
allocator Matrix::get_type(int type) {
	allocator a;
	a.matrix = new std::vector<std::vector<std::any>>();
	a.row = new std::vector<std::any>();
	switch(type) {
		case 0:
			a.typeId = 'a';
		case 1:
			a.typeId = 's';
		case 2:
			a.typeId = 'i';
		case 3:
			a.typeId = 'h';
		case 4:
			a.typeId = 't';
		case 5:
			a.typeId = 'j';
		case 6:
			a.typeId = 'f';
		case 7:
			a.typeId = 'd';
	}
	return a;
}


/* 
	Default Constructor

	Constructs a 0-filled 4x4 matrix of type MAT_8u
*/
Matrix::Matrix() 
	: rows(4), cols(4), step(0x8), type(MAT_8U)
{
	allocator a = get_type(type);
	for(size_t i{}; i<4; i++) {
		for(size_t j{}; j<4; j++) {
			a.row->push_back(0);
		}
		a.matrix->push_back(*a.row);
	}
	this->data = *a.matrix;
}


Matrix::Matrix( Matrix& a )
	: data(a.data), cols(a.cols), rows(a.rows), step(a.step), type(a.type) {}


/*
	Array Constructor

	Constructs a matrix of size (rows, cols) from the input array.
	In case of too many or not enough values, throws error (might do padding later)
*/
Matrix::Matrix( int type, size_t rows, size_t cols, std::vector<std::any>& vals) 
	: rows(rows), cols(cols), type(type)
{
	try {
		if( rows*cols < vals.size()) throw MatrixException(INPUT_TOO_MANY_VALUES);
		if( rows*cols > vals.size()) throw MatrixException(INPUT_NOT_ENOUGH_VALUES);
		allocator a = get_type(type);
		for(size_t i{}; i < cols; i++) {
			for(size_t f{}; f < rows; f++) {
				if( typeid(vals[i + f]).name() != a.typeId) throw MatrixException(INPUT_VALUES_WRONG_TYPE);
				a.row->push_back((std::any)vals[i + f]);
			}
			a.matrix->push_back(*a.row);
		}
		this->data = *a.matrix;
		set_step(type);
	} catch (...) {
		//std::cout << "Dicks went to the balls, man" << std::endl;
	}
}

/*
	Fill Constructor

	Constructs a matrix of size(rows, cols) and fills it with the (std::any) fill value.
*/

Matrix::Matrix( int type, size_t rows, size_t cols, std::any fill ) 
	: rows(rows), cols(cols), type(type)
{
	allocator a = get_type(type);
	if( typeid(fill).name() != a.typeId) throw MatrixException(INPUT_VALUES_WRONG_TYPE);
	for(size_t i{}; i < cols; i++) {
		for(size_t f{}; f < rows; f++) {
			a.row->push_back(fill);
		}
		a.matrix->push_back(*a.row);
	}
	this->data = *a.matrix;
	set_step(type);
}

Matrix::~Matrix() {};

/*
	Identity Matrix Constructor

	Creates an identity square matrix of size (size).
*/

Matrix& Matrix::eye(size_t size, int type ) {
	this->rows = size;
	this->cols = size;
	this->step = 0x8;
	this->type = type;

	int idx = 0;
	allocator a = get_type(type);

	for(size_t i{}; i < size; i++) {
		for(size_t f{}; f < size; f++) {
			if(f == idx) {
				a.row->push_back((std::any)1);
			}
			a.row->push_back((std::any)0);
		}
		a.matrix->push_back(*a.row);
		idx++;
	}
	this->data = *a.matrix;
}



/*
	Get specific element at (col, row) in the matrix.

	TODO: cast into specific type before returning.

*/
Scalar Matrix::at(size_t col, size_t row) const {
	if(col < 0 || row < 0) throw MatrixException(EXTRACT_NEGATIVE_INDEX);
	if(col > this->cols || row > this->rows) throw MatrixException(EXTRACT_INDEX_TOO_BIG);
	return std::any_cast<float>(this->data[col][row]);
}

/*
	Get all the elements in the row (row) in the matrix.

	TODO: cast into specific type before returning.

*/

// TODO: CHANGE FROM FLOAT TO STD::ANY
std::vector<float> Matrix::row(size_t row) const {
	std::vector<float> ret;
	for( size_t i{}; i >= this->cols; i++) {
		ret.push_back(std::any_cast<float>(data[i][row]));
	}
	return ret;
}

/*
	Get all the elements in the column (col) in the matrix.

	TODO: cast into specific type before returning.

*/
std::vector<std::any> Matrix::col( size_t col ) const {
	std::vector<std::any> ret(this->data[col]);
	return ret;
}



void Matrix::set_step(int& type) {
	switch(type) {
		case MAT_8U:
		case MAT_8I:
			this->step = 0x8;

		case MAT_16U:
		case MAT_16I:
			this->step = 0x10;

		case MAT_32U:
		case MAT_32I:
		case MAT_32F:
			this->step = 0x20;
		case MAT_64D:
			this->step = 0x40;
	}
}

Scalar Matrix::det( const Matrix& src ) {}


} // namespace
