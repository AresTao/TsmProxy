UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
endif

LIBRECV = ./libreceiver.a
DLLRECV = ./receiver.so

SRC_LIBRECV = \
	Receiver.C

SRC_DLLRECV = \
	${SRC_LIBRECV}

SRC_ALL = \
	${SRC_DLLRECV}

OBJ_LIBRECV = ${SRC_LIBRECV:.C=.o}
OBJ_DLLRECV = ${SRC_DLLRECV:.C=.o}

UNIFRAMEINC = \
	-I.\
	-I../../xframe/include/comserv\
	-I../../xframe/include/msg\
	-I../../xframe/include/kernel\
	-I../../xframe/include/task\
	-I../../xframe/include/_compat\
        -I../../_lib/proxy/include\
        -I../../jsoncpp/include

CFLAGS = -g ${UNIFRAMEINC} -Wno-deprecated ${X86_64DEF}


lib: ${LIBRECV}

dll: ${DLLRECV}

all: ${LIBRECV} ${DLLRECV}

${LIBRECV}: ${OBJ_LIBRECV}
	ar crv ${LIBRECV} ${OBJ_LIBRECV}
	@echo

${DLLRECV}: ${OBJ_DLLRECV}
	g++ -shared -o ${DLLRECV} ${OBJ_DLLRECV} -DDLL_EXPORTS ../../_lib/proxy/lib/libpsahttp.a -fPIC
#	refcheck .

.C.o:   ${SRC_ALL}
	g++ -o $@ -c ${CFLAGS}  $<

#msgdatadef_sip.C : msgdatadef_sip.def  
#	../../_bin/msc -c . -h . -x . -i ../../../include/msg:../../../include/comserv  -m ../../../include/msg  msgdatadef.def
#	../../_bin/msc -c . msgdatadef_sip.def
#
#msgdef_sip.C : msgdef_sip.def 
#	../../_bin/msc -c . -i ../../../include/msg:../../../include/comserv:../../../include/_compat  msgdef_sip.def
#
#msglib_inter_sip.C : msglib_inter_sip.xml  msg_MSGTYPE_SIP.match\
#	msgdef_sip.def 
#	../../_bin/mlibc -f msglib_inter_sip.xml

clean:
	rm -f ${LIBRECV} ${DLLRECV} ${OBJ_DLLRECV} 

LIBDIR = ../../_lib/proxy/lib
DLLDIR = ../../_bin
install:
	mkdir -p ${LIBDIR}/
	cp ${LIBRECV} ${LIBDIR}/
	mkdir -p ${DLLDIR}/
	mkdir -p ${DLLDIR}/app/

