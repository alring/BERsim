IDIR=-I$(value PWD) -I/usr/include/python2.6
CPP=g++
FLAGS=-c -fPIC -g -Wall
NOPYTHON=-DNOPYTHON
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

lib: $(LIBOBJ)
	$(CPP) -shared -Wl,-soname,$(SONAME) -o $(REALNAME) $(LIBOBJ) -lc
	ar rcs $(STATNAME) $(LIBOBJ)
	ln -s ./$(REALNAME) ./$(LINKNAME)
	ln -s ./$(REALNAME) ./$(SONAME)

# must be root
install:
	@echo 'Copying Libraries to /usr/lib'
	cp $(STATNAME) /usr/lib
	cp $(REALNAME) /usr/lib
	@echo 'Making Symbolic Links'
	ln -s /usr/lib/$(REALNAME) /usr/lib/$(LINKNAME)
	ln -s /usr/lib/$(REALNAME) /usr/lib/$(SONAME)

# must be root
remove:
	@echo 'Removing Libraries from /usr/lib'
	rm -f /usr/lib/$(STATNAME)
	rm -f /usr/lib/$(REALNAME)
	@echo 'Removing Symbolic Links'
	rm -f /usr/lib$(SONAME)
	rm -f /usr/lib$(LINKNAME)



%.o:%.c
	$(CC) $(FLAGS) $+ $(IDIR)

%.o:%.cpp
	$(CPP) $(FLAGS) $+ $(IDIR)

.PHONY:
	clean

clean:
	rm -f *.o ber $(SONAME) $(LINKNAME) $(REALNAME) $(STATNAME) *.zip *.tar
