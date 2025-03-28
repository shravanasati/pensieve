CXX = g++
CXXFLAGS = -Ilib
SOURCES = src/pensieve.cpp src/interpreter.cpp src/constants.cpp src/lexer.cpp src/stringutils.cpp src/token.cpp lib/linenoise.cpp lib/wcwidth.cpp lib/ConvertUTF.cpp
TARGET = pensieve.out

# default target
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run:
	./pensieve.out

.PHONY: all clean