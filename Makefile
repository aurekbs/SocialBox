INCLUDE_DIR = include
OBJ_DIR = obj
SRC_DIR = src
JAVA_DIR = java
SCRIPT_DIR = script

CC = gcc
CFLAGS = -Wall -Wextra -fPIC -c -I$(INCLUDE_DIR)

PROG = intersections 
TEST = runtest
LIB = $(JAVA_DIR)/libintersections.so
EXPE = $(SCRIPT_DIR)/expe

JAVA_HOME = /Users/aurelienkrebs/Library/Java/JavaVirtualMachines/openjdk-23.0.1/Contents/Home
JAVA_INCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin

_DEPS = util.h rational.h list.h geometry.h algo.h tree.h
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ= util.o rational.o list.o geometry.o algo.o tree.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

.PHONY: all run test jni expe clean delete deletetest deletelib deleteexpe cleanall memorycheck testmemorycheck

all : $(PROG) $(TEST) $(LIB) $(EXPE)

run : $(PROG)
	./$(PROG) $(in) $(out) $(algo)

test : $(TEST)
	./$(TEST)

jni : $(LIB)

expe : $(EXPE)

clean :
	rm -f $(OBJ_DIR)/*.o

delete :
	rm -f $(PROG)

deletetest :
	rm -f $(TEST)

deletelib :
	rm -f $(LIB)

deleteexpe :
	rm -f $(EXPE)

cleanall : clean delete deletetest deletelib deleteexpe

memorycheck : $(PROG)
	valgrind ./$(PROG) $(in) $(out) $(algo)

testmemorycheck : $(TEST)
	valgrind ./$(TEST)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(DEP)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/application_Intersections.o : $(SRC_DIR)/application_Intersections.c $(DEP)
	$(CC) $(CFLAGS) $(JAVA_INCLUDES) -o $@ $<

$(PROG) : $(OBJ) $(OBJ_DIR)/main.o
	$(CC) -o $@ $^ -lm

$(TEST) : $(OBJ) $(OBJ_DIR)/test.o
	$(CC) -o $@ $^ -lm

$(EXPE) : $(OBJ) $(OBJ_DIR)/expe.o
	$(CC) -o $@ $^ -lm

$(LIB) : $(OBJ) $(OBJ_DIR)/application_Intersections.o
	$(CC) -shared -o $@ $^ -lm
