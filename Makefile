
CC := gcc

ALL_C	?= $(wildcard *.c)
SOURCES	?= $(filter-out %_test.c,$(ALL_C))
OBJS	?= $(addprefix , $(SOURCES:.c=.o)) 

CFLAGS  := -W -Wall -pedantic -O2 -I.

LDFLAGS :=

TARGET  := usart

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "[CC] $@"
	@$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): %.o: %.c
	@echo "[CC] $@"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "[RM] $(OBJS)"
	@rm -f $(OBJS) $(TARGET)

