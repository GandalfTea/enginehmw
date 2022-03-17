
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


// Prototypes
bool test_creation();


typedef enum {
	CREATION_INCORRECT_SIZE_ROWS = 0,
	CREATION_INCORRECT_SIZE_COLUMNS = 1,
	CREATION_INCORRECT_TYPE = 2,
	CREATION_INCORRECT_DECLARED_TYPE = 3,
	CREATION_INCORRECT_VALUES = 4,
	CREATION_INCORRECT_VALUES_SPECIFIC = 5,
	CREATION_INCORRECT_STRING_REPRESENTATION = 6
} TestingError;


// this is very very stupid but works for now;
class Exception {
	public:
		const char* printError[7] = {"CREATION_INCORRECT_SIZE_ROWS",
															"CREATION_INCORRECT_SIZE_COLUMNS",
		                          "CREATION_INCORRECT_TYPE",
		                          "CREATION_INCORRECT_DECLARED_TYPE",
		                          "CREATION_INCORRECT_VALUES",
		                          "CREATION_INCORRECT_VALUES_SPECIFIC",
		                          "CREATION_INCORRECT_STRING_REPRESENTATION"};
		TestingError error_;
		explicit Exception(TestingError error) : error_(error) {
			cout << printError[error] << endl;
		}
};


int main(int argc, char* argv[]) {
	test_creation();
}

inline std::vector<std::any> splitArray(std::vector<std::any> arr, size_t from, size_t to) {
	if(from <= 0 || to > arr.size()) throw 0;
	std::vector<std::any> ret;
	for(size_t i = from; i == to; i++) {
		ret.push_back(arr[i]);
	}
	return ret;
}

bool test_creation() {
		
	// testing 	Matrix( int type = MAT_8U, size_t rows = 2, size_t cols = 2, std::vector<std::any>& vals );
	cout << "Testing :  Matrix Creation with Array of values." << endl;

	// TEST uint8_t
	// this test also contains the allocation test
	vector<std::any>  uint8_correct_values              { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	vector<std::any>  uint8_incorrect_too_many_values   { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<std::any>  uint8_incorrect_not_enough_values { 1, 2, 3, 4, 5, 6, 7, 8 };
	vector<std::any>  uint8_incorrect_wrong_type_correct_number   { 1.f, 1.5, 4.567, 0.453, 2.f, 5.67, 32.12, 3.f, 6.32, 567.2, 4.f, 5.f };
	vector<std::any>  uint8_incorrect_wrong_type_incorrect_number { 1.f, 1.5, 4.567, 0.453, 2.f, 5.67, 32.12, 3.f, 6.32, 567.2, 4.f };

	
	// correct values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_correct_values);

		if (a.rows != 3) throw Exception(CREATION_INCORRECT_SIZE_ROWS);
		if (a.cols != 3) throw Exception(CREATION_INCORRECT_SIZE_COLUMNS);
		if (a.type != MAT_8U) throw Exception(CREATION_INCORRECT_DECLARED_TYPE);
		if (*typeid(a.at(0, 0)).name() != 'h') throw Exception(CREATION_INCORRECT_TYPE);

		// !!!! TODO : THIS IS THE CODE THROWING THE WIERD VECTOR ERROR
		//if (a.row(0) != splitArray(uint8_correct_values, 0, 2)) throw Exception(CREATION_INCORRECT_VALUES);
		//if (a.row(0) != splitArray(uint8_correct_values, 3, 5)) throw Exception(CREATION_INCORRECT_VALUES);
		//if (a.row(0) != splitArray(uint8_correct_values, 6, 8)) throw Exception(CREATION_INCORRECT_VALUES);

		//TODO: find way to cast before returning
		if (std::any_cast<uint8_t>(a.col(0)[0]) != 1 || 
				std::any_cast<uint8_t>(a.col(0)[1]) != 4 || 
				std::any_cast<uint8_t>(a.col(0)[2]) != 7)
						throw Exception(CREATION_INCORRECT_VALUES); 
		if (std::any_cast<uint8_t>(a.col(1)[0]) != 2 || 
				std::any_cast<uint8_t>(a.col(1)[1]) != 5 || 
				std::any_cast<uint8_t>(a.col(1)[2]) != 8) 
						throw Exception(CREATION_INCORRECT_VALUES); 
		if (std::any_cast<uint8_t>(a.col(2)[0]) != 3 || 
				std::any_cast<uint8_t>(a.col(2)[1]) != 6 || 
				std::any_cast<uint8_t>(a.col(2)[2]) != 9) 
						throw Exception(CREATION_INCORRECT_VALUES); 
		if (std::any_cast<uint8_t>(a.at(0, 2)) != 3) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
		if (std::any_cast<uint8_t>(a.at(1, 1)) != 5) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
		if (std::any_cast<uint8_t>(a.at(2, 0)) != 7) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 

		//TODO: Test operator<<
		//if ((std::string)a != "1 2 3\n4 5 6\n7 8 9\n") throw Exception(CREATION_INCORRECT_STRING_REPRESENTATION);

	} catch(const Exception exception) {
		cout << "Fail" << endl;
		return 0;

	} catch(Error exception) {
		cout << "Fail" << endl;
		return 0;
	}

	// too many values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_incorrect_too_many_values);

	} catch(Error exception) {
		if( exception != Error::INPUT_TOO_MANY_VALUES ) {
			cout << "Fail" << endl;
		}
		return 0;
	}

	// not enough values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_incorrect_not_enough_values);

	} catch(Error exception) {
		cout << "Fail" << endl;
		return 0;
	}

	// incorrect type, correct number of values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_incorrect_wrong_type_correct_number);

	} catch(Error exception) {
		cout << "Fail" << endl;
		return 0;
	}

	// incorrect type, incorrect number of values
	try {
		Matrix a(MAT_8U, 3, 3, uint8_incorrect_wrong_type_incorrect_number);

	} catch(Error exception) {
		cout << "Fail" << endl;
		return 0;
	}

}
