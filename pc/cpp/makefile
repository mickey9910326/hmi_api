CC = gcc
CXX = g++
SRCS =

CFLAGS += -g -Wall -Wextra
CXXFLAGS += $(CFLAGS)

VPATH = src/ test/
TESTS = test_encoder test_decoder

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(notdir $(SRCS)))
SRCS_TEST = $(wildcard test/*.cpp)
OBJS_TEST = $(patsubst %.cpp, %.o, $(notdir $(SRCS_TEST)))
# House-keeping build targets.

all : $(TESTS)

clean :
	rm -f $(TESTS) *.o *.exe

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

test_encoder : encoder.o test_encoder.o test_main.o
	$(CXX) $^ -lpthread -lgtest -o $@

test: $(OBJS) $(OBJS_TEST)
	$(CXX) $^ -lpthread -lgtest -o $@

.PHONY: all clean
