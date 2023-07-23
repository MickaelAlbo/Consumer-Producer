CXX = g++
CXXFLAGS = -pthread -std=c++11

SRCS = main.cpp Queue.cpp Producer.cpp Dispatcher.cpp CoEditor.cpp ScreenManager.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = ex3.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

