




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
// .....................................................................................
typedef enum {
	INPUT_VALUES_WRONG_TYPE,
	INPUT_TOO_MANY_VALUES,
	INPUT_NOT_ENOUGH_VALUES,
	EXTRACT_INDEX_TOO_BIG,
	EXTRACT_NEGATIVE_INDEX
} MatrixError;

class MatrixException {
	public:
		MatrixError error_;
		explicit MatrixException(MatrixError error) : error_(error) {}

};

inline std::ostream& operator<< (std::ostream& outs, MatrixException& err) {
		if( err.error_ == INPUT_VALUES_WRONG_TYPE)
			outs << "\nINPUT_VALUES_WRONG_TYPE" << " %s", __LINE__;
		else if( err.error_ == INPUT_TOO_MANY_VALUES)
			outs << "\nINPUT_TOO_MANY_VALUES";
		else if( err.error_ == INPUT_NOT_ENOUGH_VALUES)
			outs << "\nINPUT_NOT_ENOGUH_VALUES";
		else if( err.error_ == EXTRACT_INDEX_TOO_BIG)
			outs << "\nEXTRACT_INDEX_TOO_BIG";
		else if( err.error_ == EXTRACT_NEGATIVE_INDEX)
			outs << "\nEXTRACT_NEGATIVE_INDEX";
	return outs;
}


// MATRIX BASE TEMPLATE CLASS
// .....................................................................................

template<class Type>
class Matrix {

	public:


        // MEMBER VARIABLES
		std::vector<std::vector<Type>> data;
		uint16_t cols;
		uint16_t rows;
		uchar step = 0x00;
		mutable MatrixType type;
        // Maybe should hold allocator ?


        /*  .....................................................................................

	        Default Constructor
            TODO: Init the header. Wait for the user to call the Matrix.create() function.
            .....................................................................................
        */
		Matrix() {
            set_type();
            set_step((uint8_t)this->type);
        }


        /*  .....................................................................................

            Copy Constructor
            .....................................................................................
        */
		Matrix( Matrix<Type>& a ) // TODO: Copy constructor from other types
	        : data(a.data), cols(a.cols), rows(a.rows), step(a.step) {}



        /*  .....................................................................................

            Array Constructor with channels
            Constructs a matrix of size (rows, cols) from the input array.
            In case of too many or not enough values, throws error (might do padding later)
            TODO: IMPLEMENTATION
            .....................................................................................
        */
		Matrix(std::vector<Type>& vals, size_t rows, size_t cols, size_t channels = 1 ) {
            std::cout << "CALL FOR CHANNEL" << std::endl;
        }



        /*  .....................................................................................

            Array Constructor without channels
            Constructs a matrix of size (rows, cols) from the input array.
            In case of too many or not enough values, throws error (might do padding later)
            .....................................................................................
        */
        Matrix( size_t cols, size_t rows, std::vector<Type>& vals = DEFAULT_VECTOR<Type>) 
            : rows(rows), cols(cols)
        {
            if( rows*cols < vals.size()) throw MatrixException(INPUT_TOO_MANY_VALUES);
            if( rows*cols > vals.size()) throw MatrixException(INPUT_NOT_ENOUGH_VALUES);
            allocator a = get_allocator(vals[0]);
            for(size_t i{0}; i <= vals.size(); i++) {
                if( typeid(vals[i]).name() != a.typeId) throw MatrixException(INPUT_VALUES_WRONG_TYPE);
                if(i % cols == 0 && i != 0) {
                    a.matrix.push_back(a.row);
                    a.row.clear();
                }
                a.row.push_back(vals[i]);
            }
            this->data = a.matrix;
            set_type();
            set_step((uint8_t)this->type);
        }


        /*  .....................................................................................

            Fill Constructor
            Constructs a matrix of size(rows, cols) and fills it with the fill value.
            .....................................................................................
        */
		Matrix( size_t rows, size_t cols, Type fill = 0 )
            : rows(rows), cols(cols)
        {
            allocator a = get_allocator(fill);
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


        /*  .....................................................................................

            Destructor 
            Does object cleanul, frees up memory.
            TODO: Implement
            .....................................................................................
        */
        ~Matrix() {};



		// Helpers
// ................................................................................................

        /*
            Get specific element at (col, row) in the matrix.
            .....................................................................................
        */
		Type at( size_t col, size_t row ) const {
            if(col < 0 || row < 0) throw MatrixException(EXTRACT_NEGATIVE_INDEX);
            if(col > this->cols || row > this->rows) throw MatrixException(EXTRACT_INDEX_TOO_BIG);
            return this->data[col][row];
        }

        /*
            Get all the elements in the row (row) in the matrix.
            .....................................................................................
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
            .....................................................................................
        */
		std::vector<Type> col( size_t col ) const {
            std::vector<Type> ret;
            for(auto i : data) {
                ret.push_back(i[col]);
            }
            return ret;
        }

	
		// Arithmatic
// ................................................................................................

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
// ................................................................................................


        /*  

            Identity Matrix Constructor
            Creates an identity square matrix of size (size).
            TODO: Reimplement this using % size+1
            .....................................................................................
        */
		Matrix& eye(size_t size, int type = MAT_U8C1) {
            this->rows = size;
            this->cols = size;
            this->step = 0x8;

            int idx = 0;
            allocator a = get_allocator(type);

            for(size_t i{0}; i <= size; i++) {
                for(size_t j{0}; j <= size; j++) {
                    (i == j) ?  a.row.push_back(1) : a.row.push_back(0);
                }
                a.matrix.push_back(a.row);
                a.row.clear();
            }
            this->data = a.matrix;
        }

        /*  .....................................................................................

            Get determinant of matrix.
            TODO: implement
            .....................................................................................
        */
		Scalar det( const Matrix& src );
		static Matrix inverse( const Matrix& src );


        /*  .....................................................................................

            Compare different matrix types in order to cast to biggest one.
            .....................................................................................
        */
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

        /*
            Set hex step between matrix elements 
            (not in use, future use is planned, when the data in the matrix will be bits)
            .....................................................................................
        */
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
        //.....................................................................................
        allocator<Type> get_allocator(auto& value) {
            //using type_ptr = std::remove_reference<decltype(value)>::type;
            allocator<Type> a;
            a.typeId = typeid(value).name();
            return a;
        }


        // set type from template input
        // terrible code, but there is not switch for string.
        //.....................................................................................
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


//      ARITHMETIC AND OPERATORS
// ................................................................................................


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
