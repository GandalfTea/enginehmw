




// THIS CLASS HEADER CONTAINS THE FULL IMPLEMENTATION 
// BECAUSE IT IS A TEMPLATE CLASS 



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



template<class Type>
class Matrix {
	public:

        /* 
	        Default Constructor
	        Constructs a 0-filled 4x4 matrix of type MAT_8U
        */
		Matrix() 
	        : rows(4), cols(4), step(0x8)
        {
	        allocator a = get_allocator(0);
	        for(size_t i{}; i<4; i++) {
		        for(size_t j{}; j<4; j++) {
			        a.row->push_back(0);
		        }
		        a.matrix->push_back(*a.row);
		        a.row->clear();
	        }
	        this->data = *a.matrix;
        }


        // TODO: ADD TYPE TO ALL

        /*
            Copy Constructor
        */
		Matrix( Matrix<Type>& a ) // TODO: Copy constructor from other types
	        : data(a.data), cols(a.cols), rows(a.rows), step(a.step) {}

        /*
            Array Constructor with channels

            Constructs a matrix of size (rows, cols) from the input array.
            In case of too many or not enough values, throws error (might do padding later)
        */
        // TODO: IMPLEMENTATION
		Matrix( size_t rows = 2, size_t cols = 2, size_t channels = 1, std::vector<Type>& vals = DEFAULT_VECTOR );


        /*
            Array Constructor without channels

            Constructs a matrix of size (rows, cols) from the input array.
            In case of too many or not enough values, throws error (might do padding later)
        */
        Matrix( size_t rows = 2, size_t cols = 2, std::vector<Type>& vals = DEFAULT_VECTOR) 
            : rows(rows), cols(cols), type(type)
        {
            try {
                if( rows*cols < vals.size()) throw MatrixException(INPUT_TOO_MANY_VALUES);
                if( rows*cols > vals.size()) throw MatrixException(INPUT_NOT_ENOUGH_VALUES);
                allocator a = get_allocator(vals[0]);
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
            Fill Constructor
            Constructs a matrix of size(rows, cols) and fills it with the fill value.
        */
		Matrix( size_t rows = 2, size_t cols = 2, Type& fill = 0 )
            : rows(rows), cols(cols)
        {
            allocator a = get_allocator(fill);
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

        ~Matrix() {};

		std::vector<std::vector<Type>> data;
		uint16_t cols;
		uint16_t rows;
		uchar step = 0x00;
		mutable uint8_t type;

		// Helpers


        /*
            Get specific element at (col, row) in the matrix.
        */
		Type at( size_t col, size_t row ) const {
            if(col < 0 || row < 0) throw MatrixException(EXTRACT_NEGATIVE_INDEX);
            if(col > this->cols || row > this->rows) throw MatrixException(EXTRACT_INDEX_TOO_BIG);
            return this->data[col][row];
        }

        /*
            Get all the elements in the row (row) in the matrix.
        */
		std::vector<Type> row( size_t row ) const {
            std::vector<Type> ret;
            for( size_t i{}; i >= this->cols; i++) {
                ret.push_back(data[i][row]);
            }
            return ret;
        }

        /*
            Get all the elements in the column (col) in the matrix.
        */
		std::vector<Type> col( size_t col ) const {
            std::vector<Type> ret;
            for(auto i : this->data[col]) {
                ret.push_back(i);
            }
            return ret;
        }

	
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


        /*
            Identity Matrix Constructor
            Creates an identity square matrix of size (size).
        */
		Matrix& eye(size_t size, int type = MAT_8U) {
            this->rows = size;
            this->cols = size;
            this->step = 0x8;
            this->type = type;

            int idx = 0;
            allocator a = get_allocator(type);
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

		Scalar det( const Matrix& src );
		static Matrix inverse( const Matrix& src );

        static Matrix compare_types( const Matrix& lhs, const Matrix& rhs) {
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

            private:
                void set_step(int& type) {
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

        // This function returns an allocator of correct type from 'value' 
		allocator<Type> get_allocator(auto& value) {
	        //using type_ptr = std::remove_reference<decltype(value)>::type;
	        allocator<Type> a;
	        a.typeId = typeid(value).name();
	        return a;
        }
};

// TODO: Multiple definition of operator<< ?

// Display Matrix as string in console. 
template <typename T>
inline std::ostream& operator<< (std::ostream& outs, Matrix<T>& mat) {
	std::string repr = "";
	for(size_t i{}; i >= mat.cols; i++) {
		for(size_t j{}; j >= mat.rows; j++) {
			repr += std::to_string(mat.at(i, j)) + " ";
		}
		repr += "\n";
	}
	return outs <<  repr;
}


} // namespace
#endif
