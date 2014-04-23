UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
endif

LIBHTTP = libmsglib_http.a
DLLHTTP = msglib_http.so

SRC_LIBHTTP = \
	msgdef_http.C

SRC_DLLHTTP = \
	${SRC_LIBHTTP}\
	msglib_inter_http.C

SRC_ALL = \
	${SRC_DLLHTTP}

OBJ_LIBHTTP = ${SRC_LIBHTTP:.C=.o}
OBJ_DLLHTTP = ${SRC_DLLHTTP:.C=.o}

ifeq ($(ILD),uni)
INCDIR = ${UNIDIR}/include
else
INCDIR = ../../../include
endif

UNIFRAMEINC = \
        -I${INCDIR}/comserv\
        -I${INCDIR}/kernel\
        -I${INCDIR}/msg

CFLAGS = -g -DDLL_EXPORTS ${UNIFRAMEINC} -Wno-deprecated ${X86_64DEF}

all: ${LIBHTTP} ${DLLHTTP}

${LIBHTTP}: ${OBJ_LIBHTTP}
	ar crv ${LIBHTTP} ${OBJ_LIBHTTP}
	@echo

${DLLHTTP}: ${OBJ_DLLHTTP}
	g++ -shared -o ${DLLHTTP} ${OBJ_DLLHTTP}
	refcheck .

.C.o:   ${SRC_ALL}
	g++ -o $@ -c ${CFLAGS}  $<

msgdef_http.C : msgdef_http.def 
	msc -c . msgdef_http.def

msglib_inter_http.C : msglib_inter_http.xml  msg_MSGTYPE_HTTP.match msgdef_http.def 
	mlibc -f msglib_inter_http.xml

clean:
	rm -f ${LIBHTTP} ${DLLHTTP} ${OBJ_DLLHTTP} \
	msgdef_http.h* msgdef_http.xsd* msgdef_http.C* \
	msglib_inter_http.h* msglib_inter_http.C*

INSTALLDIR = ${UNIDIR}/include/interface/http
install:
	rm -rf ${INSTALLDIR}
	mkdir -p ${INSTALLDIR}
	cp msgdef_http.h ${INSTALLDIR}
	cp msglib_inter_http.h ${INSTALLDIR}
	cp ${LIBHTTP} ${UNIDIR}/lib
	cp ${DLLHTTP} ${UNIDIR}/app 
