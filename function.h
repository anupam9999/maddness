#ifndef _FUNCTION
#define _FUNCTION

#include "ConcurrentMap.h"
#include "Key.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quadrature.h"
#include "Twoscalecoeffs.h"

class Function {
private:

	int k;
	double thresh;
	double (*f)(double);
	int max_level; // = 30

	ConcurrentMap s;
	ConcurrentMap d;

	Matrix * hg;
	Matrix * hg0;
	Matrix * hg1;
	Matrix * hgT;

	Matrix * rm;
	Matrix * r0;
	Matrix * rp;

	bool compressed;

	Vector * quad_w;
	Vector * quad_x;
	int quad_npt;
	Matrix * quad_phi;
	Matrix * quad_phiT;
	Matrix * quad_phiw;

public:
	Function(int n, int l, int kInput,
	double threshInput, double (*function) (double));

	Function(CkMigrateMessage *msg);

	// used in the constructor function
	void init_twoscale(int k);
	void init_quadrature(int order);
	void make_dc_periodic();

	// operators
	void refine(int n, int l);

	// used in refine function
	Vector project(int nInput, int lInput);

};

Function::Function(int n, int l, int kInput,
	double threshInput, double (*function) (double)) {
	
	k = kInput;
	thresh = threshInput;
	f = function;
	max_level = 30;

	init_twoscale(k);
	init_quadrature(k);
	make_dc_periodic();
	
	compressed = false;

	refine(n, l);
}


Function::Function(CkMigrateMessage *msg) {}

void Function::init_twoscale(int k) {

	double  (*hgInput)[22] = twoscalecoeffs(k);

	hg = new Matrix(2*k, 2*k);
	hg0 = new Matrix(2*k, 2*k);
	hg1 = new Matrix(2*k, 2*k);
	hgT = new Matrix(2*k, 2*k);

	for (int i = 0; i < 2 * k; ++i) {
		for (int j = 0; j < 2 * k; ++j) {
			((*hg)[i])[j] = hgInput[i][j];
			((*hgT)[i])[j] = hgInput[j][i];

		}
	}

	for (int i = 0; i < 2 * k; ++i) {
		for (int j = 0; j < k; ++j) {
			((*hg0)[i])[j] = hgInput[i][j];
			((*hg1)[i])[j] = hgInput[i][j + k];
		}
	}
}

void Function::init_quadrature(int order) {
	double *x = gauss_legendre_point(order);
	double *w = gauss_legendre_weight(order);

	quad_w = new Vector(w, 0, order);
	quad_x = new Vector(x, 0, order);

	int npt = order;
	quad_npt = npt;

	quad_phi = new Matrix(npt, k);
	quad_phiT = new Matrix(k, npt);
	quad_phiw = new Matrix(npt, k);

	for (int i = 0; i < npt; ++i) {
		double * p = phi((*quad_x)[i], k);
		for (int m = 0; m < k; ++m) {
			((*quad_phi)[i])[m] = p[m];
			((*quad_phiT)[m])[i] = p[m];
			((*quad_phiw)[i])[m] = w[i] * p[m];
		}
	}
}

void Function::make_dc_periodic() {
	rm = new Matrix(k, k);
	r0 = new Matrix(k, k);
	rp = new Matrix(k, k);

	double iphase = 1.0;
	for (int i = 0; i < k; ++i) {
		double jphase = 1.0;

		for (int j = 0; j < k; ++j) {
			double gammaij = sqrt(( 2 * i + 1) * ( 2 * j + 1));
			double Kij;
			if ((( i -  j ) > 0) && (((i - j ) % 2) == 1 )) {
				Kij = 2.0;
			} else {
				Kij = 0.0;
			}

			((*r0)[i])[j] = 0.5 * (1.0 - iphase * jphase - 2.0 * Kij) * gammaij;
            ((*rm)[i])[j] = 0.5 * jphase * gammaij;
            ((*rp)[i])[j] =-0.5 * iphase * gammaij;
            jphase = -1 * jphase;
		}
		iphase = -1 * iphase;
	}
}

Vector Function::project(int nInput, int lInput) {
	Vector s(k);

    double h = pow(0.5, nInput);
    double scale = sqrt(h);
    for (int mu = 0; mu < quad_npt; ++mu) {
      double x = (lInput + (*quad_x)[mu]) * h;
      double fValue = f(x);
      for (int i = 0; i < k; ++i) {
        s[i] = s[i] + (scale * fValue * ((*quad_phiw)[mu])[i]);
      }
    }
    return s;
}

// operators
void Function::refine(int n, int l) {

	Vector s0 = project(n + 1, 2 * l);
	Vector s1 = project(n + 1, 2 * l + 1);

	Vector ss(s0 | s1);
	Vector d(ss * (*hgT));

	if (d.normf(k, 2*k) < thresh ||  n >= max_level - 1) {

	    // thread-safe versions
	    Key key0(n + 1, 2 * l);
	    Key key1(n + 1, 2 * l + 1);

	    // s.set(key0, s0)
	    // s.set(key1, s1);
	    s.set_not_threadsafe(key0, s0);
	    s.set_not_threadsafe(key1, s1);

	  } 
	  else {
	  	// this also can be parallelized using cilk or openMP
	  	// cilk_spawn refine(n + 1, 2 * l);
	  	
	    refine(n + 1, 2 * l);
	    refine(n + 1, 2 * l + 1);
	  }
}



#endif