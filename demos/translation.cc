#include <Matrix.h>

using namespace std;

int main() {
   
    const vector<float> cube = { 0.500000, -0.500000, -0.500000, 0.500000, -0.500000, 0.500000, -0.500000, -0.500000, 0.500000, -0.500000, -0.500000, -0.500000, 0.500000, 0.500000, -0.500000, 0.500000, 0.500000, 0.500000, -0.500000, 0.500000, 0.500000, -0.500000, 0.500000, -0.500000 };

    int idx = 1;
    vector<float> cubeHomogenous = {};

    for(auto i : cube) {
        cubeHomogenous.push_back(i);
        if(idx % 3 == 0) cubeHomogenous.push_back(1);
        idx++;
    }

    MEGA::Matrix<MEGA::F32C1> cubeVert (4, 8, cubeHomogenous);

    MEGA::Matrix<MEGA::F32C1> t;
    t.translation(5, 3, 2);

    cout << cubeVert << endl;
    cout << t << endl;

    try {
        std::vector<MEGA::F32C1> data {};
        for( size_t i = 0; i < cubeVert.rows; i++) {
            MEGA::Matrix<MEGA::F32C1> result = t.dot(cubeVert.row(i).T());
            for(auto i : result.data) {
                data.push_back(i[0]);
            }
        }
        MEGA::Matrix<MEGA::F32C1> translated (4, 8, data);
        std::cout << translated << std::endl;
    } catch(MEGA::MatrixException exception) {
        cout << exception << endl;
    }
    
	return 0;
}
