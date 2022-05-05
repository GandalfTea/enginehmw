




// THIS CLASS HEADER CONTAINS THE FULL IMPLEMENTATION 
// BECAUSE IT IS A TEMPLATE CLASS 




#ifndef VECTOR
#define VECTOR

#include <Types.h>

#include <typeinfo>
#include <iostream>
#include <string>
#include <cmath>     // this is included just for a sqrt function. Implement yourself

namespace MEGA {




// VECTOR BASE TEMPLATE CLASS
// .....................................................................................

template <class Type>
class Vector {
    public:

        // MEMBER VARIABLES
        std::vector<Type> data;           // public data in vector format
		uint16_t size;                    // size of vectpr
        uint16_t L;                       // length of vector;
		uchar step = 0x00;                // step between elements (bit depth of type)
		mutable MEGAType type;            // for now use matrix types
        // Maybe add T here



        /*  .....................................................................................

	        Default Constructors
            .....................................................................................
        */
        Vector() 
            : size(-1)
        {}

        Vector( Type a, Type b ) {
            this->data.push_back(a);
            this->data.push_back(b);
            this->size = this->data.size();
            this->L = sqrt( a*a + b*b );
        }

        Vector( Type a, Type b, Type c ) {
            this->data.push_back(a);
            this->data.push_back(b);
            this->data.push_back(c);
            this->size = this->data.size();
            this->L = sqrt( a*a + b*b + c*c );
        }

        Vector( Type a, Type b, Type c, Type d ) {
            this->data.push_back(a);
            this->data.push_back(b);
            this->data.push_back(c);
            this->data.push_back(d);
            this->size = this->data.size();
            this->L = sqrt( a*a + b*b + c*c + d*d );
        }



        /*  .....................................................................................

            Copy Constructor
            .....................................................................................
        */
        Vector( Vector& src ) {}
        

        /*  .....................................................................................

            Vector Constructor with Array initializer
            .....................................................................................
        */
        Vector( size_t size, std::vector<Type> vals ) {
            if(vals.size() > size || vals.size() < size) throw 3;
            this->Data = vals;
            this->data = vals;
            this->size = size;
            setType();

            // calculate the length of the vector
            float length{0};
            for( auto i : vals ) {
                length += i*i;
            }
            this->L = sqrt(length);
        }

        // TODO: VECTOR CONSTRUCTOR WITH VARIADIC FUNCTION


		// Helpers
// ................................................................................................

        /*  
            Transpose Vector 
            .....................................................................................
        */
        Vector<std::vector<Type>> T() {
            Vector<std::vector<Type>> ret;
            for(auto i : this->data) {
                std::vector<Type> buffer {i};
                ret.push_back(buffer);
            }
            return ret;
        }

        /*  
            Add values to vector (vertical and horizontal) 
            .....................................................................................
        */
        void push_back(Type value) {
            this->Data.push_back(value);
            this->data.push_back(value);
            this->size++;
        }

        void push_back(std::vector<Type> value) {
            this->Data.push_back(value);
            this->data.push_back(value);
            this->size++;
        }

        Type& operator[](int idx) {
            return this->data[idx];
        }

        void normalize() {
            for( auto i : data) {
                i /= L;
            }
        }

		// Arithmetic 
// ................................................................................................
        inline bool operator!= (const Vector<Type>& rhs) {
            return this->data != rhs.data;
        }

        inline Vector<Type> operator- (const Vector<Type> rhs) {
            if( this->size != rhs.size || this->size != 3 ) std::cout << "Wrong size of vector" << std::endl; // raise exception
            Vector<Type> ret;
            for( size_t i{}; i < size; i++) {
                ret.data.push_back( this->data[i] - rhs.data[i]); 
            }
        }

        inline Vector<Type> operator% (const Vector<Type>& rhs) {
            if( this->size != rhs.size || this->size != 3 ) std::cout << "Wrong size of vector" << std::endl; // raise exception
            float x = this->data[1] * rhs.data[2] - this->data[2]*rhs.data[1];
            float y = this->data[2] * rhs.data[0] - this->data[0]*rhs.data[2];
            float z = this->data[0] * rhs.data[1] - this->data[1]*rhs.data[0];
            return Vector<Type>(x, y, z);
        }


        inline double dot(const Vector<Type>& lhs) {
            double result{};
            for (size_t i = 0; i < this->size; i++) {
                result += this->data[i] * lhs.data[i];
            }
            return result;
        }




    protected:
        std::vector<Type> Data;

    private:

        void setType() {
            // TODO: Check for complex types
            if (*typeid(Type).name() == 'j') this->type = MEGA_U32C1;
            else if (*typeid(Type).name() == 't') this->type = MEGA_U16C1;
            else if (*typeid(Type).name() == 'h') this->type = MEGA_U8C1;
            else if (*typeid(Type).name() == 'i') this->type = MEGA_S32C1;
            else if (*typeid(Type).name() == 's') this->type = MEGA_S16C1;
            else if (*typeid(Type).name() == 'a') this->type = MEGA_S8C1;
            else if (*typeid(Type).name() == 'f') this->type = MEGA_U32C1;
            else if (*typeid(Type).name() == 'd') this->type = MEGA_F64C1;
        }
};

} // MEGA namespace


/*  .....................................................................................

    Display Vector as string in the console with operator<<.

    We define a new to_string() function in the std namespace in order to represent
    vertical vectors corectly in the following function. 
    .....................................................................................
*/
namespace std {

template <typename T>
inline std::string to_string(std::vector<T> src) {
    static int idx = 0;
    std::string repr = "";
    for( auto i : src ) {
        (idx != 0) ? repr += "\n\t  " : repr+= "";
        repr += "[ ";
        repr+= to_string(i) + " ]";
        ++idx;
    }
    return repr;
}

} // std namespace

namespace MEGA {

template <typename T>
inline std::ostream& operator<< (std::ostream& outs, const Vector<T>& lhs) {
	std::string repr = "\n\t[ ";
    for (auto i : lhs.data) {
        repr += std::to_string(i) + ", ";
    }
    repr += " ]\n";
	return outs <<  repr;
}

template <typename T>
inline std::ostream& operator<< (std::ostream& outs, Vector<T>& lhs) {
	std::string repr = "\n\t[ ";
    for (auto i : lhs.data) {
        repr += std::to_string(i) + ", ";
    }
    repr += " ]\n";
	return outs <<  repr;
}
/*  .....................................................................................

    Vector Arithmetic
    .....................................................................................
*/

//inline bool operator== (Vector& rhs, Vector& lhs) {}


} // MEGA  namespace
#endif
