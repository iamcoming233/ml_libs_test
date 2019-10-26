
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
    struct DataPtr{

        public:
            T* data_ptr;
            int _len;
        
        private:
            int ref_count;
    
            DataPtr(int sz){
                
                ref_count = 1;
                _len = sz;
                data_ptr = new T[_len];
            }
            ~DataPtr(){
                delete data_ptr;
            }
    };
    
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
            //shallow copy
            AutoMat& operator=(const AutoMat<T>& b);

            void filldata(T d);

            void show();

            int cols;
            int rows;
            int channel;
            int rstep;

        private:
            shared_ptr<DataPtr<T>> data; //矩阵数据指针
        
    };


template <class T>
AutoMat<T>::AutoMat(){
    cols = -1;
    rows = -1;
    channel = -1;

    // data_ptr = make_shared(NULL);
    printf("construct()\n");
}

template <class T>
AutoMat<T>::AutoMat(int _rows,int _cols){

    cols = _cols;
    rows = _rows;
    channel = 1;
    
    
    data = make_shared(cols * rows * 1);
    // printf("construct(int _rows,int _cols) data_ptr:%x\n",data_ptr);

}

template <class T>
AutoMat<T>::AutoMat(int _rows,int _cols,int channel){

    cols = _cols;
    rows = _rows;
    channel = channel;
    data = make_shared(cols * rows * channel);
}

template <class T>
AutoMat<T>::~AutoMat(){
    
    printf("release data ptr:%x\n",data.get()->data_ptr);
    // if(data.count==0)
    //   delete *(data_ptr.get());
    // *(data_ptr.get())=NULL;
}

template <class T>
AutoMat<T> AutoMat<T>::operator*(const AutoMat<T>& b){
         
        assert(this->channel==1&&b.channel==1);
        assert(this->cols==b.rows);

        AutoMat<T>* R = new AutoMat<T>(this->rows,b.cols); 
        
        // AutoMat<T> R = new float[R_rows*R_cols];

        for (int  i  =  0;  i  <  R->rows;  i++){
            T* tmp =  data.get()->data_ptr + (i * this->cols);
            T* out = R->data.get()->data_ptr + (i * R->cols);
            for  (int  j =  0;  j  <  b.cols;  j++){
                   T sum = 0;
                    for(int k= 0; k < this->cols ; k++){
                      sum+= tmp[k] * b.data.get()->data_ptr[b.cols * j  + k];
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
              R.data.get()->data_ptr[i*cols + j] = this->data.get()->data_ptr[i*cols + j] + b.data.get()->data_ptr[i*cols + j];
        }
    return R;
}

template <class T>
AutoMat<T> AutoMat<T>::operator-(const AutoMat<T>& b){
    assert(this->rows==b.rows || this->cols==b.cols);

    AutoMat<T> R = new AutoMat(this->rows,this->cols); 
    for(int i = 0; i < this->rows; i++)
        for(int j = 0; j < this->cols; j++){
              R.data.get()->data_ptr[i*cols + j] = this->data.get()->data_ptr[i*cols + j] + b.data.get()->data_ptr[i*cols + j];
        }
    return R;
}

template <class T>
AutoMat<T>& AutoMat<T>::operator=(const AutoMat<T>& b){

    // AutoMat<T>* R = new AutoMat(); 
    cols = b.cols;
    rows = b.rows; 
    channel = b.channel;
    data = b.data;

    return *this;
}


template <class T>
void AutoMat<T>::transpose(AutoMat<T>& b){
    assert(b.data);

    for(int i = 0; i < this->rows; i++)
        for(int j = 0; j < this->cols; j++){
         b.data.get()->data_ptr[j * rows + i] = b.data.get()->data_ptr[i*cols + j];
    }
    return b;
}

template <class T>
void AutoMat<T>::show(){
    assert(this->data);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
        //  printf("22");
         cout<<data.get()->data_ptr[i*cols + j]<<" ";
        }
        cout<<endl;
    }
}

template <class T>
void AutoMat<T>::filldata(T d){

    assert(this->data);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            // printf("(%d,%d)|",i,j);
        //    cout<<i<<" "<<j<<"|";
           data.get()->data_ptr[i*cols + j] = d;
        }
        // printf("\n");
   }
}
}



#endif