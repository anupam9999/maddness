#ifndef _VECTOR
#define _VECTOR

#include <string>
#include <iostream>
#include <math.h>
#include <string>
#include <cstddef>
#include <sstream>
#include "Vector.h"
using namespace std;

class Vector {
public:
   // data members
   double *data;
   int size;
   
   // member functions
   
   // constructor
   Vector(int inputSize);

   // another constructor
   Vector(double *array, int low, int high);

   // another constructor
   Vector();

   // copy constructor
   Vector(const Vector &v2);

   // destructor
   ~Vector();

   //other functions
   double normf(int low, int high);

   double normf();
   
   double inner(const Vector &v2);

   Vector &gaxpy(double alpha, const Vector &v2, double beta);
   Vector gaxpy_byValue(double alpha, const Vector &v2, double beta);

   Vector &scale(double s);

   Vector &emule(const Vector &v2);

   double get_item(int ind) const;

   void set_item(int ind, double value);

   Vector get_slice(int low, int high);

   void set_slice(int low, int high, double value);

   void set_slice_from_another_vector(int low, int high, const Vector &v);

   int length() const;

   void print_vector() const;

   double &operator[](int ind);

   Vector& operator=(const Vector &v);

   friend const Vector operator|(const Vector &left, const Vector &right);

   friend const Vector operator+(const Vector &left, const Vector &right);

};


// constructor
Vector::Vector(int inputSize) {
   size = inputSize;
   data = new double[size];
   for (int i =0; i < size; ++i) {
       data[i] = 0;
   }
} 


// another constructor
Vector::Vector(double *array, int low, int high) {
   size = (high - low);
   data = new double[size];
   int index = 0;
   for (int i = low; i < high; ++i) {
       data[index++] = array[i];
   }
}

// another constructor
Vector::Vector() {
  Vector(10);
}

// copy constructor
Vector::Vector(const Vector &v2) {
   size = v2.size;
   data = new double[size];
   for (int i = 0; i < size; ++i) {
      data[i] = v2.data[i];
   }
}


// destructor
Vector::~Vector() {
   delete[] data;
   size = 0;
   data = 0;
}

double Vector::normf(int low, int high) {
  double sum = 0.0;
  for (int i = low; i < high; ++i) {
      sum += (data[i] * data[i]);
   }
   return sqrt(sum);
}

double Vector::normf() {
   double sum = 0.0;
   for (int i = 0; i < size; ++i) {
      sum += (data[i] * data[i]);
   }
   return sqrt(sum);
}

double Vector::inner(const Vector &v2) {
   double sum = 0.0;
   for (int i = 0; i < size; ++i) {
      sum += (data[i] * v2.data[i]);
   }
   return sum;
}

Vector Vector::gaxpy_byValue(double alpha, const Vector &v2, double beta) {
  Vector result(size);
  for (int i = 0; i < size; ++i) {
    result.data[i] = alpha * data[i] + beta * v2.data[i];
  }
  return result;
}

Vector &Vector::gaxpy(double alpha, const Vector &v2, double beta){
   for (int i = 0; i < size; ++i){ 
      data[i] = alpha * data[i] + beta * v2.data[i];
   }
   return *this;
}

Vector &Vector::scale(double s){
   for (int i = 0; i < size; ++i) {
      data[i] *= s;
   }
   return *this;
}

Vector &Vector::emule(const Vector &v2) {
   for (int i = 0; i < size; ++i) {
      data[i] *= v2.data[i];
   }
   return *this;
}

double Vector::get_item(int ind) const {
   if ((ind >= size) || (ind < 0))
     cout << "Vector::get_item:: the index is either less than 0 or greater/equal to size" << endl;
   return data[ind];
}

void Vector::set_item(int ind, double value) {
   if ((ind >= size) || (ind < 0))
       cout << "Vector::set_item:: the index is either less than 0 or greater/equal to size" << endl;
   data[ind] = value;
   return;
}

Vector Vector::get_slice(int low, int high) {
   if (low > high) {
      cout << "Vector::get_slice::The value of low is greater than the value of high" << endl;
      return 0;
   }
   else if ((low < 0) || (low >= size)) {
      cout << "Vector::get_slice:: the value of the parameter low is either less than 0 or greater/equal to size" << endl;
      return 0;
   }
   else if ((high < 0) || (high > size)) {
      cout << "Vector::get_slice:: the value of the parameter high is either less than 0 or greater/equal to size" << endl;
      return 0;
   } 

  Vector result(data, low, high);
  return result; 
   
}

void Vector::set_slice_from_another_vector(int low, int high, const Vector &v) {
  for (int i = low; i < high; ++i) {
    data[i] = v.data[i];
  }
}

void Vector::set_slice(int low, int high, double value) {
   for (int i = low; i < high; ++i) {
       data[i] = value;
   }
   return;
}

int Vector::length() const {
   return size;
}

void Vector::print_vector() const {
   cout << "[";
   if (size >= 1) {
       for (int i = 0; i < (size - 1); ++i) {
           ostringstream strs;
           strs << data[i];
           cout << strs.str();
           cout << ", ";
       }
       
       ostringstream strs;
       strs << data[size - 1];
       cout << strs.str();
   }
   cout << "]" << endl;
   return ;

}

double &Vector::operator[](int ind) {
  if ((ind >= size) || (ind < 0))
     cout << "Vector::get_item:: the index is either less than 0 or greater/equal to size" << endl;
   return data[ind];
}

Vector& Vector::operator=(const Vector &v2) {
  if (this == &v2) return *this;
  else {
      delete[] data;
      size = v2.size;
      data = new double[size];
      for (int i = 0; i < size; ++i) {
        data[i] = v2.data[i];
      }

      return *this;
  }
}

const Vector operator|(const Vector &left, const Vector &right) {
  Vector result(left.size + right.size);
  for (int i = 0; i < left.size; ++i)
    result.data[i] = left.data[i];

  for (int j = 0; j < right.size; ++j)
    result.data[left.size + j] = right.data[j];

  return result;
}

const Vector operator+(const Vector &left, const Vector &right) {

    Vector result(left.size);
    for (int i = 0; i < left.size; ++i) {
        result.data[i] = left.data[i] + right.data[i];
    }
    
    return result;
}

#endif
