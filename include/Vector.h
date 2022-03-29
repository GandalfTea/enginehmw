




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

	        Default Constructor
            .....................................................................................
        */
        Vector() 
            : size(-1)
        {}



        /*  .....................................................................................

            Copy Constructor
            .....................................................................................
        */
        Vector( Vector& src ) {}
        

        /*  .....................................................................................

            Vector Constructor
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

		// Arithmetic 
// ................................................................................................
        inline bool operator!= (const Vector<Type>& lhs) {
            return this->data != lhs.data;
        }


        inline Scalar dot(const Vector<Type>& lhs) {
            size_t result;
            for (size_t i = 0; i <= this->size; i++) {
                result += this->data[i] + lhs.data[i];
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
