CC=gcc

ALL_C=$(wildcard *.c)
SOURCES?=$(filter-out %_test.c,$(ALL_C))
OBJS?=$(addprefix , $(SOURCES:.c=.o)) 

CFLAGS +=-W -Wall -pedantic -O2 -I.

LDFLAGS+=

TARGET := usart

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

