#ifndef _MAIN_H
#define _MAIN_H

class Main : public CBase_Main {
private:
	int doneCount = 0;
	clock_t tStart;
	
public:

  /// Constructors ///
  Main(CkArgMsg* msg);
  Main(CkMigrateMessage* msg);

  void done_refine();

};

#endif //_MAIN_H