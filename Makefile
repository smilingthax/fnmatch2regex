
#CPPFLAGS+=
CXXFLAGS+=-std=c++11 -Wall

all: test

clean:
	$(RM) test.o fnmatch2regex.o test

test.o: fnmatch2regex.h

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

test: test.o fnmatch2regex.o
	$(CXX) -o $@ $^

