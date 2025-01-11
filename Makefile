PROG = minesweeper
CXX = g++
CXXFLAGS = -Wall -Wextra -O0 -I/usr/include/X11
LDFLAGS = -lX11

SRCS = ${PROG}.cpp data.cpp
OBJS = ${SRCS:.cpp=.o}

all: ${PROG}

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${PROG}: ${OBJS}
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -f ${PROG} ${OBJS}

.PHONY: all clean