
#ifndef MEGA_COORDS_CORE
#define MEGA_COORDS_CORE

#define _USE_MATH_DEFINES // import constants
#include <cmath>

#define radians_to_degrees( x ) x * (180 / M_PI)
#define degrees_to_radians( x ) x * (M_PI / 180)

#ifndef USING_EIGEN_MAT
#include <Matrix>
#endif

namespace MEGA {

class Quaternion {
  public:
    float s;
    float v1, v2, v3;
    float m; //magnitude

    Quaternion();
    ~Quaternion();

    Quaternion( float s, float v1, float v2, float v3 ) 
        : s(s), v1(v1), v2(v2), v3(v3)
    {
      m = sqrt( pow(s, 2) + pow(v1, 2) + pow(v2, 2) + pow(v3, 2) );
    }

    // Euler -> Quaternion
    Quaternion( EulerAngle src ) {

      float x = src.phi/2;
      float y = src.theta/2;
      float z = src.psi/2;

      s  = sin(x) * cos(y) * cos(z) - 
           cos(x) * sin(y) * sin(z);
      v1 = cos(x) * sin(y) * cos(z) +
           sin(x) * cos(y) * sin(z);
      v2 = cos(x) * cos(y) * sin(z) -
           sin(x) * sin(y) * cos(z);
      v3 = cos(x) * cos(y) * cos(z) +
           sin(x) * sin(y) * sin(z);

      m = sqrt( pow(s, 2) + pow(v1, 2) + pow(v2, 2) + pow(v3, 2) );
    }

#ifndef USING_EIGEN_MAT

    // Rotation Matrix -> Quaternion
    Quaternion( Matrix src ) {
      //assert( src.t() * src == Matrix<F32C1>.eye(4); 
      //assert( src.det = 1 );

      s  = sqrt( 1.0f + src.at(0, 0) + src.at(1, 1) + src.at(2, 2) ) / 2;
      v1 = ( src.at(2, 1) - src.at(1, 2) ) / 4*s;
      v2 = ( src.at(0, 2) - src.at(2, 0) ) / 4*s;
      v3 = ( src.at(1, 0) - src,at(0, 1) ) / 4*s;
      m = sqrt( pow(s, 2) + pow(v1, 2) + pow(v2, 2) + pow(v3, 2) );
    }
#else
    // PS: I am not yet using Eigen, so coursework is still fine 
    Quaternion( Eigen::MatrixXd src );
#endif

    // Quaternion -> Rotation Matrix
    Matrix toRotMat() {
      double R_data [9];
      R_data[0] = 2*( s  * s  + v1 * v1) - 1;
      R_data[1] = 2*( v1 * v2 -  s * v3);
      R_data[2] = 2*( v1 * v3 +  s * v2);

      R_data[3] = 2*( v1 * v2 + s  * v3);
      R_data[4] = 2*( s  * s  + v2 * v2) - 1;
      R_data[5] = 2*( v2 * v3 - s  * v1);

      R_data[6] = 2*( v1 * v3 - s  * v2);
      R_data[7] = 2*( v2 * v3 + s  * v1);
      R_data[8] = 2*( s  * s  + v3 * v3) - 1;

      Matrix<F64C1> R (3, 3, R_data);
      return R;
    }


    // Helpers
    Quaternion invert( Quaternion src );
    float normalize( Quaternion src );


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

    Quaternion operator+ ( Quaternion rhs ) { arithmetic( this, rhs, [](float x, float y){ return x+y; }); }
    Quaternion operator- ( Quaternion rhs ) { arithmetic( this, rhs, [](float x, float y){ return x-y; }); }
    Quaternion operator* ( Quaternion rhs ) { arithmetic( this, rhs, [](float x, float y){ return x*y; }); }
    Quaternion operator/ ( Quaternion rhs ) { arithmetic( this, rhs, [](float x, float y){ return x/y; }); }

    static float innerProduct( Quaternion lhs, Quaternion rhs ) {
      return lhs.s*rhs.s + lhs.v1*rhs.v1 + lhs.v2*rhs.v2 + lhs.v3*rhs.v3;
    }
};

template <typename T>
inline std::ostream& operator<< ( std::ostream& outs, Quaternion& q ) {
  std::string repr = "\n[ ";
  repr += std::to_string(s) + " ";
  repr += std::to_string(v1) + " ";
  repr += std::to_string(v2) + " ";
  repr += std::to_string(v3) + " ]";
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
        : phi(phi, theta(theta), psi(psi)) {};

    // Quaternion -> Euler Angles
    EulerAngle( Quaternion src ) {
      auto t0 = 2.0 * ( src.s * src.v1 + src.v1 * src.v3 );
      auto t1 = 1.0 - 2.0 * ( pow(src.s, 2) + pow(src.v1, 2));   
      phi = atan2(t0, t1);

      auto t2 = 2.0 * ( src.s * src.v1 - src.v1 * src.v3 );
      (t2 > 1.0)  ? t2= 1.0 : t2;
      (t2 < -1.0) ? t2=-1.0 : t2;
      theta = asin(t2);

      auto t3 = 2.0 * ( src.s * src.v1 + src.v1 * src.v3 );
      auto t4 = 1.0 - 2.0 * ( pow(src.v1, 2) + pow(src.v2, 2));
      psi = atan2(t3, t4);
    }

#ifndef USING_EIGEN_MAT

    // Rotation Matrix -> Euler Angles
    EulerAngle( Matrix src ) {
      if( src.at(3, 1) != 1 || src.at(3, 1) != -1) {
        theta = -asin(src.at(3,1));
        theta2 = M_PI - theta;
        phi  = atan2( src.at(3, 2) / cos(theta),  src.at(3, 3) / cos(theta));
        phi2 = atan2( src.at(3, 2) / cos(theta2), src.at(3, 3) / cos(theta2));
        psi  = atan2( arc.at(2, 1) / cos(theta),  src.at(1, 1) / cos(theta));
        psi2 = atan2( arc.at(2, 1) / cos(theta2), src.at(1, 1) / cos(theta2));

      } else {
        psi = 0;
        if( src.at(3, 1) == -1) {
          theta = M_PI / 2;
          phi = psi + atan2( src.at(1, 2), src.at(1, 3) );
        } else {
          theta = -M_PI / 2;
          phi = -psi + atan2( -src.at(1, 2), -src.at(1, 3) );
        }
      }
    }

    // Euler Angles -> Rotation Matrix
    Matrix toRotMat() {

      float R_x [] = { 1,    0,         0,
                       0, cos(phi), -sin(phi),
                       0, sin(phi),  cos(phi) };      

      float R_y [] = { cos(theta), 0, sin(theta),
                            0,     1,     0,
                      -sin(theta), 0, cos(theta) };

      float R_z [] = { cos(psi), -sin(psi), 0,
                       sin(psi),  cos(psi), 0,
                          0,         0,     1 };

      Matrix<F64C1> Rx(3, 3, R_x);
      Matrix<F64C1> Ry(3, 3, R_y);
      Matrix<F64C1> Rz(3, 3, R_z);
      Matrix<F64C1> R = Rx * Ry * Rz;
      return R;
    }

#else
    // Not using Eigen yet.
    EulerAngle( Eigen::MatrixXd src );
#endif
};

template <typename T>
inline std::ostream& operator<< ( std::ostream& outs, EulerAngle a ) {
  std::string repr = "\n[ ";
  repr += std::to_string(phi) + " ";
  repr += std::to_string(theta) + " ";
  repr += std::to_string(psi) + " ]";
  return outs << repr;
}

} // namespace
#endif
