# qcstyle version
VERSION = 1.1

# customize below to fit your system

# paths
#PREFIX = /usr/local
PREFIX = ${HOME}/.local

# compiler and linker
CXX = g++
CROSS_CXX = x86_64-w64-mingw32-g++

CXXFLAGS = -std=c++14 -Os -Wall -Wextra -DVERSION=\"${VERSION}\"
LDFLAGS = -s
