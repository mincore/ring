TARGET=test
OBJS=test.o

CFLAGS=-Wall -Werror -g
LDFLAGS=

$(TARGET):$(OBJS) 
	gcc $^ $(LDFLAGS) -o $@

.c.o: ring.h
	gcc $(CFLAGS) $< -c -o $@

clean:
	rm -f $(TARGET) $(OBJS)


