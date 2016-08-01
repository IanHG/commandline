OBJS=example.o
CXX=g++
CXXSTD=--std=c++14
CXXFLAGS=-Wall
CXXOPTIMFLAGS=-O3
CXXDEBUGFLAGS=-g -O0
CXXFLAGS=$(CXXOPTIMFLAGS)
LIBS=

# link
example.x: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o example.x $(LIBS)

# pull dependencies for existing .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info
%.o: %.cpp
	$(CXX) $(CXXSTD) -c $(CXXFLAGS) $*.cpp -o $*.o
	$(CXX) $(CXXSTD) -MM $(CXXFLAGS) $*.cpp > $*.d

clean:
	rm -f *core *.o *.d tcg.x
