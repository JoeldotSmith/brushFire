# Makefile Compile all .c into .x
ALL:  $(patsubst %.c,%.x, $(wildcard *.c))
	
%.x: %.cpp
	g++sim -o $*.x $*.cpp
	
clean:
	$(RM) *.x
