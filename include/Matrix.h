




// THIS CLASS HEADER CONTAINS THE FULL IMPLEMENTATION 
// BECAUSE IT IS A TEMPLATE CLASS 



#ifndef MATRIX
#define MATRIX

#include "Types.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <map>

namespace MEGA 
{

template <typename T>
struct allocator {
	std::vector<std::vector<T>> matrix;
	std::vector<T> row;
	std::string typeId;
};

template <typename T>
std::vector<T> DEFAULT_VECTOR;


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
			outs << "INPUT_VALUES_WRONG_TYPE" << " %s", __LINE__;
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
		Matrix( size_t rows = 2, size_t cols = 2, size_t channels = 1, std::vector<Type>& vals = DEFAULT_VECTOR<Type> ) {}


        /*
            Array Constructor without channels

            Constructs a matrix of size (rows, cols) from the input array.
            In case of too many or not enough values, throws error (might do padding later)
        */
        Matrix( size_t rows = 2, size_t cols = 2, std::vector<Type>& vals = DEFAULT_VECTOR<Type>) 
            : rows(rows), cols(cols), type(type)
        {
            try {
                if( rows*cols < vals.size()) throw MatrixException(INPUT_TOO_MANY_VALUES);
                if( rows*cols > vals.size()) throw MatrixException(INPUT_NOT_ENOUGH_VALUES);
                allocator a = get_allocator(vals[0]);
                for(size_t i{0}; i <= vals.size(); i++) {
                    if( typeid(vals[i]).name() != a.typeId) throw MatrixException(INPUT_VALUES_WRONG_TYPE);
                    if(i % rows == 0 && i != 0) {
                        a.matrix.push_back(a.row);
                        a.row.clear();
                    }
                    a.row.push_back(vals[i]);
                }
                this->data = a.matrix;
                set_step((uint8_t)type);
                set_type();
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
                    a.row.push_back(fill);
                }
                a.matrix.push_back(a.row);
                a.row.clear();
            }
            this->data = a.matrix;
            set_step((uint8_t)type);
        }

        ~Matrix() {};

        // MEMBER VARIABLES

		std::vector<std::vector<Type>> data;
		uint16_t cols;
		uint16_t rows;
		uchar step = 0x00;
		mutable MatrixType type;

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
            for( auto i : this->data[row]) { 
                ret.push_back(i);
            }
            return ret;
        }

        /*
            Get all the elements in the column (col) in the matrix.
        */
		std::vector<Type> col( size_t col ) const {
            std::vector<Type> ret;
            for(auto i : data) {
                ret.push_back(i[col]);
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
		Matrix& eye(size_t size, int type = MAT_U8C1) {
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
            std::map<int, std::string> typeName { {0, "MAT_S8C1"}, {1, "MAT_S16C1"}, {2, "MAT_S32C1"}, {3, "MAT_U8C1"} , 
                                                          {4, "MAT_U16C1"}, {5, "MAT_U32C1"}, {6, "MAT_F32C1"}, {7, "MAT_F64C1"}};

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
        void set_step(uint8_t type) {
            switch(type) {
                case MAT_U8C1:
                case MAT_U8C2:
                case MAT_U8C3:
                case MAT_S8C1:
                case MAT_S8C2:
                case MAT_S8C3:
                    this->step = 0x8;
                    break;
                case MAT_U16C1:
                case MAT_U16C2:
                case MAT_U16C3:
                case MAT_S16C1:
                case MAT_S16C2:
                case MAT_S16C3:
                    this->step = 0x10;

                case MAT_U32C1:
                case MAT_U32C2:
                case MAT_U32C3:
                case MAT_S32C1:
                case MAT_S32C2:
                case MAT_S32C3:
                case MAT_F32C1:
                case MAT_F32C2:
                case MAT_F32C3:
                    this->step = 0x20;
                case MAT_F64C1:
                case MAT_F64C2:
                case MAT_F64C3:
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


        // set type from template input
        // terrible code, but there is not switch for string.
        void set_type() {
            // TODO: Check for complex types
            if (*typeid(Type).name() == 'j') this->type = MAT_U32C1;
            else if (*typeid(Type).name() == 't') this->type = MAT_U16C1;
            else if (*typeid(Type).name() == 'h') this->type = MAT_U8C1;
            else if (*typeid(Type).name() == 'i') this->type = MAT_S32C1;
            else if (*typeid(Type).name() == 's') this->type = MAT_S16C1;
            else if (*typeid(Type).name() == 'a') this->type = MAT_S8C1;
            else if (*typeid(Type).name() == 'f') this->type = MAT_U32C1;
            else if (*typeid(Type).name() == 'd') this->type = MAT_F64C1;
        }
};

// TODO: Multiple definition of operator<< ?

// Display Matrix as string in console. 
template <typename T>
inline std::ostream& operator<< (std::ostream& outs, Matrix<T>& mat) {
	std::string repr = "";
    for (auto i : mat.data) {
        repr += "\n\t";
        for (auto j : i) {
            repr += std::to_string(j) + " ";
        }
    }
	return outs <<  repr;
}


} // namespace
#endif
