CXX = g++
CXXFLAGS = -g -w
SRCS = main.cpp setup.cpp doubleLinkList.cpp cd.cpp echo.cpp pwd.cpp ls.cpp pinfo.cpp search.cpp history.cpp redirection.cpp prompt.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = shell.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

|clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: all clean
