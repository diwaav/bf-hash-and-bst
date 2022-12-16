CC = clang
CFLAGS = -Werror -Wall -Wextra -Wpedantic
TARGET = banhammer
LFLAGS = -lm

OBJECTS = banhammer.o speck.o ht.o bst.o node.o bf.o bv.o parser.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(TARGET) *.o

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
