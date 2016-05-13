#include "main.decl.h"

#include "main.h"
#include "slave.decl.h"

#include <time.h>


/* readonly */ CProxy_Main mainProxy;
/* readonly */ int numProcesses;
/* readonly */ int k;
/* readonly */ double thresh;
/* readonly */ int max_level;



// Entry point of Charm++ application
Main::Main(CkArgMsg* msg) {

  tStart = clock();
  numProcesses = 16; // Default numProcesses to 5

  // There should be 0 or 1 command line arguements.
  // If there is one, it is the number of "Hello"
  // chares that should be created.
  if (msg->argc > 1) {
    numProcesses = atoi(msg->argv[1]);
  }

  k = 2;
  thresh = 0.001;
  max_level = 10;

  // We are done with msg so delete it.
  delete msg;

  // Display some info about this execution
  // for the user.
  CkPrintf("Running \"Function\" with %d elements "
           "using %d processors.\n",
           numProcesses, CkNumPes());

  // Set the mainProxy readonly to point to a
  // proxy for the Main chare object (this
  // chare object).
  mainProxy = thisProxy;

  // Create the array of Hello chare objects. NOTE: The
  // 'helloArray' object that is returned by 'ckNew()' is
  // actually a Proxy object to the array.
  CProxy_Slave slaveArray = CProxy_Slave::ckNew(numProcesses);
}

// Constructor needed for chare object migration (ignore for now)
// NOTE: This constructor does not need to appear in the ".ci" file
Main::Main(CkMigrateMessage* msg) {}

// When called, the "done()" entry method will cause the program
// to exit.
void Main::done_refine() {
	doneCount++;
	if (doneCount >= numProcesses) {
    CkPrintf("Time taken for refine is: %.2fs\n",
      (double)(clock() - tStart)/CLOCKS_PER_SEC);
		CkExit();
	}
}

#include "main.def.h"