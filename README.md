&nbsp;

### About
Barely runnable game engine made for MEGA.      
Optimised for ~ 10 million vertices, ~25MB.

&nbsp;

### TODO:
* Perlin noise random terrain generation
* First person player movement
* Object Collision
* Gun with bullets that follow the cursor?

&nbsp;

### Docs

&nbsp;

#### Matrix

Matrix types follow the format: 
```
TYPE (U)nsigned (S)igned (F)loat) + DEPTH (8, 16, 32, 64) + CHANNELS (1, 2, 3)

U8C1 - unsigned integer of 8 bit depth with one channel - uint_t
F32C3 - float value of 32 bit depth with 3 channels.
```

```c++
// Default Constructor
Matrix<F32C1> a;

// Array Constructor
F64C1 array = { 0.3, 0.8451, 0.6598745, 4.126654, 0.25486, 0.789215 };
Matrix<F64C1> b (2, 3, array); // The size of array should not be bigger or smaller than rows*cols.

// Fill Constructor
Matrix<U32C1> c (5, 5, (U32C1)value); // It is recomanded to cast the value into the right type.

// Copy Constructor
Matrix<U8C1> d (5, 5, (U8C1)69);
Matrix<U8C1> e (d);

// Identity Constructor
Matrix<F32C1> f;
f.eye(10)       // fills f with an identity matrix of size 10
```

Getting values
```c++
a.row(index);    // returns the row at the specific index
a.col(index);    // returns the col at the specific index
a.at(x, y);      // returns the specific value at column x and row y
a.data;          // returns all the matrix data in the expected type;
```

Helpers
```c++
a.cols;      // returns the number of columns in a
a.rows;      // returns the number of rows in a
a.det();     //returns the determinant
a.inverse(); // returns the inverse of matrix a 
std::cout << a << std::endl;    // prints the matrix to the console in a readable format
```

