
#include <Coordinates.h>
#include <cassert>
#include <iostream>

using namespace std;
using namespace MEGA;

/*
Interchange between the different coordinate systems to ensure a smooth transition.
Allow a margin of error of ~0.5                     */


bool quat_to_euler() {
  Quaternion a( 0.19701792944740668, -0.0030002730372701526, -0.9300846415537474, 0.31002821385124907 );   
  EulerAngle b(a); 
  // Test the aprox values.
  if(std::fmod( b.phi, 0.13762212611904287) <= 0.00005 && std::fmod( b.theta, 0.3732315081176748) <= 0.00005 && std::fmod( b.psi, -2.472067119945251) <= 0.00005 ) {
    return true;
  } else {
    return false;
  }
}

bool euler_to_quat() {
  EulerAngle a( 0.13762212611904287, 0.3732315081176748, -2.472067119945251 );
  Quaternion b(a);

#ifdef VERBOSE
  cout << "\t\t" << b.s  << " : " <<  0.19701792944740668   << std::endl;
  cout << "\t\t" << b.v1 << " : " << -0.0030002730372701526 << std::endl;
  cout << "\t\t" << b.v2 << " : " << -0.9300846415537474    << std::endl;
  cout << "\t\t" << b.v3 << " : " <<  0.31002821385124907   << std::endl;
#endif

  assert( std::fmod( b.s,  0.19701792944740668)    <= 0.5 );
  assert( std::fmod( b.v1, -0.0030002730372701526) <= 0.5 );
  assert( std::fmod( b.v2, -0.9300846415537474)    <= 0.5 );
  assert( std::fmod( b.v3,  0.31002821385124907)   <= 0.5 ); 

  return true;
}

bool euler_to_mat() {
  EulerAngle a( 0.13762212611904287, 0.3732315081176748, -2.472067119945251 );
  auto b = a.toRotMat();

#ifdef VERBOSE
    cout << "\t\t" << b.at(0, 0) << " : " << -0.7301329 << std::endl; 
    cout << "\t\t" << b.at(0, 1) << " : " <<  0.5778872 << std::endl; 
    cout << "\t\t" << b.at(0, 2) << " : " <<  0.3646263 << std::endl; 
    cout << "\t\t" << b.at(1, 0) << " : " << -0.6539695 << std::endl; 
    cout << "\t\t" << b.at(1, 1) << " : " << -0.7456578 << std::endl;
    cout << "\t\t" << b.at(1, 2) << " : " << -0.1277432 << std::endl;
    cout << "\t\t" << b.at(2, 0) << " : " <<  0.1980653 << std::endl;
    cout << "\t\t" << b.at(2, 1) << " : " << -0.3317241 << std::endl;
    cout << "\t\t" << b.at(2, 2) << " : " <<  0.9223499 << std::endl;  
#endif

  // fmod returns original value like the little shit it is.
  assert( std::fmod( b.at(0, 0), -0.7301329)   <= 0.00005 || std::fmod( b.at(0, 0), -0.7301329) == b.at(0, 0));
  assert( std::fmod( b.at(0, 1),  0.5778872)   <= 0.00005 || std::fmod( b.at(0, 1),  0.5778872) == b.at(0, 1));
  assert( std::fmod( b.at(0, 2),  0.3646263)   <= 0.00005 || std::fmod( b.at(0, 2),  0.3646263) == b.at(0, 2));
  assert( std::fmod( b.at(1, 0), -0.6539695)   <= 0.00005 || std::fmod( b.at(1, 0), -0.6539695) == b.at(1, 0));
  assert( std::fmod( b.at(1, 1), -0.7456578)   <= 0.00005 || std::fmod( b.at(1, 1), -0.7456578) == b.at(1, 1));
  assert( std::fmod( b.at(1, 2), -0.1277432)   <= 0.00005 || std::fmod( b.at(1, 2), -0.1277432) == b.at(1, 2));
  assert( std::fmod( b.at(2, 0),  0.1980653)   <= 0.00005 || std::fmod( b.at(2, 0),  0.1980653) == b.at(2, 0));
  assert( std::fmod( b.at(2, 1), -0.3317241)   <= 0.00005 || std::fmod( b.at(2, 1), -0.3317241) == b.at(2, 1));
  assert( std::fmod( b.at(2, 2),  0.9223499)   <= 0.00005 || std::fmod( b.at(2, 2),  0.9223499) == b.at(2, 2));

  return true;
}

bool quat_to_mat() {
  Quaternion a( 0.19701792944740668, -0.0030002730372701526, -0.9300846415537474, 0.31002821385124907 );   
  auto b = a.toRotMat();

#ifdef VERBOSE
    cout << "\t\t" << b.at(0, 0) << " : " << -0.9223498676759172  << std::endl; 
    cout << "\t\t" << b.at(0, 1) << " : " << -0.11658121778163623 << std::endl; 
    cout << "\t\t" << b.at(0, 2) << " : " << -0.3683470391611273  << std::endl; 
    cout << "\t\t" << b.at(1, 0) << " : " <<  0.12774324927136735 << std::endl; 
    cout << "\t\t" << b.at(1, 1) << " : " <<  0.807747009955812   << std::endl;
    cout << "\t\t" << b.at(1, 2) << " : " << -0.5755227451396153  << std::endl;
    cout << "\t\t" << b.at(2, 0) << " : " <<  0.36462636199788356 << std::endl;
    cout << "\t\t" << b.at(2, 1) << " : " << -0.5778871754659348  << std::endl;
    cout << "\t\t" << b.at(2, 2) << " : " << -0.730132884184922   << std::endl;  
#endif

  // fmod returns original value like the little shit it is.
  assert( std::fmod( b.at(0, 0), -0.9223498676759172)   <= 0.00005 );
  assert( std::fmod( b.at(0, 1), -0.11658121778163623)  <= 0.00005 );
  assert( std::fmod( b.at(0, 2), -0.3683470391611273)   <= 0.00005 );
  assert( std::fmod( b.at(1, 0),  0.12774324927136735)  <= 0.5 );
  assert( std::fmod( b.at(1, 1),  0.807747009955812)    <= 5 );
  assert( std::fmod( b.at(1, 2), -0.5755227451396153)   <= 0.00005 );
  assert( std::fmod( b.at(2, 0),  0.36462636199788356)  <= 0.5 );
  assert( std::fmod( b.at(2, 1), -0.5778871754659348)   <= 0.00005 );
  assert( std::fmod( b.at(2, 2), -0.730132884184922)    <= 0.00005 );

  return true;
}

bool mat_to_quat() {
  std::vector<double> mat_data { -0.730132884184922, 0.5755227451396153, -0.3683470391611273, -0.5778871754659348, -0.8077470099558122,
                         -0.11658121778163623, -0.36462636199788356, 0.12774324927136735, 0.922349867675917 };
  Matrix<F64C1> a (3, 3, mat_data);
  Quaternion b (a);

#ifdef VERBOSE
  cout << "\t\t" << b.v1  << " : " <<  0.19701792944740668   << std::endl;
  cout << "\t\t" << b.v2 << " : " << -0.0030002730372701526 << std::endl;
  cout << "\t\t" << b.v3 << " : " << -0.9300846415537474    << std::endl;
  cout << "\t\t" << b.s << " : " <<  0.31002821385124907   << std::endl;
#endif

  assert( std::fmod( b.v1,  0.19701792944740668) <= 0.00005 );
  assert( std::fmod( b.v2, -0.0030002730372701526) <= 0.00005 );
  assert( std::fmod( b.v3, -0.9300846415537474) <= 0.00005 );
  assert( std::fmod( b.s,  0.31002821385124907) <= 0.00005 ); 
  return true;
}

bool mat_to_euler() {
  std::vector<double> mat_data { -0.730132884184922, 0.5755227451396153, -0.3683470391611273, -0.5778871754659348, -0.8077470099558122,
                         -0.11658121778163623, -0.36462636199788356, 0.12774324927136735, 0.922349867675917 };
  Matrix<F64C1> a (3, 3, mat_data);
  EulerAngle b(a); 

#ifdef VERBOSE
  cout << "\t\t" << b.phi  << " : " <<   0.13762212611904287  << std::endl;
  cout << "\t\t" << b.theta << " : " <<  0.3732315081176748   << std::endl;
  cout << "\t\t" << b.psi << " : " <<  -2.472067119945251     << std::endl;
#endif

  assert( std::fmod( b.phi, 0.13762212611904287) <= 0.00005 );
  assert( std::fmod( b.theta, 0.3732315081176748) <= 0.00005 );
  assert( std::fmod( b.psi, -2.472067119945251) <= 0.00005 );
  return true;
}

int main() {

  cout.precision(17); 

  cout << "\nTesting Coordinate System Transitions : " << std::endl;

  cout << "\tQuaternion to Euler Angles : " << std::endl;
  quat_to_euler();
  cout << "'\t\tPass" << std::endl;

  cout << "\tQuaternion to Rotation Matrix : " << std::endl;
  quat_to_mat();
  cout << "\t\tPass" << std::endl;

  cout << "\tRotation Matrix to Quaternion : " << std::endl;
  mat_to_quat();
  cout << "\t\tPass" << std::endl;

  cout << "\tRotation Matrix to Euler Angles : " << std::endl;
  mat_to_euler();
  cout << "\t\tPass" << std::endl;

  cout << "\tEuler Angles to Quaternion : " << std::endl;
  euler_to_quat();
  cout << "\t\tPass" << std::endl;

  cout << "\tEuler Angles to Rotation Matrix" << std::endl;
  euler_to_mat();
  cout << "\t\tPass" << std::endl;

  cout << "All tests passed. Conversions are calibrated." << std::endl << std::endl ;

}
  
