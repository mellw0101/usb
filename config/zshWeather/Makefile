FASTFLAGS = \
-O3 -march=native

CFLAGS = \
-std=c++20 ${FASTFLAGS} -pedantic -Wall -Wno-deprecated-declarations 
CXXFLAGS = \
${CFLAGS}

LIBS = -lcurl
LDFLAGS  = ${LIBS} -flto -O3 -march=native
# -s

cc = clang
CXX = clang++

SRC = zshWether.cpp
OBJ = $(SRC:../src/%.cpp=%.o)
DEPS = $(OBJ:.o=.d)

all: zshWeather

%.o: %.cpp
	${CXX} -c ${CXXFLAGS} $< -o $@ -MMD -MP


zshWeather: $(OBJ)
	${CXX} -o $@ $^ ${LDFLAGS}

clean:

backup:

dist: clean

install: all
	mkdir		-p							/bin
	cp 			zshWeather					/bin/
	chmod		755							/bin/zshWeather

	mkdir		-p							/etc/systemd/system
	cp			weather-fetcher.service		/etc/systemd/system/
	cp			weather-fetcher.timer		/etc/systemd/system/

	systemctl	daemon-reload
	systemctl	enable						weather-fetcher.timer
	systemctl	start						weather-fetcher.timer

uninstall:

.PHONY: all clean dist install uninstall
