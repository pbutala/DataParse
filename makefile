TARGET = dataparse.exe

OBJS = u16List.o dataparse.o

REBUILDABLES = $(OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)     
	gcc -o $@ $^

%.o: %.c
	gcc -o $@ -c $^ 2>build.txt
    
clean:
	rm -f $(REBUILDABLES) build.txt
