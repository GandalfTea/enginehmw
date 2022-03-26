
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

template <typename T>
inline std::vector<T> splitArray(const std::vector<T>& arr, size_t from, size_t to) {
	if(from < 0 || to > arr.size()) {
		cerr << "Fail SPLIT_INCORRECT_INDICIES from " << to_string(from) << " to " << to_string(to) << endl;
	}
	std::vector<T> ret{};
	for(size_t i = from ; i <= to ; i++) {
		ret.push_back(arr[i]);
	}
	return ret;
}





bool test_creation() {
		
	cout << "\n\nTesting :  Matrix Creation with Array of values." << endl;

	// TEST uint8_t
	// this test also contains the allocation test
	vector<uint8_t>  uint8_correct_values              { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	vector<uint8_t>  uint8_incorrect_too_many_values   { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<uint8_t>  uint8_incorrect_not_enough_values { 1, 2, 3, 4, 5, 6, 7, 8 };
    vector<float>  float_correct_values {1.86, 8845.12548, 254.156, 0.2548, 0.5698, 0.002, 0.875, 8.2158 };
    vector<float>  float_lotta_values;
    for(size_t i{0}; i < 10000000; i++) {
        float f = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float_lotta_values.push_back(f);
    }

	
	// correct values
	try {

		Matrix<U8C1> a (3, 3, uint8_correct_values);
        Matrix<F32C1> b (4, 2, float_correct_values);
        Matrix<F32C1> c (500000, 20, float_lotta_values);

		cout << "\tMatrix Metadata" << endl;

		if (a.rows != 3) throw Exception(CREATION_INCORRECT_SIZE_ROWS);
		if (a.cols != 3) throw Exception(CREATION_INCORRECT_SIZE_COLUMNS);
		if (a.type != MAT_U8C1) throw Exception(CREATION_INCORRECT_DECLARED_TYPE);

		// TODO: test after cast
		//if (*typeid(a.at(0, 0)).name() != 'h') throw Exception(CREATION_INCORRECT_TYPE);

		cout << "\tPass" << endl;
		cout << "\tRows Integrity" << endl;

		if (a.row(0) != splitArray<uint8_t>(uint8_correct_values, 0, 2)) throw Exception(CREATION_INCORRECT_VALUES);
		if (a.row(1) != splitArray<uint8_t>(uint8_correct_values, 3, 5)) throw Exception(CREATION_INCORRECT_VALUES);
		if (a.row(2) != splitArray<uint8_t>(uint8_correct_values, 6, 8)) throw Exception(CREATION_INCORRECT_VALUES);

		cout << "\tPass" << endl;
		cout << "\tColumns Integrity" << endl;

		if (a.col(0)[0] != 1 || 
		    a.col(0)[1] != 4 || 
		    a.col(0)[2] != 7)
				throw Exception(CREATION_INCORRECT_VALUES); 
		if (a.col(1)[0] != 2 || 
			a.col(1)[1] != 5 || 
			a.col(1)[2] != 8) 
				throw Exception(CREATION_INCORRECT_VALUES); 
		if (a.col(2)[0] != 3 || 
			a.col(2)[1] != 6 || 
			a.col(2)[2] != 9) 
				throw Exception(CREATION_INCORRECT_VALUES); 

		cout << "\tPass" << endl;

		if (a.at(0, 2) != 3) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
		if (a.at(1, 1) != 5) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
		if (a.at(2, 0) != 7) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 

		//TODO: Test operator<<
		//if ((std::string)a != "1 2 3\n4 5 6\n7 8 9\n") throw Exception(CREATION_INCORRECT_STRING_REPRESENTATION);

	} catch(const Exception exception) {
		cout << "Fail" << endl;
		return 0;

	} catch(MatrixException exception) {
		cout << "Fail " << endl;
		return 0;
	}

	cout << "\nTesting :  Matrix Creation with Incorrect Values" << endl;




    cout << "\tToo Many Values" << endl;
	try {
		Matrix<U8C1> a (2, 3, uint8_incorrect_too_many_values);
	} catch(MatrixException exception) {
		if( exception.error_ != MatrixError::INPUT_TOO_MANY_VALUES ) {
			cout << "Fail" << endl;
		}
        cout << "\tPass" << endl;
	} catch (...) {
        cout << "Unexpected Error thrown. Stoping." << endl;
    }




    cout << "\tNot Enough Values" << endl;
	try {
		Matrix<U8C1> a (3, 3, uint8_incorrect_not_enough_values);

	} catch(MatrixException exception) {
		if( exception.error_ != MatrixError::INPUT_NOT_ENOUGH_VALUES ) {
			cout << "Fail" << endl;
		}
        cout << "\tPass" << endl;
	}


    return true;
}
