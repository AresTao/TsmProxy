UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
endif

LIBSMMID = ./libmsglib_sip.a
DLLSMMID = ./libmsglib_sip.so

SRC_LIBSMMID = \
	msgdef_sip.C\
	msghelper_sip.C

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
        -I../../../_lib/resip/include\
        -I../../../_lib/resip/include/rutil\
	-I../../../_lib/resip/include/rutil/ssl\
        -I../../../_lib/resip/include/rutil/dns\
        -I../../../_lib/resip/include/rutil/stun\
        -I../../../_lib/resip/include/resip/stack\
        -I../../../_lib/resip/include/resip/stack/ssl\
        -I../../../_lib/resip/include/resip/dum\
        -I../../../_lib/resip/include/contrib\
        -I../../../_lib/resip/include/contrib/ares

CFLAGS = -g ${UNIFRAMEINC} -Wno-deprecated ${X86_64DEF}


lib: ${LIBSMMID}

dll: ${DLLSMMID}

all: ${LIBSMMID} ${DLLSMMID}

${LIBSMMID}: ${OBJ_LIBSMMID}
	ar crv ${LIBSMMID} ${OBJ_LIBSMMID}
	@echo

${DLLSMMID}: ${OBJ_DLLSMMID}
	g++ -shared -o ${DLLSMMID} ${OBJ_DLLSMMID} -DDLL_EXPORTS -L../../../_lib/resip/lib -lrutil -lresip
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
	cp msgdef_sip.h ${INSDIR}/
	cp msgconstdef_sip.h ${INSDIR}/
	cp msghelper_sip.h ${INSDIR}/
	mkdir -p ${LIBDIR}/
	cp ${LIBSMMID} ${LIBDIR}/
	cp ${DLLSMMID} ${LIBDIR}/
	mkdir -p ${DLLDIR}/
	mkdir -p ${DLLDIR}/app/
	cp ${DLLSMMID} ${DLLDIR}/app/
