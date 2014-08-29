TARGET=test
OBJS=test.o

CFLAGS=-Wall -g
LDFLAGS=-lpthread

$(TARGET):$(OBJS) ring.h
	gcc $^ $(LDFLAGS) -o $@

.c.o:
	gcc $(CFLAGS) $< -c -o $@

clean:
	rm -f $(TARGET) $(OBJS)


