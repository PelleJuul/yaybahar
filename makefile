CPP=clang++
CPPFLAGS=-std=c++11
LIBS=-lsdl2 -lportaudio -lsndfile -framework OpenGl

# Uncomment this if you want to debug your app.
# CPPFLAGS+=-g

# Uncomment this if you want to optimize your app.
CPPFLAGS+=-O3

# Edit these variables to suit your application.
BIN=yaybahar
SOURCES=$(wildcard source/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
DEPS := $(OBJECTS:.o=.d)

# pal specific variables.
PAL_SOURCES=$(wildcard pal/*.cpp) $(wildcard pal/**/*.cpp)
PAL_HEADERS=$(wildcard pal/*.h) $(wildcard pal/**/*.h)
PAL_OBJECTS=$(patsubst %.cpp, %.o, $(PAL_SOURCES))
PAL_DEPS := $(PAL_OBJECTS:.o=.d)

all: $(BIN)

$(BIN): $(OBJECTS) $(PAL_OBJECTS)
	$(CPP) $(CPPFLAGS) $(PAL_OBJECTS) $(OBJECTS) $(LIBS) -o $(BIN)

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c -MMD -MP $< -o $@

.phony: clean run

run: all
	./$(BIN)

clean:
	rm -f $(PAL_OBJECTS)
	rm -f $(PAL_DEPS)
	rm -f $(OBJECTS)
	rm -f $(DEPS)
	rm -f $(BIN)

-include $(PAL_DEPS)
-include $(DEPS)