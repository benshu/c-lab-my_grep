CC=gcc
CFLAGS=-Wall -Wextra -I.
DEPS = linked_list.h
OBJ = linked_list.o linked_list_ui.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ex1: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o