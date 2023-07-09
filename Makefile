CC=gcc
CFLAGS=-I. -lcurl -lpthread
DEPS = web_crawler.h
OBJ = main.o web_crawler.o queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

web_crawler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) web_crawler

