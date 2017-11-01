OBJS = test.out

p1: libtest.cc CampbellLib.cc CampbellLib.h
	g++ libtest.cc -o test.out

test:
	./test.out

clean:
	\rm *.out

tar:
	tar cfv CampbellLib.tar CampbellLib.cpp CampbellLib.h libtest.cc
