###############################################################################
#
#    makefile for building Uninetssc under Linux
#
#    copyright(c) 2005-2008, by MT2.
#    all rights reserved. 

#    author: Li Jinglin
#    e-mail: jlli@mt2.com.cn
#    date  : 2006-5-11 
###############################################################################

UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
endif

CC=g++

OUTDIR=../../_lib

INC_PSA_HTTP=-I.

ifeq ($(ILD),uni)
INC_UNI_DIR = ${UNIDIR}/include
else
INC_UNI_DIR = ../../_lib/xframe/include
endif

INC_UNIFRAME= -I${INC_UNI_DIR}/msg -I${INC_UNI_DIR}/comserv -I${INC_UNI_DIR}/kernel -I${INC_UNI_DIR}/task 

INC_ALL= $(INC_PSA_HTTP) $(INC_UNIFRAME) 

CFLAGS=$(INC_ALL) -DDLL_EXPORTS -Wno-deprecated -g ${X86_64DEF}

SRC_C=\
	httpserver.C\
	httpclient.C\
	httpconn.C\
	httpmessage.C\
	httpthread.C\
	httphandler.C


SRC_C_ALL = ${SRC_C}  

OBJ=${SRC_C:.C=.o}
OBJ_TEST=${SRC_C_TEST:.C=.o}
	
SO=./psahttp.so

LIB=./libpsahttp.a

all: 	${SO} ${LIB} 

${SO}: ${OBJ}
	$(CC) -shared -o ${SO} ${OBJ}

${LIB}: ${OBJ}
	ar crv ${LIB} ${OBJ}
	tar cf ./include.tar\
		httpserver.h\
		httpconn.h\
		httpclient.h\
		httpmessage.h\
		httpthread.h\
		httphandler.h

.C.o:	${SRC_C_ALL}
	${CC} -o $@ -c ${CFLAGS}  $<

clean:
	rm -f ${SO} ${OBJ} ${SO_TEST} ${OBJ_TEST}

install:
	rm -f ${UNIDIR}/app/psahttp.so
	cp ${SO} ${OUTDIR}/proxy/lib
	cp ${LIB} ${OUTDIR}/proxy/lib
	tar xf ./include.tar --directory=${OUTDIR}/proxy/include/
