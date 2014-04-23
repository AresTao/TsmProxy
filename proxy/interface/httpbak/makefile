UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
endif

LIBSMMID = ./libmsglib_http.a
DLLSMMID = ./libmsglib_http.so

SRC_LIBSMMID = \
	msgdef_http.C\

SRC_DLLSMMID = \
	${SRC_LIBSMMID}

SRC_ALL = \
	${SRC_COM}\
	${SRC_DLLSMMID}

OBJ_LIBSMMID = ${SRC_LIBSMMID:.C=.o}
OBJ_DLLSMMID = ${SRC_DLLSMMID:.C=.o}

UNIFRAMEINC = \
	-I.\
	-I../../../xframe/include/comserv\
	-I../../../xframe/include/msg\
	-I../../../xframe/include/_compat\
        -I../../../_lib/proxy/include

CFLAGS = -g ${UNIFRAMEINC} -Wno-deprecated ${X86_64DEF}


lib: ${LIBSMMID}

dll: ${DLLSMMID}

all: ${LIBSMMID} ${DLLSMMID}

${LIBSMMID}: ${OBJ_LIBSMMID}
	ar crv ${LIBSMMID} ${OBJ_LIBSMMID}
	@echo

${DLLSMMID}: ${OBJ_DLLSMMID}
	g++ -shared -o ${DLLSMMID} ${OBJ_DLLSMMID} -DDLL_EXPORTS -L../../../_lib/proxy/lib -lpsahttp
#	refcheck .

.C.o:   ${SRC_ALL}
	g++ -o $@ -c ${CFLAGS}  $<

clean:
	rm -f ${LIBSMMID} ${DLLSMMID} ${OBJ_DLLSMMID} 

INSDIR = ../../../_lib/proxy/include
LIBDIR = ../../../_lib/proxy/lib
DLLDIR = ../../../_bin
install:
	mkdir -p ${INSDIR}/
	cp msgdef_http.h ${INSDIR}/
	mkdir -p ${LIBDIR}/
	cp ${LIBSMMID} ${LIBDIR}/
	cp ${DLLSMMID} ${LIBDIR}/
	mkdir -p ${DLLDIR}/
	mkdir -p ${DLLDIR}/app/
	cp ${DLLSMMID} ${DLLDIR}/app/
