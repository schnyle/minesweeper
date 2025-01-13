PROG = xwindow
CXX = g++
CXXFLAGS = -Wall -Wextra -O0 -I/usr/include/X11
LDFLAGS = -lX11

all: ${PROG}

${PROG}.o: ${PROG}.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${PROG}: ${PROG}.o
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -f ${PROG} ${PROG}.o

.PHONY: all clean