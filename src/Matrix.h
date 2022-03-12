

#include <vector>

#ifndef MATRIX
#define MATRIX

using namespace MEGA 
{

typedef int Scalar;

// maybe use typedef ? 
template <uint8_t MAT_8UC1>
template <uint16_t MAT_16UC1>
template <uint32_t MAT_32UC1>
template <int8_t MAT_8IC1>
template <int16_t MAT_16UC1>
template <int32_t MAT_31UC1>
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
		Matrix( SataType type = IDENTITY, size_t size );
		~Matrix();

		std::vector<std::vector<DataType>> data;
		int cols;
		int rows;
		uint32_t step = 0x00;

		// helpers
		static Scalar det( Matrix& a );
		static Matrix inverse( Matrix& a );

	private:
		void set_step(DataType type);
};


Matrix::Matrix( Matrix& a ) {}

Matrix::Matrix( DataType type = MAT_8UC1, size_t rows, size_t cols, std::vector& vals ) {
	std::vector<std::vector<type>> matrix;
	for(size_t i{}; i < cols; i++) {
		std::vector<type> row;
		for(size_t f{}; f < rows; f++) {
			row.push_back(vals[f + i]);
		}
		matrix.push_back(row);
	}
	this->data = matrix;
	this->cols = cols;
	this->rows = rows;
	set_step(type);
}

Matrix::Matrix( DataType type = MAT_8UC1, size_t rows, size_t cols, int fill = 0 ) {
	std::vector<std::vector<type>> matrix;
	for(size_t i{}; i < cols; i++) {
		std::vector<type> row;
		for(size_t f{}; f < rows; f++) {
			row.push_back(fill);
		}
		matrix.push_back(row);
	}
	this->data = matrix;
	this->cols = cols;
	this->rows = rows;
	set_step(type);
}

Matrix::Matrix( DataType type = IDENTITY, size_t size ) {
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
	this->cols = size;
	this->rows = size;
	this->step = 0x8;
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

}
#endif
