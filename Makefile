CHARMDIR = /home/mahdi/Desktop/charm
CHARMC = $(CHARMDIR)/bin/charmc $(OPTS)


default: all
all: slave


slave : main.o slave.o
	$(CHARMC) -language charm++ -o slave main.o slave.o

main.o : main.C main.h main.decl.h main.def.h slave.decl.h
	$(CHARMC) -o main.o main.C

main.decl.h main.def.h : main.ci
	$(CHARMC) main.ci

slave.o : slave.C slave.h slave.decl.h slave.def.h main.decl.h
	$(CHARMC) -o slave.o slave.C

slave.decl.h slave.def.h : slave.ci
	$(CHARMC) slave.ci


clean:
	rm -f main.decl.h main.def.h main.o
	rm -f slave.decl.h slave.def.h slave.o
	rm -f slave charmrun
