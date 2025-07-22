CXX = g++-14
CXXFLAGS = -std=c++20 -Wall -MMD -Werror=vla -g
EXEC = chess
SRC = $(filter-out gui.cc, $(wildcard *.cc))
OBJECTS = ${SRC:.cc=.o}
DEPENDS = ${OBJ:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} # Comment out for X11
	#${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11 # Uncomment for X11

-include ${DEPENDS}

.PHONY: clean

clean:
	- rm ${OBJECTS} ${EXEC} ${DEPENDS}
