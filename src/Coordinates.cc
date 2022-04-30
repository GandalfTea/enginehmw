
#include <Coordinates.h>


namespace MEGA {


// Quanternions

Quaternion::Quaternion( float s, float v1, float v2, float v3 ) 
    : s(s), v1(v1), v2(v2), v3(v3)
{
  m = sqrt( pow(s, 2) + pow(v1, 2) + pow(v2, 2) + pow(v3, 2) );
}

// Euler -> Quaternion
Quaternion::Quaternion( EulerAngle src ) {

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


// Rotation Matrix -> Quaternion
Quaternion::Quaternion( Matrix<F64C1> src ) {
  //assert( src.t() * src == Matrix<F32C1>.eye(4); 
  //assert( src.det = 1 );

  // TODO: Check for division by zero, sqrt of negative num and de-orthogonalised matrix
  float trace = src.at(0, 0) + src.at(1, 1) + src.at(2, 2);

  if(trace > 0) {
    float S  = sqrt( 1.0f + trace) * 2;
    s = 0.25 *S;
    v1 = ( src.at(2, 1) - src.at(1, 2) ) / S;
    v2 = ( src.at(0, 2) - src.at(2, 0) ) / S;
    v3 = ( src.at(1, 0) - src.at(0, 1) ) / S;
  } else if( (src.at(0, 0) > src.at(1, 1)) & (src.at(0, 0) > src.at(2, 2))) {
    float S = sqrt(1.0 + src.at(0, 0) - src.at(1, 1) - src.at(2, 2)) * 2;
    s = ( src.at(2, 1) - src.at(1, 2) ) / S;
    v1 = 0.25 *S;
    v2 = ( src.at(0, 1) - src.at(1, 0) ) / S;
    v3 = ( src.at(0, 2) - src.at(2, 0) ) / S;
  } else if( src.at(1, 1) > src.at(2, 2)) {
    float S = sqrt(1.0 + src.at(1, 1) - src.at(0, 0) - src.at(2, 2)) * 2;
    s = ( src.at(0, 2) - src.at(2, 0) ) / S;
    v1 = ( src.at(0, 1) - src.at(1, 0) ) / S;
    v2 = 0.25 *S;
    v3 = ( src.at(1, 2) - src.at(2, 1) ) / S;
  } else {
    float S = sqrt(1.0 + src.at(2, 2) - src.at(0, 0) - src.at(1, 1)) * 2;
    s = ( src.at(1, 0) - src.at(0, 1) ) / S;
    v1 = ( src.at(0, 2) - src.at(2, 0) ) / S;
    v2 = ( src.at(1, 2) - src.at(2, 1) ) / S;
    v3 = 0.25 *S;
  }
  m = sqrt( pow(s, 2) + pow(v1, 2) + pow(v2, 2) + pow(v3, 2) );
}


// Quaternion -> Rotation Matrix
Matrix<F64C1> Quaternion::toRotMat() {
  std::vector<double> R_data;
  R_data.push_back( 1 - 2*( v2  * v2  + v3 * v3));  // wrong
  R_data.push_back( 2*( v1 * v2 -  s * v3));      // wrong
  R_data.push_back( 2*( v1 * v3 +  s * v2));
      
  R_data.push_back( 2*( v1 * v2 + s  * v3));      // wrong
  R_data.push_back( 2*( s  * s  + v2 * v2) - 1);  
  R_data.push_back( 2*( v2 * v3 - s  * v1));      // wrong

  R_data.push_back( 2*( v1 * v3 - s  * v2));
  R_data.push_back( 2*( v2 * v3 + s  * v1));      // wrong
  R_data.push_back( 2*( s  * s  + v3 * v3) - 1);  //wrong

  Matrix<F64C1> R (3, 3, R_data);
  return R;
}

float Quaternion::innerProduct( Quaternion lhs, Quaternion rhs ) {
  return lhs.s*rhs.s + lhs.v1*rhs.v1 + lhs.v2*rhs.v2 + lhs.v3*rhs.v3;
}

Quaternion::~Quaternion() {}



// EULER ANGLE


EulerAngle::EulerAngle( Quaternion src ) {
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

// Rotation Matrix -> Euler Angles
EulerAngle::EulerAngle( Matrix<F64C1> src ) {
  if( src.at(2, 0) != 1 || src.at(2, 0) != -1) {
    theta = -asin(src.at(2,0));
    //theta2 = M_PI - theta;
    phi  = atan2( src.at(2, 1) / cos(theta),  src.at(2, 2) / cos(theta));
    //phi2 = atan2( src.at(3, 2) / cos(theta2), src.at(3, 3) / cos(theta2));
    psi  = atan2( src.at(1, 0) / cos(theta),  src.at(1, 0) / cos(theta));
    //psi2 = atan2( arc.at(2, 1) / cos(theta2), src.at(1, 1) / cos(theta2));

  } else {
    psi = 0;
    if( src.at(2, 0) == -1) {
      theta = M_PI / 2;
      phi = psi + atan2( src.at(0, 1), src.at(0, 2) );
    } else {
      theta = -M_PI / 2;
      phi = -psi + atan2( -src.at(0, 1), -src.at(0, 2) );
    }
  }
}

Matrix<F64C1> EulerAngle::toRotMat() {

  std::vector<double> R_x { 1,    0,         0,
                            0, cos(phi), -sin(phi),
                            0, sin(phi),  cos(phi) };      

  std::vector<double> R_y { cos(theta), 0, sin(theta),
                                 0,     1,     0,
                           -sin(theta), 0, cos(theta) };

  std::vector<double> R_z { cos(psi), -sin(psi), 0,
                            sin(psi),  cos(psi), 0,
                               0,         0,     1 };

  Matrix<F64C1> Rx(3, 3, R_x);
  Matrix<F64C1> Ry(3, 3, R_y);
  Matrix<F64C1> Rz(3, 3, R_z);
  Matrix<F64C1> temp = Ry.dot(Rz);
  Matrix<F64C1> R = Rx.dot(temp);
  return R;
}

EulerAngle::~EulerAngle() {}



}// namespace
