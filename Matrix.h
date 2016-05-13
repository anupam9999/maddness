#ifndef _MATRIX
#define _MATRIX

#include "Vector.h"
#include <iostream>
#include <iomanip>
#include "Matrix.h"
using namespace std;


class Matrix {
public:
	// data members
	int row_size;
	int col_size;
	Vector** data;

	// member functions

	// constructor
	Matrix(int n, int m);

	// another constructor
	Matrix(int n, int m, double **inputData);

	// copy constructor
	Matrix(const Matrix &m);

	// destructor
	~Matrix();

	// dims
	const int get_row_size() const;

	const int get_col_size() const;

	// non-modifiable __getitem__
	double get_item(int index_row, int index_column) const;
	
	// modifiable __getitem__
	Vector &operator[] (int index);

	// __str__
	void print_matrix();

	Matrix &operator=(const Matrix &m2);

	// __mul__
	friend Vector operator*(const Matrix &left, const Vector &right);

	// __rmul__
	friend Vector operator*(const Vector &left, const Matrix &right);

};

// constructor
Matrix::Matrix(int n, int m) {
	row_size = n;
	col_size = m;

	data = new Vector*[row_size];
	for (int i = 0; i < row_size; ++i) {
		data[i] = new Vector(col_size);
	}
}

// another constructor
Matrix::Matrix(int n, int m, double **inputData) {
	row_size = n;
	col_size = m;

	data = new Vector*[row_size];
	for (int i = 0; i < row_size; ++i) {
		data[i] = new Vector(col_size);
		for (int j = 0; j < col_size; ++j) {
			data[i][j] = inputData[i][j];
		}
	} 
}

// copy constructor
Matrix::Matrix(const Matrix &m) {
	row_size = m.row_size;
	col_size = m.col_size;

	data = new Vector*[row_size];
	for (int i = 0; i < row_size; ++i) {
		data[i] = new Vector(col_size);
		for (int j = 0; j < col_size; ++j) {
			data[i][j] = m.data[i][j];
		}
	}
}

// destructor
Matrix::~Matrix() {
	for (int i = 0; i < row_size; ++i) {
		delete data[i];
	}

	delete[] data;
	data = 0;
}

// dims
const int Matrix::get_row_size() const{
	return row_size;
}

const int Matrix::get_col_size() const{
	return col_size;
}


// non-modifiable __getitem__
double Matrix::get_item(int index_row, int index_column) const {
	return data[index_row] -> get_item(index_column);
}

// modifiable __getitem__
Vector &Matrix::operator[] (int index) {

	if ((index >= row_size) || (index < 0)) {
		cout << "Matrix::operator[]:: the index is either less than 0 or greater/equal to row_size" << endl;
	}
	
	return (*data[index]);
}

// __str__
void Matrix::print_matrix() {
	//cout << setw(12) << setprecision(6) << fixed;
	for (int i = 0; i < row_size; ++i) {
		Vector row = (*data[i]);
		for (int j = 0; j < col_size; ++j) {
			//cout << setw(12) << setprecision(6) << fixed <<
			 cout << j << " = " << row[j] << " ";
		}
		cout << endl;
	}

}

Matrix &Matrix::operator=(const Matrix &m2) {

	if (this == &m2) return *this;

	for (int i = 0; i < row_size; ++i) {
		delete data[i];
	}

	delete[] data;

	row_size = m2.row_size;
	col_size = m2.col_size;

	data = new Vector*[row_size];
	for (int i = 0; i < row_size; ++i) {
		data[i] = new Vector(col_size);
		for (int j = 0; j < col_size; ++j) {
			data[i][j] = m2.data[i][j];
		}
	}

	return *this;

}

// __mul__
Vector operator*(const Matrix &left, const Vector &right) {
	if (left.get_col_size() != right.length()) {
		cout << "matrix and vector do not conform" << endl;
		return 0;
	}

	Vector result(left.get_row_size());
	for (int i = 0; i < left.get_row_size(); ++i) {
		for (int j = 0; j < left.get_col_size(); ++j) {
			result[i] = result[i] + (left.get_item(i, j) * right.get_item(j));
		}
	}

	return result;
}

// __rmul__
Vector operator*(const Vector &left, const Matrix &right) {
	
	if (right.get_row_size() != left.length()) {
		cout << "Matrix::operator*::matrix and vector do not conform" << endl;
		return 0;
	}

	Vector result(right.get_col_size());
	for (int i = 0; i < right.get_col_size(); ++i) {
		for (int j = 0; j < right.get_row_size(); ++j) {
			result[i] = result[i] + (left.get_item(j) * right.get_item(j, i));
		}
	}

	return result;
}

#endif