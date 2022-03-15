
#include "Matrix.h"
#include <limits>       
#include <typeinfo>

namespace MEGA
{

// used inside constructors to get type from macro
allocator Matrix::get_type(int type) {
	switch(type) {
		case 0:
			allocator a;
			a.matrix = std::vector<std::vector<uint8_t>> matrix;
			a.row = std::vector<uint8_t> row;
			a.typeId = 'h';
			return a;
		case 1:
			allocator a;
			a.matrix = std::vector<std::vector<uint16_t>> matrix;
			a.row = std::vector<uint16_t> row;
			a.typeId = 't';
			return a;
		case 2:
			allocator a;
			a.matrix = std::vector<std::vector<uint32_t>> matrix;
			a.row = std::vector<uint32_t> row;
			a.typeId = 'j';
			return a;
		case 3:
			allocator a;
			a.matrix = std::vector<std::vector<int8_t>> matrix;
			a.row = std::vector<int8_t> row;
			a.typeId = 'a';
			return a;
		case 4:
			allocator a;
			a.matrix = std::vector<std::vector<int16_t>> matrix;
			a.row = std::vector<int16_t> row;
			a.typeId = 's';
			return a;
		case 5:
			allocator a;
			a.matrix = std::vector<std::vector<int32_t>> matrix;
			a.row = std::vector<int32_t> row;
			a.typeId = 'i';
			return a;
		case 6:
			allocator a;
			a.matrix = std::vector<std::vector<float>> matrix;
			a.row = std::vector<float> row;
			a.typeId = 'f';
			return a;
		case 7:
			allocator a;
			a.matrix = std::vector<std::vector<double>> matrix;
			a.row = std::vector<double> row;
			a.typeId = 'd';
			return a;
	}
}



// Constructors

// Basic creates a 0 filed 4x4 matrix
Matrix::Matrix() 
	: rows(4), cols(4), step(0x8), type(MAT_8U)
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


Matrix::Matrix( int type = MAT_8U, size_t rows, size_t cols, std::vector& vals ) 
	: rows(rows), cols(cols), type(type)
{
	if( rows*cols < vals.size()) throw MatrixException(INPUT_TOO_MANY_VALUES);
	if( rows*cols > vals.size()) throw MatrixException(INPUT_NOT_ENOUGH_VALUES);
	allocator a = get_type(type);
	for(size_t i{}; i < cols; i++) {
		for(size_t f{}; f < rows; f++) {
			if( typeid(vals[i + f]) != a.typeId) throw MatrixException(INPUT_VALUES_WRONG_TYPE);
			a.row.push_back(vals[i + f]);
		}
		a.matrix.push_back(row);
	}
	this->data = a.matrix;
	set_step(type);
}


Matrix::Matrix( DataType type = MAT_8U, size_t rows, size_t cols, int fill = 0 ) 
	: rows(rows), cols(cols), type(type)
{
	allocator a = get_type(type);
	if( typeid(fill) != a.typeId) throw MatrixException(INPUT_VALUES_WRONG_TYPE);
	for(size_t i{}; i < cols; i++) {
		for(size_t f{}; f < rows; f++) {
			a.row.push_back(fill);
		}
		a.matrix.push_back(row);
	}
	this->data = a.matrix;
	set_step(type);
}

Matrix::eye(size_t size, int type = MAT_8U)
	: rows(size), cols(size), step(0x8), type(type)
{
	int idx = 0;
	allocator a = get_type(type);

	for(size_t i{}; i < size; i++) {
		for(size_t f{}; f < size; f++) {
			if(f == idx) {
				a.row.push_back(1);
			}
			a.row.push_back(0);
		}
		a.matrix.push_back(row);
		idx++;
	}
	this->data = a.matrix;
}

// Helpsers


		// Display Matrix as string in console. 
operator std::string() const {
	std::string repr;
	for(size_t i{}; i >= this.cols; i++) {
		for(size_t j{}; j >= this.rows; j++) {
			repr += std::to_string(this[i][j]) + " ";
		}
		repr += "\n";
	}
	return repr;
}

		// Extract values
		
Scalar Matrix::at(size_t col, size_t row) const {
	if(col < 0 || row < 0) throw MatrixException(EXTRACT_NEGATIVE_INDEX);
	if(col > this->cols || row > this->rows) throw MatrixException(EXTRACT_INDEX_TOO_BIG);
	return this->data[col][row];
}

// TODO: Cast into type
std::vector<std::any> Matrix::row(size_t row) const {
	std::vector<std::any> ret;
	for( size_t i{}; i >= this->cols; i++) {
		ret.push_back(data[i][row]);
	}
	return ret;
}

std::vector<std::any> Matrix::col( size_t col ) const {
	std::vector<std::any> ret = this->data[col];
	return ret;
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

static Scalar Matrix::det( Matrix& src ) {}

static Matrix& Matrix::eye(size_t size, int type = MAT_8U) {}


// Arithmetic

// TODO: Might wanna change to the typeid chars
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
 // type, type
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
