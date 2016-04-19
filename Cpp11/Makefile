# the compiler: g++ for C++ program, define as g++ for C++
CXX = g++
CC = gcc
RM = rm -f
CFLAGS = $(CXXFLAGS)
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CXXFLAGS  = -Wall -Werror
#LDFLAGS = -lm
SPECIAL_FLAGS = -std=c++11
TURNOFF_FLAGS = -fno-elide-constructors

# the build target executable:
TARGET = type_traits \
		enable_if \
		CRTP \
		variadic_templates \
		rvalue \
		regex


all: $(TARGET)

type_traits: type_traits.cpp
	$(CXX) $(CXXFLAGS) $(SPECIAL_FLAGS) $< -o $@

enable_if: enable_if.cpp
	$(CXX) $(CXXFLAGS) $(SPECIAL_FLAGS) $< -o $@

CRTP: CRTP.cpp
	$(CXX) $(CXXFLAGS) $(SPECIAL_FLAGS) $< -o $@

variadic_templates: variadic_templates.cpp
	$(CXX) $(CXXFLAGS) $(SPECIAL_FLAGS) $< -o $@

rvalue: rvalue.cpp
	$(CXX) $(CXXFLAGS) $(SPECIAL_FLAGS) $(TURNOFF_FLAGS) $< -o $@

regex: regex.cpp
	$(CXX) $(CXXFLAGS) $(SPECIAL_FLAGS) $< -o $@

clean:
	$(RM) $(TARGET) *.o *.class
