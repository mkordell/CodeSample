SDIR=src
TDIR=tests
IDIR=include
ODIR=src/obj
LDIR=lib
LIBS=-lm
CXX=g++
CXXFLAGS=-O2 -std=c++11 -flto -march=native -I$(IDIR)
#CXXFLAGS=-g -std=c++11 -flto -march=native -I$(IDIR)

_DEPS=Vec4.h Particle.h Histogram.h Nucleon.h Nucleus.h Event.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_SRCS=Collider.cpp Nucleon.cpp Nucleus.cpp Event.cpp
SRCS=$(patsubst %,$(SDIR)/%,$(_SRCS))

_TESTS=test1.cpp test2.cpp test3.cpp
TESTS=$(patsubst %,$(TDIR)/%,$(_TESTS))

_OBJS=$(_SRCS:.cpp=.o)
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))

_SRCS_T=$(filter-out Collider.cpp,$(_SRCS))
_OBJS_T=$(_SRCS_T:.cpp=.o)
OBJS_T=$(patsubst %,$(ODIR)/%,$(_OBJS_T))

MAIN=Collider

all: $(MAIN)
	@echo Making Collider.out

$(MAIN): $(OBJS)
	$(CXX) -o $@.out $^ $(CXXFLAGS) $(LIBS)

test1 test2 test3:  $(OBJS_T)
	$(CXX) -o $@.out $(TDIR)/$@.cpp $^ $(CXXFLAGS) $(LIBS)
	./$@.out
	rm $@.out

tests: test1 test2 test3

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY : clean

clean :
	rm -f *.out $(ODIR)/*.o *~ core $(IDIR)/*~ $(TDIR)/*.o
