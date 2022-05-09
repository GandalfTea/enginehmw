
#ifndef MEGA_COORDS_CORE
#define MEGA_COORDS_CORE

#define _USE_MATH_DEFINES // import constants
#include <cmath>

#define radians_to_degrees( x ) x * (180 / M_PI)
#define degrees_to_radians( x ) x * (M_PI / 180)

#ifndef USING_EIGEN_MAT
#include <Matrix.h>
#endif

namespace MEGA {

class EulerAngle;


class Quaternion {
  public:
    float s;
    float v1, v2, v3;
    float m; //magnitude

    Quaternion();
    ~Quaternion();
    Quaternion( float s, float v1, float v2, float v3 ); 
    Quaternion( EulerAngle src );      // Euler -> Quaternion

#ifndef USING_EIGEN_MAT

    Quaternion( Matrix<F64C1> src );   // Rotation Matrix -> Quaternion
    Matrix<F64C1> toRotMat();          // Quaternion -> Rotation Matrix

#else
    // PS: I am not yet using Eigen, so coursework is still fine 
    Quaternion( Eigen::MatrixXd src );
#endif

    // Helpers
    Quaternion invert( Quaternion src );
    float normalize( Quaternion src );
    static float innerProduct( Quaternion lhs, Quaternion rhs );


    // Arithmetic
    // got too lazy to write all of them, so made a parent function
    // this can also be used if there is any other function on the vars, just pass a lambda
    template <typename Func>
    Quaternion arithmetic( Quaternion lhs, Quaternion rhs, Func function) {
      Quaternion ret;
      ret.s  = function( lhs.s,  rhs.s);
      ret.v1 = function( lhs.v1, rhs.v1);
      ret.v2 = function( lhs.v2, rhs.v2);
      ret.v3 = function( lhs.v3, rhs.v3);
      return ret;
    }

    Quaternion operator+ ( Quaternion rhs ) { return arithmetic( *this, rhs, [](float x, float y){ return x+y; }); }
    Quaternion operator- ( Quaternion rhs ) { return arithmetic( *this, rhs, [](float x, float y){ return x-y; }); }
    Quaternion operator* ( Quaternion rhs ) { return arithmetic( *this, rhs, [](float x, float y){ return x*y; }); }
    Quaternion operator/ ( Quaternion rhs ) { return arithmetic( *this, rhs, [](float x, float y){ return x/y; }); }
};

template <typename T>
inline std::ostream& operator<< ( std::ostream& outs, Quaternion& q ) {
  std::string repr = "\n[ ";
  repr += std::to_string(q.s) + " ";
  repr += std::to_string(q.v1) + " ";
  repr += std::to_string(q.v2) + " ";
  repr += std::to_string(q.v3) + " ]";
  return outs << repr;
}



class EulerAngle {

  public:
    double phi;
    double theta;
    double psi;

    EulerAngle();
    ~EulerAngle();

    EulerAngle( double phi, double theta, double psi )
        : phi(phi), theta(theta), psi(psi) {};
    EulerAngle( Quaternion src );    // Quaternion -> Euler Angles
    Matrix<F64C1> toRotMat();     // Euler Angles -> Rotation Matrix

#ifndef USING_EIGEN_MAT
    EulerAngle( Matrix<F64C1> src );   // Rotation Matrix -> Euler Angles
#else
    // Not using Eigen yet.
    EulerAngle( Eigen::MatrixXd src );
#endif
};

template <typename T>
inline std::ostream& operator<< ( std::ostream& outs, EulerAngle a ) {
  std::string repr = "\n[ ";
  repr += std::to_string(a.phi) + " ";
  repr += std::to_string(a.theta) + " ";
  repr += std::to_string(a.psi) + " ]";
  return outs << repr;
}

} // namespace
#endif
