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

    AutoMat<int> *m1 = new AutoMat<int>(10,10);
    m1->filldata(1);
    AutoMat<int> *m2 = new AutoMat<int>(10,10);
    m2->filldata(2);
    
    ((*m1)*(*m2)).show();
    

    m1->show();
    return 0;
}

