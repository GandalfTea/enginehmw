
#include <Matrix.h>
#include <typeinfo>
#include <iostream>

using namespace MEGA; 
using namespace std;

	/*
	 TO TEST:
	
		Creation
			Creation from array
			Creation with fill
			Create Identity
		Destruction
		Operations on Matrices
			Determinant
			Inverse
			Copy
		Arithmatic
			Matrix Addition
			Matrix Subtraction
			Multiplication with Matrix
			Multiplication with Vector
			Multiplication with Scalar
			Division with Matrix
			Division with Vector
			Division with Scalar
			Compare
		Modification
			GET / SET row
			GET / SET column
			GET / SET value
	*/

/*
 
typeid Output table:

uint32_t - j
uint16_t - t
uint8_t  - h
int32_t  - i
int16_t  - s
int8_t   - a
float    - f
double   - d

*/


class Exception {
	public:
		typedef enum {
			CREATION_INCORRECT_SIZE_ROWS,
			CREATION_INCORRECT_SIZE_COLUMNS,
			CREATION_INCORRECT_TYPE,
			CREATION_INCORRECT_DECLARED_TYPE,
			CREATION_INCORRECT_VALUES,
			CREATION_INCORRECT_VALUES_SPECIFIC,
			CREATION_INCORRECT_STRING_REPRESENTATION
		} Error;
		Error error_;
		explicit Exception(Error error) : error_(error) {}
};



int main(int argc, char* argv[]) {

}

bool test_creation() {
		
	// testing 	Matrix( int type = MAT_8U, size_t rows = 2, size_t cols = 2, std::vector<std::any>& vals );
	cout << "Testing :  Matrix Creation with Array of values." << endl;
	
	// TEST uint8_t
	// this test also contains the allocation test
	vector<uint8_t>  uint8_correct_values              { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	vector<uint8_t>  uint8_incorrect_too_many_values   { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<uint8_t>  uint8_incorrect_not_enough_values { 1, 2, 3, 4, 5, 6, 7, 8 };
	vecto<float> uint8_incorrect_wrong_type_correct_number   { 1.f, 1.5, 4.567, 0.453, 2.f, 5.67, 32.12, 3.f, 6.32, 567.2, 4.f, 5.f };
	vecto<float> uint8_incorrect_wrong_type_incorrect_number { 1.f, 1.5, 4.567, 0.453, 2.f, 5.67, 32.12, 3.f, 6.32, 567.2, 4.f };

	// correct values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_correct_values);
		if (a.rows != 3) throw Exception(CREATION_INCORRECT_SIZE_ROWS);
		if (a.cols != 3) throw Exception(CREATION_INCORRECT_SIZE_COLLUMS);
		if (a.type != MAT_8U) throw Exception(CREATION_INCORRECT_DECLARED_TYPE);
		if (typeid(a.at(0, 0)).name() != 'h') throw Exception(CREATION_INCORRECT_TYPE);

		if (a.row(0) != [1, 2, 3]) throw Exception(CREATION_INCORRECT_VALUES);
		if (a.row(1) != [4, 5, 6]) throw Exception(CREATION_INCORRECT_VALUES);
		if (a.row(2) != [7, 8, 9]) throw Exception(CREATION_INCORRECT_VALUES);
		if (a.col(0) != [1, 4, 7]) throw Exception(CREATION_INCORRECT_VALUES); 
		if (a.col(1) != [2, 5, 8]) throw Exception(CREATION_INCORRECT_VALUES); 
		if (a.col(2) != [3, 6, 9]) throw Exception(CREATION_INCORRECT_VALUES); 
		if (a.at(0, 2) != 3) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
		if (a.at(1, 1) != 5) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
		if (a.at(2, 0) != 7) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 

		// this should not be here
		if (to_string(a) != "1 2 3\n4 5 6\n7 8 9\n") throw Exception(CREATION_INCORRECT_STRING_REPRESENTATION);

	} catch(const Exception exception) {
		cout << "Fail" << endl;
		cout << exception << endl;
		return 0;

	} catch{const MatrixException exception} {
		cout << "Fail" << endl;
		cout << exception << endl;
		return 0;
	}

	// too many values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_incorrect_too_many_values);

	} catch(MatixException exception) {
		cout << "Fail" << endl;
		cout << exception << endl;
		return 0;
	}

	// not enough values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_incorrect_not_enough_values);

	} catch(MatrixException exception) {
		cout << "Fail" << endl;
		cout << exception << endl;
		return 0;
	}

	// incorrect type, correct number of values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_incorrect_wrong_type_correct_number);

	} catch(MatrixException exception) {
		cout << "Fail" << endl;
		cout << exception << endl;
		return 0;
	}

	// incorrect type, incorrect number of values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_incorrect_wrong_type_incorrect_number);

	} catch(MatrixException exception) {
		cout << "Fail" << endl;
		cout << exception << endl;
		return 0;
	}

}
