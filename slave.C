
// VERSION TWO: we have main chare and also the slave chare (which inside its constructor, we can have the main part of the function.cpp file (also, we can have test1, test2, etc))
#include "slave.decl.h"

#include "slave.h"
#include "main.decl.h"

#include <math.h> 
#include "function.h"

extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ int numProcesses;
extern /* readonly */ int k;
extern /* readonly */ double thresh;
extern /* readonly */ int max_level;

// function declrataions
double test1(double x);
double test2(double x);

Slave::Slave() {
	/* ==> read-only variables set by the main chare
	k = kInput;
	thresh = threshInput;
	max_level = 30;

	*/
	CkPrintf("Constructor of the Slave chare # %d is called on processor %d.\n", thisIndex, CkMyPe());

	int n = (int) log2(numProcesses);
	int l = thisIndex;

	Function *function = new Function(n, l, k, thresh, test1);

	mainProxy.done_refine();
}

Slave::Slave(CkMigrateMessage *msg) {}


// TODO: ADD more FUNCTIONS HERE!!
double test1(double x) {
	double a = 500.0;
	return (pow(2*a/M_PI,0.25)*exp(-1 * a * pow((x-0.5),2)));
}

double test2(double x) {
	return (test1(x-0.3) + test1(x)  + test1(x+0.3));
}


#include "slave.def.h"