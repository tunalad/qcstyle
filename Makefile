# qcstyle - trimmed astyle fork
# See LICENSE file for copyright and license details.

include config.mk

SRC = src/ASBeautifier.cpp src/ASEnhancer.cpp src/ASFormatter.cpp src/ASResource.cpp src/ASHelp.cpp src/astyle_main.cpp
OBJ = ${SRC:.cpp=.o}
TARGET = qcstyle

all: ${TARGET}

.cpp.o:
	${CXX} -c ${CXXFLAGS} $< -o $@

${TARGET}: ${OBJ}
	${CXX} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f ${TARGET} ${OBJ} ${TARGET}-${VERSION}.tar.gz

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

.PHONY: all options clean dist install uninstall
