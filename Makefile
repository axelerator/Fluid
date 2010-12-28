TARGET    = fluid

SOURCES = $(wildcard *.cpp)

OBJECTS = $(SOURCES:.cpp=.o)
NOVRFOBJECTS = $(SOURCES:.cpp=_novrf.o)

INCLUDES = $(SOURCES:.cpp=.h)
VRFINCLUDES = $(wildcard ./vrf/include/*.h)

CC = g++
LD = g++

ifeq ($(shell uname -m),x86_64)
	ARCH = athlon64
endif

ifeq ($(shell uname -m),i686)
	ARCH = pentium3
endif

OPTIMIZATION = -O3 -march=$(ARCH) -falign-functions=4 -ffast-math -fforce-addr -fprefetch-loop-arrays -funroll-loops -fomit-frame-pointer

CPPFLAGS = -I./
CFLAGS = $(OPTIMIZATION) -Wall -Wextra -Werror -Wno-unused-parameter -ansi -pedantic

# Linker flags
LDFLAGS_LINUX    = -L/usr/X11R6/lib
LDFLAGS_LINUX64  = -L/usr/X11R6/lib64
LDFLAGS_COMMON   = -L./vrf/lib
LDFLAGS          = $(LDFLAGS_COMMON) $(LDFLAGS_$(OS))
LDFLAGSNOVRF	 = $(LDFLAGS_$(OS))

# Linker libraries
XLIBS            = -lX11
GLLIBS           = -lGLEW -lGLU -lGL -lSDL
LDLIBS           = $(GLLIBS) $(XLIBS) -lvrf -lm
NOVRFLDLIBS	 = $(GLLIBS) $(XLIBS) -lm
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

novrf : $(TARGET)_novrf

$(TARGET)_novrf : $(NOVRFOBJECTS)
	$(LD) $(LDFLAGSNOVRF) $(NOVRFOBJECTS) $(NOVRFLDLIBS) -o $(TARGET)_novrf

%_novrf.o : %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -DNOVRF=true -c -o $*_novrf.o $*.cpp

clean:
	rm -f $(NOVRFOBJECTS)
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f $(TARGET)_novrf	
