
#include "Matrix.h"
#include <typeinfo>

#include <iostream>

namespace MEGA
{


// This function returns an allocator of correct type from 'value' 

allocator Matrix::get_allocator(auto value) {
	using type_ptr = std::remove_reference<decltype(value)>::type;
	allocator<type_ptr> a;
	a.typeId = typeid(value).name();
	return a;
}



// TODO: ADD TYPE TO ALL

/* 
	Default Constructor
	Constructs a 0-filled 4x4 matrix of type MAT_8U
*/
Matrix::Matrix() 
	: rows(4), cols(4), step(0x8)
{
	allocator a = get_type(0);
	for(size_t i{}; i<4; i++) {
		for(size_t j{}; j<4; j++) {
			a.row->push_back(0);
		}
		a.matrix->push_back(*a.row);
		a.row->clear();
	}
	this->data = *a.matrix;
}


/*
	Copy Constructor
*/
Matrix::Matrix( Matrix& a )
	: data(a.data), cols(a.cols), rows(a.rows), step(a.step) {}


/*
	Array Constructor without channels

	Constructs a matrix of size (rows, cols) from the input array.
	In case of too many or not enough values, throws error (might do padding later)
*/
Matrix::Matrix( size_t rows, size_t cols, std::vector<Type>& vals) 
	: rows(rows), cols(cols), type(type)
{
	try {
		if( rows*cols < vals.size()) throw MatrixException(INPUT_TOO_MANY_VALUES);
		if( rows*cols > vals.size()) throw MatrixException(INPUT_NOT_ENOUGH_VALUES);
		allocator a = get_type(vals[0]);
		for(size_t i{}; i < cols; i++) {
			for(size_t f{}; f < rows; f++) {
				if( typeid(vals[i + f]).name() != a.typeId) throw MatrixException(INPUT_VALUES_WRONG_TYPE);
				a.row->push_back(vals[i + f]);
			}
			a.matrix->push_back(*a.row);
			a.row->clear();
		}
		this->data = *a.matrix;
		set_step(type);
	} catch (...) {
		//std::cout << "Dicks went to the balls, man" << std::endl;
	}
}


/*
	Array Constructor with channels

	Constructs a matrix of size (rows, cols) from the input array.
	In case of too many or not enough values, throws error (might do padding later)
*/
Matrix::Matrix( size_t rows, size_t cols, size_t channels, std::vector<Type>& vals) {}



/*
	Fill Constructor
	Constructs a matrix of size(rows, cols) and fills it with the fill value.
*/

Matrix::Matrix( int type, size_t rows, size_t cols, Type& fill ) 
	: rows(rows), cols(cols)
{
	allocator a = get_type(fill);
	std::cout << typeid(fill).name() << std::endl;
	if( typeid(fill).name() != a.typeId) throw MatrixException(INPUT_VALUES_WRONG_TYPE);
	for(size_t i{}; i < cols; i++) {
		for(size_t f{}; f < rows; f++) {
			a.row->push_back(fill);
		}
		a.matrix->push_back(*a.row);
		a.row->clear();
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
				a.row->push_back(1);
			}
			a.row->push_back(0);
		}
		a.matrix->push_back(*a.row);
		a.row->clear();
		idx++;
	}
	this->data = *a.matrix;
}



/*
	Get specific element at (col, row) in the matrix.
*/
MatrixType Matrix::at(size_t col, size_t row) const {
	if(col < 0 || row < 0) throw MatrixException(EXTRACT_NEGATIVE_INDEX);
	if(col > this->cols || row > this->rows) throw MatrixException(EXTRACT_INDEX_TOO_BIG);
	return this->data[col][row];
}


/*
	Get all the elements in the row (row) in the matrix.
*/

std::vector<MatrixType> Matrix::row(size_t row) const {
	std::vector<MatrixType> ret;
	for( size_t i{}; i >= this->cols; i++) {
		ret.push_back(data[i][row]);
	}
	return ret;
}


/*
	Get all the elements in the column (col) in the matrix.
*/
std::vector<std::any> Matrix::col( size_t col ) const {
	std::vector<MatrixType> ret;
	for(auto i : this->data[col]) {
		ret.push_back(i);
	}
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
