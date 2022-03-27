
#include <Matrix.h>
#include <typeinfo>
#include <iostream>
#include <chrono>

using namespace MEGA; 
using namespace std;
    using namespace std::chrono;

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
			cout << "\n" << printError[error] << endl;
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
    cout << "\tMatrix Creation" << endl;

	// Primitive Types
 
	vector<uint8_t>  uint8_correct_values              { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	vector<uint8_t>  uint8_incorrect_too_many_values   { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<uint8_t>  uint8_incorrect_not_enough_values { 1, 2, 3, 4, 5, 6, 7, 8 };
    vector<uint16_t> uint16_lotta_values;
    for(size_t i{0}; i < 9998244; i++) {
        uint16_t f = static_cast<uint16_t>(rand()) / static_cast<uint16_t>(RAND_MAX);
        uint16_lotta_values.push_back(f);
    }
    vector<float>  float_correct_values {1.86, 8845.12548, 254.156, 0.2548, 0.5698, 0.002, 0.875, 8.2158 };
    vector<float>  float_lotta_values;
    for(size_t i{0}; i < 10000000; i++) {
        float f = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float_lotta_values.push_back(f);
    }
    vector<char> char_test { 't', 'i', 't', 's', 'u', 'p'};
    vector<double> double_lotta_values;
    for(size_t i{0}; i < 100000000; i++) {
        double f = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        double_lotta_values.push_back(f);
    }

    // Compount Types
    
    /*
    vector<uint16_t> buffer;
    vector<U16C3> uint16_3channel;
    for (auto i : uint16_lotta_values) {
        buffer.push_back(i);
        if(buffer.size() == 3) {
            uint16_3channel.push_back(buffer);
            buffer.clear();
        }
    }
    */

	
	try {
        auto start = high_resolution_clock::now();
		Matrix<U8C1> a (3, 3, uint8_correct_values);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
		cout << "\t\t" << duration.count() << "ms ...  9 elements - 3 x 3 MAT_U8C1" << endl;

        start = high_resolution_clock::now();
        Matrix<F32C1> b (4, 2, float_correct_values);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
		cout << "\t\t" << duration.count() << "ms ...  9 elements - 4 x 2 MAT_F32C1" << endl;

        start = high_resolution_clock::now();
        Matrix<F32C1> c (500000, 20, float_lotta_values);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
		cout << "\t\t" << duration.count() << "ms ...  10.000.000 elements - 500000 x 20 MAT_F32C1" << endl;

        start = high_resolution_clock::now();
        Matrix<F64C1> d (500, 200000, double_lotta_values);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
		cout << "\t\t" << duration.count() << "ms ...  100.000.000 elements - 500 x 200000 MAT_F64C1" << endl;

        start = high_resolution_clock::now();
        Matrix<U16C1> e (3162, 3162, uint16_lotta_values);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
		cout << "\t\t" << duration.count() << "ms ...  9.998.244 elements - 3162 x 3162 MAT_U16C1" << endl;

        //Matrix<U16C3> z (3162, 3162, uint16_3channel);

        start = high_resolution_clock::now();
        Matrix<F32C1> x (4, 2500000, float_lotta_values);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
		cout << "\t\t" << duration.count() << "ms ...  10.000.000 elements - 4 x 2500000 MAT_F32C1" << endl;


        // FILL CONSTRUCTOR
        Matrix<F32C1> v (5, 10, 0);

        // IDENTITY MATRIX
        Matrix<F64C1> h;
        h.eye(10);
        

		cout << "\tMatrix Metadata";

		if (a.rows != 3) throw Exception(CREATION_INCORRECT_SIZE_ROWS);
		if (a.cols != 3) throw Exception(CREATION_INCORRECT_SIZE_COLUMNS);
		if (a.type != MAT_U8C1) throw Exception(CREATION_INCORRECT_DECLARED_TYPE);

		cout << " - Pass" << endl;
		cout << "\tRows Integrity";

		if (a.row(0) != splitArray<uint8_t>(uint8_correct_values, 0, 2)) throw Exception(CREATION_INCORRECT_VALUES);
		if (a.row(1) != splitArray<uint8_t>(uint8_correct_values, 3, 5)) throw Exception(CREATION_INCORRECT_VALUES);
		if (a.row(2) != splitArray<uint8_t>(uint8_correct_values, 6, 8)) throw Exception(CREATION_INCORRECT_VALUES);
        if (c.row(0) != splitArray<F32C1>(float_lotta_values, 0, 499999))
        if (c.row(1) != splitArray<F32C1>(float_lotta_values, 500000, 999999));
        if (e.row(0) != splitArray<U16C1>(uint16_lotta_values, 0, 3161));
        if (e.row(1) != splitArray<U16C1>(uint16_lotta_values, 3162, 6323));

		cout << " - Pass" << endl;
		cout << "\tColumns Integrity";

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

		cout << " - Pass" << endl;
		cout << "\tExtraction";


		if (a.at(0, 2) != 3) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
		if (a.at(1, 1) != 5) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
		if (a.at(2, 0) != 7) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 

        for(size_t i{1}; i <= 199999; i++) {
            for(size_t f{1}; f <= 499; f++) {
                //cout << d.at(i, f);
                //cout << " - " << double_lotta_values[i * f] << endl;
               //if(!e.at(i, f) == uint16_lotta_values[i * f]) throw Exception(CREATION_INCORRECT_VALUES_SPECIFIC); 
            }
        }

		cout << " - Pass" << endl;

	} catch(const Exception exception) {
		cout << "Fail" << endl;
		return 0;

	} catch(MatrixException exception) {
		cout << "Fail " << endl;
		return 0;
	}

	cout << "\nTesting :  Matrix Creation with Incorrect Values" << endl;



    cout << "\tToo Many Values";
	try {
		Matrix<U8C1> a (2, 3, uint8_incorrect_too_many_values);
	} catch(MatrixException exception) {
		if( exception.error_ != MatrixError::INPUT_TOO_MANY_VALUES ) {
			cout << "Fail" << endl;
		}
        cout << " - Pass" << endl;
	} catch (...) {
        cout << "Unexpected Error thrown. Stoping." << endl;
    }




    cout << "\tNot Enough Values";
	try {
		Matrix<U8C1> a (3, 3, uint8_incorrect_not_enough_values);

	} catch(MatrixException exception) {
		if( exception.error_ != MatrixError::INPUT_NOT_ENOUGH_VALUES ) {
			cout << "Fail" << endl;
		}
        cout << " - Pass" << endl;
	}


    return true;
}
