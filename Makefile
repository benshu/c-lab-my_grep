CC=gcc
CFLAGS=-g -O0 -Wall -Wextra -I.
#DEPS = linked_list.h
OBJ = my_grep.o my_regex.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

my_grep: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o