

EXEC = play/test
FILE = $(EXEC)
OBJECT = Test.o 

CFLAGS = -g
OPENGLFLAGS = -lglfw -lGL -lGLEW

$(EXEC): $(OBJECT)
	@mkdir -p play
	gcc $(OBJECT) $(CFLAGS) -o $(FILE) $(OPENGLFLAGS)
	./$(FILE)

clean:
	rm $(OBJECT) $(FILE) 

run:
	./$(FILE)

rungdb:
	gdb ./$(FILE)



#It work like would work if all files have changes and we need all remake -B
