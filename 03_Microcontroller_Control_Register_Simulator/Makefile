CC = gcc-14
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g

motor_tests: motor_control.o test_motor_control.o
	$(CC) motor_control.o test_motor_control.o -o motor_tests
	
motor_control.o: motor_control.c motor_control.h
	$(CC) $(CFLAGS) -c motor_control.c -o motor_control.o
	
test_motor_control.o: test_motor_control.c motor_control.h
	$(CC) $(CFLAGS) -c test_motor_control.c -o test_motor_control.o

.PHONY: test clean

test: motor_tests
	./motor_tests


clean:
	rm -f motor_control.o test_motor_control.o motor_tests

