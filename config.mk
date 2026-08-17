# qcstyle version
VERSION = 1.1

# customize below to fit your system

# paths
#PREFIX = /usr/local
PREFIX = ${HOME}/.local
#MANPREFIX = ${PREFIX}/share/man

# compiler and linker
CXX = g++
CXXFLAGS = -std=c++14 -Os -Wall -Wextra -DVERSION=\"${VERSION}\"
LDFLAGS = -s
