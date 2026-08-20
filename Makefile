# qcstyle - trimmed astyle fork
# See LICENSE file for copyright and license details.

include config.mk

SRC = src/ASBeautifier.cpp src/ASEnhancer.cpp src/ASFormatter.cpp src/ASResource.cpp src/ASHelp.cpp src/astyle_main.cpp
OBJ = ${SRC:.cpp=.o}
TARGET = qcstyle

all: ${TARGET}

src/%.o: src/%.cpp
	${CXX} -c ${CXXFLAGS} $< -o $@

# unix
${TARGET}: ${OBJ}
	${CXX} -o $@ ${OBJ} ${LDFLAGS}

# windows
${TARGET}.exe: ${SRC}
	${CROSS_CXX} -std=c++14 -Os -Wall -Wextra -DVERSION=\"${VERSION}\" $^ -o $@ -s -static -static-libgcc -static-libstdc++

win: ${TARGET}.exe

clean:
	rm -f ${TARGET} ${TARGET}.exe ${OBJ}

dist: clean
	mkdir -p ${TARGET}-${VERSION}
	cp -R LICENSE Makefile config.mk README.md src ${TARGET}-${VERSION}
	tar -cf ${TARGET}-${VERSION}.tar ${TARGET}-${VERSION}
	gzip ${TARGET}-${VERSION}.tar
	rm -rf ${TARGET}-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${TARGET} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/${TARGET}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${TARGET}

help:
	@echo "  make            build for current system"
	@echo "  make win        cross-compile for Windows (needs mingw-w64)"
	@echo "  make clean      remove build artifacts"
	@echo "  make install    install to ${PREFIX}/bin"
	@echo "  make dist       create release tarball"

.PHONY: all clean dist install uninstall win help
