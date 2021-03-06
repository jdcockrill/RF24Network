#############################################################################
#
# Makefile for librf24-bcm on Raspberry Pi
#
# License: GPL (General Public License)
# Author:  Charles-Henri Hallard 
# Date:    2013/03/13 
#
# Description:
# ------------
# use make and sudo make install to install the library 
# You can change the install directory by editing the LIBDIR line
#
PREFIX=/usr/local

# Library parameters
# where to put the lib
LIBDIR=$(PREFIX)/lib
# where to put the header files
INCDIR=${PREFIX}/include
# lib name 
LIB_RFN=librf24network
# shared library name
LIBNAME_RFN=$(LIB_RFN).so.1.0

HEADERS=RF24Network_config.h RF24Network.h Sync.h

# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s

# make all
# reinstall the library after each recompilation
all: librf24network

# Make the library
librf24network: RF24Network.o
	g++ -shared -Wl,-soname,$@.so.1 ${CCFLAGS} -o ${LIBNAME_RFN} $^ -lrf24-bcm

rx-example: rx-example.cpp
	g++ ${CCFLAGS} -Wall -lrf24-bcm -lrf24network $^ -o $@
	
# Library parts
RF24Network.o: RF24Network.cpp
	g++ -Wall -fPIC ${CCFLAGS} -lrf24-bcm -c $^

# clear build files
clean:
	rm -rf *.o ${LIB_RFN}.*

# Install the library to LIBPATH
install: 
	@echo "[Install]"
	@if ( test ! -d $(PREFIX)/lib ) ; then mkdir -p $(PREFIX)/lib ; fi
	@install -m 0755 ${LIBNAME_RFN} ${LIBDIR}
	@install ${HEADERS} ${INCDIR}
	@ln -sf ${LIBDIR}/${LIBNAME_RFN} ${LIBDIR}/${LIB_RFN}.so.1
	@ln -sf ${LIBDIR}/${LIBNAME_RFN} ${LIBDIR}/${LIB_RFN}.so
	@ldconfig

