CC = g++
CFLAGS = -g -Wall -W -Wextra -std=c++11 -lm --fast-math -O3

a.out: main.o tmachine.o
	$(CC) $(CFLAGS) $^
main.o: main.cpp tmachine.h
	$(CC) $(CFLAGS) -c $<
tmachine.o: tmachine.cpp tmachine.h
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.out *.o
