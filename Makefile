CXX ?= g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude

SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin

TARGET := $(BINDIR)/ads1115
SOURCES := $(SRCDIR)/main.cpp $(SRCDIR)/ads1115.cpp
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	sudo ./$(TARGET)
