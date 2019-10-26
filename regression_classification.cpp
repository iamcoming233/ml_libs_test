#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <auto_mat.h>
#include <sys/time.h>
using namespace std;
using namespace automl;

int main(){

    AutoMat<float> m1(4,3); //= new AutoMat<int>(10,10);
    m1.filldata(1);
    AutoMat<float> m2(3,1); //= new AutoMat<int>(10,10);
    m2.filldata(2);
    AutoMat<float> m4;
    m4 = m1*m2;
    

    m1.show();

    // m4 = m1;
    m4.show();
    // for()
    return 0;
}

