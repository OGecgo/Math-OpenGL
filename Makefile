EXEC = test
FILE = $(EXEC)
OBJECT = src/Main.o src/glad.o src/renderer/ShaderProgram.o

CFLAGS = -g
OPENGLFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
INCLUDE = -Iinclude
SRC = src/Test.c src/glad.c

$(EXEC): $(OBJECT)
	gcc $(OBJECT) $(CFLAGS) -o $(FILE) $(OPENGLFLAGS)
	rm $(OBJECT)

src/%.o: src/%.c
	gcc -c $< $(CFLAGS) $(INCLUDE) -o $@

clean:
	rm $(OBJECT) $(FILE) 

run:
	./$(FILE)

rungdb:
	gdb ./$(FILE)
