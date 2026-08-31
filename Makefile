CXX ?= g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude
LDFLAGS := -lbcm2835

SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin

TARGET := $(BINDIR)/ads1115
SOURCES := $(SRCDIR)/main.cc $(SRCDIR)/ads1115.cc
OBJECTS := $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJECTS) $(TARGET)
