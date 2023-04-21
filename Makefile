# Makefile Compile all .c into .x
ALL:  $(patsubst %.cpp,%.x, $(wildcard *.cpp))
	
%.x: %.cpp
	g++sim -o $*.x $*.cpp
	
clean:
	$(RM) *.x
