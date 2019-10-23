/*
* optimizer - Gradient Descent
* 2019/9/29/ by daiyuanli
*
*/
#include <stdlib.h>
#include <cblas.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
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
           
           if(tail == - 1){
               printf("%d %d\n",feature_len,head);
               break;
           }
           head=tail+1;

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
                     R = new float[R_rows*R_cols];

                      for (int  i  =  0;  i  <  A_rows;  i++){
                          const float* A_ptr =  A + (i * A_cols);
                          float* R_ptr = R + (i * R_cols);
                          for  (int  j =  0;  j  <  R_cols;  j++){
                                 float sum = 0;
                                  for(int k= 0; k < A_cols ; k++){

                                    // printf("%f  x  %f\n",A_ptr[k],B[B_cols * k  + j]);
                                    sum+= A_ptr[k] * B[B_cols * j  + k];
                              }
                            //   printf("---------------------------------------------------\n");             
                              R_ptr[j] = sum;                      
                          }
                      }
}

//矩阵转置 (for circle)
void MatTranspose(const float* A,int cols,int rows,float*& B)
{
       B = new float[cols*rows]; 
       for(int i = 0;i < rows;i++)
            for(int j = 0;j < cols;j++){
            B[j * rows + i] = A[i*cols + j];
       }
}

//矩阵加
void MataddSelf(float* A,int cols,int rows,const float* B){ 

            for(int i = 0; i < rows; i++)
                    for(int j = 0; j < cols; j++){
                        //   printf("%d\n",i);
                          A[i*cols + j]+=B[i*cols + j];
            
                    }
}

void Matadd(const float* A,int cols,int rows,const float* B,float*& R,int& R_cols,int& R_rows){

            if(R == NULL)
              R = new float[cols*rows]; 

            for(int i = 0; i < rows; i++)
                    for(int j = 0; j < cols; j++){
                          R[i*cols + j] = A[i*cols + j] + B[i*cols + j];
                    }
}

//矩阵减
void Matsub(const float* A,int cols,int rows,const float* B,float*& R){

            R = new float[cols*rows]; 
            for(int i = 0; i < rows; i++)
                    for(int j = 0; j < cols; j++){
                          R[i*cols + j] = A[i*cols + j] - B[i*cols + j] ;
            }
}

void getBatch(const float* dtsrc,const float* gtsrc,float*& batch,float*& gt,const int batch_sz,const int ft_sz,const int dt_sz){

    static int idx = 0;
    
    if(batch == NULL)
       batch = new float[batch_sz * ft_sz];

    if(gt == NULL)
       gt = new float[batch_sz];

    if(idx + batch_sz >= dt_sz)
    {
       memcpy(batch,dtsrc + idx * ft_sz,(dt_sz - idx) * ft_sz * sizeof(float));
       memcpy(batch + (dt_sz - idx) * ft_sz, dtsrc, (idx + batch_sz - dt_sz) * ft_sz * sizeof(float));
       memcpy(gt,gtsrc + idx,(dt_sz - idx) * sizeof(float));
       memcpy(gt + dt_sz - idx,gtsrc,(dt_sz - idx) * sizeof(float));
       idx = (idx + batch_sz)%dt_sz;
       return;
    }   

    memcpy(batch, dtsrc + idx * ft_sz, batch_sz * ft_sz * sizeof(float));
    memcpy(gt, gtsrc + idx, batch_sz * sizeof(float));    
    idx+=batch_sz;

}

//梯度下降优化
void Gradient_Descent_LinearReg(float* M_data,float* b_data,int feature_size,float learing_rate,int step,int batchsize){


    // float grad =
    float* minibatch_d = NULL; //mini-batch data
    float* minibatch_l = NULL; //mini-batch label
    float  weight_arr[28] = { 0.0001 };
    for(int i = 0; i < feature_size; i++){
        weight_arr[i] = 0.0001;
    }
    for(int i = 0; i < step; i++){
         getBatch(M_data,b_data,minibatch_d,minibatch_l,batchsize,feature_size,129);

         //df(w) = -X'(y-Xw)
         float* M1,* M2,* M3,* M4;
         MatTranspose(minibatch_d, feature_size, batchsize, M1);
        //  for(int i = 0;i < feature_size; i++){
        //      for(int j = 0;j < batchsize;j++)
        //        cout<<M1[i*batchsize + j]<<" ";
        //     cout<<endl;
        //  }

         
         //Xw
        //  cout<<"----------------------Xw--------------------------"<<endl;
         int M3_cols = 0,M3_rows = 0;
         MatMul(minibatch_d,feature_size,batchsize,weight_arr, 1, feature_size, M3, M3_cols, M3_rows);
        //  for(int i = 0;i < M3_rows; i++){
        //      for(int j = 0;j < M3_cols;j++)
        //        cout<<M3[i*M3_cols + j]<<" ";
        //     cout<<endl;
        //  }


         //y - Xw
         Matsub(minibatch_l, 1, batchsize, M3, M2);
        //  for(int i = 0;i < batchsize; i++){
        //        cout<<M2[i]<<" ";
        //  }
        //  cout<<endl;

         //-X'(y-Xw)
         int M4_cols = 0,M4_rows = 0;
         MatMul(M1, batchsize, feature_size, M2, 1, batchsize, M4, M4_cols, M4_rows);

         for(int c = 0;c < feature_size;c++)  
         {
             M4[c]*=learing_rate / batchsize;
         }  
         //Mat add
         int W_cols = 0,W_rows = 0;
        //  Matadd(weight_arr, 1, batchsize, M1, weight_arr, W_cols,W_rows);
         MataddSelf(weight_arr, 1, feature_size, M4);
        cout<<endl;

        //train test
        float* result;
        MatMul(minibatch_d,feature_size,batchsize,weight_arr, 1, feature_size, result, M3_cols, M3_rows);
        float avg = 0.f;
        for(int i = 0;i < batchsize;i++)
          avg+=(result[i] - minibatch_l[i]);

        printf("err:%f\n",abs(avg/batchsize));
         //printf loss
        //  for(int j = 0;j < batchsize;j++)
        //  {
        //      for(int k = 0;k < feature_size; k++)
        //         cout<<minibatch_d[feature_size*j+k]<<" ";

        //      cout<<" label:"<<minibatch_l[j]<<endl;
        //  }
        // cout<<endl;
       
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

    // struct timeval begin,end;
    // gettimeofday(&begin,NULL);
    // MatMul((const float*)a,3,2,(const float*)b,2,3, r,r_c , r_r);
    // gettimeofday(&end,NULL);   
    // float time_val = 1000000*(end.tv_sec - begin.tv_sec) + end.tv_usec - begin.tv_usec;
    // printf("mat mul used %f us\n",time_val);

    // for(int i  = 0;i < r_r;i++)
    // {
    //     for(int j = 0;j <r_c;j++ )
    //     {
    //         printf("%f ",r[i * r_c + j]);
    //     }
    //     printf("\n");
    // }

    float datatest[] = {1,1,1,2,2,2,2,2,3,3,3,3,3,3,3};
    float gttest[] = {0,1,2,3,4,5};
    Gradient_Descent_LinearReg(data,gt,28,0.00001,100,120);

}
