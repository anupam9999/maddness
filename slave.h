#ifndef _SLAVE
#define _SLAVE

class Slave : public CBase_Slave {
private:

	// Function *function;

public:
	Slave();
	Slave(CkMigrateMessage *msg);
};

#endif