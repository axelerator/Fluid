TARGET    = fluid

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
INCLUDES = $(SOURCES:.cpp=.h)

CC = gcc
LD = gcc

CPPFLAGS = -I./
CFLAGS = -O2 -Wall -Wextra -Werror -Wno-unused-parameter -ansi -pedantic

XLIBS = -lXmu -lXi -lX11
GLLIBS = -lglut -lGLU -lGL

# Linker flags
LDFLAGS_LINUX    = -L/usr/X11R6/lib
LDFLAGS_LINUX64  = -L/usr/X11R6/lib64
LDFLAGS_COMMON   =
LDFLAGS          = $(LDFLAGS_COMMON) $(LDFLAGS_$(OS))

# Linker libraries
XLIBS            = -lXmu -lXi -lX11
GLLIBS           = -lglut -lGLU -lGL
LDLIBS           = $(GLLIBS) $(XLIBS) -lm

ifeq ($(findstring Linux,$(shell uname -s)),Linux)
  OS = LINUX
  ifeq ($(shell uname -m),x86_64)
    OS = LINUX64
  endif
endif

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $*.o $*.cpp

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
