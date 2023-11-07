CXX = g++
CFLAGS = -Wall -Wextra -g -std=c++17 -O1 -MMD -MP

TARGET = target

SRCDIR = ./src/
OBJDIR = ./obj/
BINDIR = ./bin/

INC = -I./include/ -I/home/jonathan/Dev/Include/
LIB = -L/home/jonathan/Dev/Lib 

SRC := $(shell find $(SRCDIR) -maxdepth 1 -type f -name "*.cpp")
OBJ := $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(SRC:.cpp=.o))


.PHONY: clean $(BINDIR)$(TARGET)
all: $(BINDIR)$(TARGET)


$(BINDIR)$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $@ $(LIB)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -c $(CFLAGS) $(INC) $< -o $@

-include $(OBJ:.o=.d)


run: $(BINDIR)$(TARGET)
	$(BINDIR)$(TARGET)

commands:
	bear -- make

clean:
	$(RM) -f -r $(OBJDIR)* $(BINDIR)*
