
//by lidaiyuan 
//a simple gemm libaray
#ifndef _AUTO_MAT_H
#define _AUTO_MAT_H

#include<auto_mat.h>
#include<cblas.h>
#include<assert.h>
#include<memory>
#include<iostream>

using namespace std;
namespace automl{
    
    template <class T>
    class AutoMat{
         
         public:
            AutoMat();
            ~AutoMat();
            AutoMat(int _rows,int _cols); //
            AutoMat(int _rows,int _cols,int _channel); //


            //TODO:matrix mutiply
            AutoMat operator*(const AutoMat& b);
            //TODO:matrix add
            AutoMat operator+(const AutoMat& b);
            //TODO:matrix transpose
            void transpose(AutoMat& b);
            //TODO:matrix substract
            AutoMat operator-(const AutoMat& b);

            void filldata(T d);

            void show();

            int cols;
            int rows;
            int channel;
            int rstep;

        private:
            shared_ptr<T> data_ptr; //矩阵数据指针
        
    };


template <class T>
AutoMat<T>::AutoMat(){
    cols = -1;
    rows = -1;
    channel = -1;
}

template <class T>
AutoMat<T>::AutoMat(int _rows,int _cols){

    cols = _cols;
    rows = _rows;
    channel = 1;
    
    data_ptr = make_shared<T>(cols * rows * 1);
}

template <class T>
AutoMat<T>::AutoMat(int _rows,int _cols,int channel){

    cols = _cols;
    rows = _rows;
    channel = channel;
    
    data_ptr = make_shared<T>(cols * rows * channel);
}

template <class T>
AutoMat<T>::~AutoMat(){
   data_ptr.reset();
}

template <class T>
AutoMat<T> AutoMat<T>::operator*(const AutoMat<T>& b){
         
        assert(this->channel==1&&b.channel==1);
        assert(this->cols==b.rows);

        AutoMat<T>* R = new AutoMat<T>(this->rows,b.cols); 
        
        // AutoMat<T> R = new float[R_rows*R_cols];

        for (int  i  =  0;  i  <  R->rows;  i++){
            T* tmp =  this->data_ptr.get() + (i * this->cols);
            T* out = R->data_ptr.get() + (i * R->cols);
            for  (int  j =  0;  j  <  b.cols;  j++){
                   float sum = 0;
                    for(int k= 0; k < this->cols ; k++){
                      sum+= tmp[k] * b.data_ptr.get()[b.cols * j  + k];
                }
                //   printf("---------------------------------------------------\n");             
                out[j] = sum;                      
            }
        }
        return *R;
}

template <class T>
AutoMat<T> AutoMat<T>::operator+(const AutoMat<T>& b){ 

    assert(this->rows==b.rows || this->cols==b.cols);

    AutoMat<T> R = new AutoMat(this->rows,this->cols); 
    for(int i = 0; i < this->rows; i++)
        for(int j = 0; j < this->cols; j++){
              R.data_ptr[i*cols + j] = this->data_ptr[i*cols + j] + b.data_ptr[i*cols + j];
        }
    return R;
}

template <class T>
AutoMat<T> AutoMat<T>::operator-(const AutoMat<T>& b){
    assert(this->rows==b.rows || this->cols==b.cols);

    AutoMat<T> R = new AutoMat(this->rows,this->cols); 
    for(int i = 0; i < this->rows; i++)
        for(int j = 0; j < this->cols; j++){
              R.data_ptr[i*cols + j] = this->data_ptr[i*cols + j] + b.data_ptr[i*cols + j];
        }
    return R;
}

template <class T>
void AutoMat<T>::transpose(AutoMat<T>& b){
    assert(b.data_ptr);

    for(int i = 0; i < this->rows; i++)
        for(int j = 0; j < this->cols; j++){
         b.data_ptr[j * rows + i] = b.data_ptr[i*cols + j];
    }
    return b;
}

template <class T>
void AutoMat<T>::show(){
    assert(this->data_ptr);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
        //  printf("22");
         cout<<data_ptr.get()[i*cols + j]<<" ";
        }
        cout<<endl;
    }
}

template <class T>
void AutoMat<T>::filldata(T d){

    assert(this->data_ptr);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("(%d,%d)|",i,j);
        //    cout<<i<<" "<<j<<"|";
           data_ptr.get()[i*cols + j] = d;
        }
        // printf("\n");
   }
}
}



#endif