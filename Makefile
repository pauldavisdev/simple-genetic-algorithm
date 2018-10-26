CC = gcc
CFLAGS = -Wall
DEPS = population_operations.h
OBJ = main.o population_operations.o
LDLIBS = -lm

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o main population_operations