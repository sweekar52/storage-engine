CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = -lstdc++fs

SOURCES = src/main.cpp src/memtable.cpp src/sstable.cpp src/storage_engine.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = storage-engine

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean