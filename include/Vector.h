




// THIS CLASS HEADER CONTAINS THE FULL IMPLEMENTATION 
// BECAUSE IT IS A TEMPLATE CLASS 




#ifndef VECTOR
#define VECTOR

#include <Types.h>
#include <typeinfo>

namespace MEGA {



template <class Type>
class Vector {
    public:

        // MEMBER VARIABLES
        std::vector<Type> data;           // public data in vector format
		uint16_t size;                    // size of vectpr
		uchar step = 0x00;                // step between elements (bit depth of type)
		mutable MEGAType type;            // for now use matrix types
        // Maybe add T here

        Vector() {}
        Vector( Vector& src ) {}
        
        Vector( size_t size, std::vector<Type> vals ) {
            if(vals.size() > size || vals.size() < size) throw 0;
            this->Data = vals;
            this->data = vals;
            this->size = size;
            setType();


            
        }

        // Transpose Vector
        Vector<std::vector<Type>> T() {
            Vector<std::vector<Type>> ret;
            for(auto i : this->data) {
                std::vector<Type> buffer {i};
                ret.push_back(buffer);
            }
            return ret;
        }

        // Add values to the vector
        void push_back(Type value) {
            this->Data.push_back(value);
            this->data.push_back(value);
        }

        void push_back(std::vector<Type> value) {
            this->Data.push_back(value);
            this->data.push_back(value);
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

// Display Vector as string in console. 
// the face that this worked is insane

}


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

}

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



} // namespace
#endif
