CXX = g++
SRCDIR = src
OBJDIR = obj
srcs = $(shell ls $(SRCDIR))
srcspath = $(addprefix $(SRCDIR)/, $(srcs))
objspath = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(srcs)))
lib = libthreadpool.a

all: $(objspath) makelib exe

$(objspath):obj/%o:src/%cpp
	$(CXX) -c $(CFLAGS) $< -o $@ -Iinclude

makelib:$(objspath)
	ar cr $(lib) $(objspath)

exe: makelib
	$(CXX) $(CFLAGS) -c test.cpp -o test.o -I./include
	$(CXX) $(CFLAGS) test.o -o test -L./ -lthreadpool -lpthread -lrt

.PHONY : clean
clean :
	-rm $(objspath)
	-rm $(lib)
	-rm test.o test
	
