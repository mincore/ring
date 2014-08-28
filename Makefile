TARGET=test
OBJS=test.o

CFLAGS=-Wall -g
LDFLAGS=

$(TARGET):$(OBJS) ring.h
	gcc $^ $(LDFLAGS) -o $@

.c.o:
	gcc $(CFLAGS) $< -c -o $@

clean:
	rm -f $(TARGET) $(OBJS)


