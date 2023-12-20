# dwm - dynamic window manager
# See LICENSE file for copyright and license details.
VERSION = M
X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# Xinerama, comment if you don't want it
# XINERAMALIBS  = -lXinerama
# XINERAMAFLAGS = -DXINERAMA

# freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include/freetype2
# OpenBSD (uncomment)
# FREETYPEINC = ${X11INC}/freetype2

# includes and libs
INCS = -I${X11INC} -I${FREETYPEINC} -I../
LIBS = -L${X11LIB} -lX11 ${XINERAMALIBS} ${FREETYPELIBS}

# flags
CPPFLAGS = \
# -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}

FASTFLAGS = \
-O3 -march=native -funroll-loops -Rpass=loop-vectorize -flto

CFLAGS = \
-std=c++20 -pedantic -Wall -Wno-deprecated-declarations ${INCS} ${CPPFLAGS} ${FASTFLAGS}
CXXFLAGS = \
${CFLAGS}

LDFLAGS  = ${LIBS} -flto -O3
# -s

cc = clang
CXX = clang++

SRC = ../src/drw.cpp ../src/dwm.cpp ../src/util.cpp
# ../src/titleBar.cpp
OBJ = $(SRC:../src/%.cpp=%.o)
DEPS = $(OBJ:.o=.d)

all: dwm

%.o: ../src/%.cpp
	${CXX} -c ${CXXFLAGS} $< -o $@ -MMD -MP

# drw.o:
# 	${CXX} -c ${CXXFLAGS} ../src/drw.cpp $<
#
# dwm.o:
# 	${CXX} -c ${CXXFLAGS} ../src/dwm.cpp $<
#
# util.o:
# 	${CXX} -c ${CXXFLAGS} ../src/util.cpp $<
#
# FileHandler.o:
# 	${CXX} -c FileHandler/FileHandler.hpp $<


dwm: $(OBJ)
	${CXX} -o $@ $^ ${LDFLAGS}

clean:
	rm -f 	dwm.o \
		drw.o \
		util.o

backup:
	cp -f dwm-backup /bin/dwm

dist: clean


install: all
	mkdir 		-p 				/bin
	cp 		-f 	../../XGaming 		/bin
	cp 		-f 	../../dwm-X 		/bin
	cp 		-f 	../../dwm-KILL 		/bin
	cp 		-f 	dwm 			/bin

	chmod 		755 				/bin/XGaming
	chmod 		755 				/bin/dwm-X
	chmod 		755 				/bin/dwm-KILL
	chmod 		755 				/bin/dwm

	mkdir 		-p 				/etc/systemd/system
	cp 		-f 	../../dwm.service 	/etc/systemd/system/dwm.service
	cp 		-f 	../../dwm-X.service 	/etc/systemd/system/dwm-X.service

	mkdir 		-p 				/etc/X11
	cp 		-f 	../../xorg.conf 	/etc/X11/xorg.conf
	cp 		-f 	../../Xwrapper.config 	/etc/X11/Xwrapper.config

	mkdir 		-p 				/etc/polkit-1/rules.d
	cp 		-f 	../../dwm.rules 	/etc/polkit-1/rules.d/dwm.rules

	systemctl 	restart 			polkit
	systemctl 	daemon-reload

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1

.PHONY: all clean dist install uninstall
