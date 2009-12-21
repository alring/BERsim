IDIR=-I$(value PWD) -I/usr/include/python2.6
CPP=g++
FLAGS=-c -fPIC -g -Wall
LIBOBJ=libBER.o
NAME=libBER
LIB=-L . -lBER -lm -lpython2.6 -lboost_python

SONAME=$(NAME).so.1
LINKNAME=$(NAME).so
REALNAME=$(NAME).so.1.0.1
STATNAME=$(NAME).a

all: lib ber

ber: ber.o
	$(CPP) -o $@ $^ $(LIB) $(IDIR)

bp: bp.o
	$(CPP) -o $@ $^ $(LIB) $(IDIR)

plottest: plottest.cpp
	$(CPP) -o $@ $^ -lplotter -lXaw -lXmu -lXt -lSM -lICE -lXext -lX11 -lm -I.



lib: $(LIBOBJ)
	$(CPP) -shared -Wl,-soname,$(SONAME) -o $(REALNAME) $(LIBOBJ) -lc
	ar rcs $(STATNAME) $(LIBOBJ)
	ln -s ./$(REALNAME) ./$(LINKNAME)
	ln -s ./$(REALNAME) ./$(SONAME)

%.o:%.c
	$(CC) $(FLAGS) $+ $(IDIR)

%.o:%.cpp
	$(CPP) $(FLAGS) $+ $(IDIR)

.PHONY:
	clean

clean:
	rm -f *.o ber $(SONAME) $(LINKNAME) $(REALNAME) $(STATNAME)
