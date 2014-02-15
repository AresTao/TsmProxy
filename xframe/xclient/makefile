LIBVER=-x32
UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
LIBVER=-x64
endif

BIN = ./xclient

SRC_BIN = \
	main.C \
	../comserv/comtypedef_vchar.C   ../comserv/fifoabstract.C\
	../comserv/info.C   ../comserv/func.C  \
	../comserv/threadif.C    ../comserv/threadcondition.C\
	../comserv/threadlock.C  ../comserv/threadmutex.C\
	../comserv/socket.C   ../comserv/udpsocket.C\
	../comserv/tinystr.C   ../comserv/tinyxml.C  ../comserv/tinyxmlerror.C	 ../comserv/tinyxmlparser.C\
	getopt_pp/getopt_pp.C
SRC_ALL = \
	${SRC_COM}\
	${SRC_BIN}

OBJ_BIN = ${SRC_BIN:.C=.o}

UNIFRAMEINC = \
	-I.\
	-I./getopt_pp\
	-I../include/comserv


CFLAGS = -g ${UNIFRAMEINC} -Wno-deprecated  ${X86_64DEF}


exe: ${BIN}

all: ${BIN}

${BIN}: ${OBJ_BIN}
	g++ -o ${BIN} ${OBJ_BIN} -lpthread

.C.o:   ${SRC_ALL}
	g++ -o $@ -c ${CFLAGS}  $<

clean:
	rm -f ${BIN} ${OBJ_BIN} 

DLLDIR=../../_lib/xframe
install:
	mkdir -p ${DLLDIR}/bin/
	cp -f ${BIN} ${DLLDIR}/bin/
