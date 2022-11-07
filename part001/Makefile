TARGET = mylisp
CLANG = clang
SRC = main.c
MPC = ../mpc/mpc.c
LIBS = edit

all:
	$(CLANG) -o $(TARGET) $(SRC) $(MPC) -l $(LIBS)

clean:
	rm $(TARGET)
