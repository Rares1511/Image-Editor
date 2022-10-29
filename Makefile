CC = gcc
CFLAGS = -g -Wall -Wextra

build: image_editor

image_editor: image_editor.o extra_functions.o
	$(CC) $(CFLAGS) $^ -o $@

image_editor.o: image_editor.c image_editor.h
	$(CC) $(CFLAGS) -c $< -o $@

extra_functions.o: extra_functions.c extra_functions.h
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./image_editor

clean:
	rm -f *.o
	rm -f image_editor
