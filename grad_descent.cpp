/*
* optimizer - Gradient Descent
* 2019/9/29/ by daiyuanli
*
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/time.h>
using namespace std;


//读取数据和标签
void get_Data_and_gt(const char* path,float*& data,float*& gt,int feature_len){

    std::ifstream fin(path); //读取CSV文件流
    std::string line;
    std::string _substr;
    char *p;
    int i = 0;
    int row = 0;

    while(getline(fin,line)){
        
        if(row++ == 0)
           continue;

        int tail = 0,head = 0;
        int cols = -1;
        std::cout<<line<<endl;

        std::string first_ = line.substr(0,2);
        // std::cout<<first_<<std::endl;
        while(head < line.size()/* && (cols++ < feature_len - 1)*/){

           tail = line.find(",",head);
           //line.substr(head,tail).c_str();
           //std::cout<< head <<"-"<<tail<< " ";
           //std::cout<<line.substr(head,tail - head)<<" "; //substr(子串的起始下标,字串长度) 如果找不到就会返回-1
           std::string string_ele = line.substr(head, tail - head);
           
           head=tail+1;
           if(tail == - 1){
               printf("%d %d\n",feature_len,head);
               break;
           }

           cols++;
           if(string_ele == " ")
           {
               data[i++] = -1;
               printf("1");
               continue;
           }

           data[i++] = atof(string_ele.c_str());
        }

        printf("%d\n",head);
        const char* string_gt = line.substr(head,line.size() - head + 1).c_str();
        gt[row] = atof(string_gt);
        std::cout<<row<<" "<<gt[row] <<endl;
        // data[i++] = strtok(line,","); //strtok为c的标准库函数
    }

}

//矩阵乘法
void MatMul(const float* A,int A_cols,int A_rows,const float* B,int B_cols,int B_rows, float*& R,int& R_cols,int& R_rows){

                     R_cols = B_cols;
                     R_rows = A_rows;
                     R = new float[R_cols*R_cols];

                      for (int  i  =  0;  i  <  R_cols;  i++){
                          const float* A_ptr =  A + (i * A_cols);
                          float* R_ptr = R + (i * R_cols);
                          for  (int  j =  0;  j  <  R_cols;  j++){
                                 float sum = 0;
                                  for(int k= 0; k < A_cols ; k++){
                                    printf("%f  x  %f\n",A_ptr[k],B[B_cols *k  + j]);
                                    sum+= A_ptr[k] *B[B_cols * j  + k];
                              }
                              printf("---------------------------------------------------\n");             
                              R_ptr[j] = sum;                      
                          }
                      }
}

//矩阵转置
void MatTranspose(const float* A,int cols,int rows,float*& B)
{
       B = new float[cols*rows]; 
       for(int i = 0;i < rows;i++)
            for(int j = 0;j < cols;j++){
            B[j * cols + i] = A[i*rows + j];
       }
}

//矩阵加
void Matadd(const float* A,int cols,int rows,const float* B,float*& R,int& R_cols,int& R_rows){

            R = new float[cols*rows]; 
            for(int i = 0; i < rows; i++){
                    for(int i = 0; i < cols; i++){
                          R[i*cols + j] = A[i*cols + j] + B[i*cols + j] ;
            }
}

//矩阵减
void Matsub(const float* A,int cols,int rows,const float* B,float*& R,int& R_cols,int& R_rows){

            R = new float[cols*rows]; 
            for(int i = 0; i < rows; i++){
                    for(int i = 0; i < cols; i++){
                          R[i*cols + j] = A[i*cols + j] - B[i*cols + j] ;
            }
}

//梯度下降优化
void Gradient_Descent_LinearReg(float* M_data,float* b_data,int feature_size,float learing_rate,int step,int batchsize){


    // float grad =

    float* minibatch_d; //mini-batch data
    float* minibatch_l; //mini-batch label
    for(int i  = 0;i < step;i++){
                  
    }
    return;

}



int main(){
    
    float* data = new float[5000];
    float* gt = new float[129];

    get_Data_and_gt("./TrainSet.csv", data, gt, 28);

    float a[6] = {1,1,1,2,2,2};
    float b[6]={2,2,1,1,3,3};
    float* r;
    int r_r,r_c;

    struct timeval begin,end;
    gettimeofday(&begin,NULL);
    MatMul((const float*)a,3,2,(const float*)b,2,3, r,r_c , r_r);
    gettimeofday(&end,NULL);   
    float time_val = 1000000*(end.tv_sec - begin.tv_sec) + end.tv_usec - begin.tv_usec;
    printf("mat mul used %f us\n",time_val);

    for(int i  = 0;i < r_r;i++)
    {
        for(int j = 0;j <r_c;j++ )
        {
            printf("%f ",r[i * r_c + j]);
        }
        printf("\n");
    }




}