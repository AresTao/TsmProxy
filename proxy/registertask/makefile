UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
endif

LIBSMMID = ./libregtask.a
DLLSMMID = ./regtask.so

SRC_LIBSMMID = \
	RegisterTask.C\
	#RegisterCS.C

SRC_DLLSMMID = \
	${SRC_LIBSMMID}

SRC_ALL = \
	${SRC_COM}\
	${SRC_DLLSMMID}

OBJ_LIBSMMID = ${SRC_LIBSMMID:.C=.o}
OBJ_DLLSMMID = ${SRC_DLLSMMID:.C=.o}

UNIFRAMEINC = \
	-I.\
	-I../../_lib/xframe/include/comserv\
	-I../../_lib/xframe/include/msg\
	-I../../_lib/xframe/include/kernel\
	-I../../_lib/xframe/include/task\
	-I../../_lib/xframe/include/mysql\
	-I../../_lib/xframe/include\
        -I../../_lib/resip/include\
        -I../../_lib/resip/include/rutil\
	-I../../_lib/resip/include/rutil/ssl\
        -I../../_lib/resip/include/rutil/dns\
        -I../../_lib/resip/include/rutil/stun\
        -I../../_lib/resip/include/resip/stack\
        -I../../_lib/resip/include/resip/stack/ssl\
        -I../../_lib/resip/include/resip/dum\
        -I../../_lib/resip/include/contrib\
        -I../../_lib/resip/include/contrib/ares\
	-I../../_lib/proxy/include\

CFLAGS = -g ${UNIFRAMEINC} -Wno-deprecated ${X86_64DEF}


lib: ${LIBSMMID}

dll: ${DLLSMMID}

all: ${LIBSMMID} ${DLLSMMID}

${LIBSMMID}: ${OBJ_LIBSMMID}
	ar crv ${LIBSMMID} ${OBJ_LIBSMMID}
	@echo

${DLLSMMID}: ${OBJ_DLLSMMID}
	g++ -shared -o ${DLLSMMID} ${OBJ_DLLSMMID} -DDLL_EXPORTS -L../../_lib/resip/lib -lrutil -lresip ../../_lib/proxy/lib/libpsahttp.a
#	refcheck .

.C.o:   ${SRC_ALL}
	g++ -o $@ -c ${CFLAGS}  $<

clean:
	rm -f ${LIBSMMID} ${DLLSMMID} ${OBJ_DLLSMMID} 

LIBDIR = ../../_lib/proxy/lib
DLLDIR = ../../_bin
install:
	mkdir -p ${LIBDIR}/
	cp ${LIBSMMID} ${LIBDIR}/
	cp ${DLLSMMID} ${LIBDIR}/
	mkdir -p ${DLLDIR}/
	mkdir -p ${DLLDIR}/app/
	cp ${DLLSMMID} ${DLLDIR}/app/
