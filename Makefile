CC=gcc
CFLAGS=-Wall -Wextra -I.
#DEPS = linked_list.h
OBJ = my_grep.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

my_grep: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o