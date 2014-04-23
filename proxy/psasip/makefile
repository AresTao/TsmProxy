UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
endif

LIBSMMID = ./libpsasip.a
DLLSMMID = ./psasip.so

SRC_LIBSMMID = \
	sipdialogmng.C\
	sipoverflow.C\
	sipstackpsa.C

SRC_DLLSMMID = \
	${SRC_LIBSMMID}

SRC_ALL = \
	${SRC_COM}\
	${SRC_DLLSMMID}

OBJ_LIBSMMID = ${SRC_LIBSMMID:.C=.o}
OBJ_DLLSMMID = ${SRC_DLLSMMID:.C=.o}

UNIFRAMEINC = \
	-I.\
	-I../../xframe/include/comserv\
	-I../../xframe/include/msg\
	-I../../xframe/include/kernel\
	-I../../xframe/include/task\
	-I../../xframe/include/_compat\
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
	-I../../_lib/proxy/include

CFLAGS = -g ${UNIFRAMEINC} -Wno-deprecated ${X86_64DEF}


lib: ${LIBSMMID}

dll: ${DLLSMMID}

all: ${LIBSMMID} ${DLLSMMID}

${LIBSMMID}: ${OBJ_LIBSMMID}
	ar crv ${LIBSMMID} ${OBJ_LIBSMMID}
	@echo

${DLLSMMID}: ${OBJ_DLLSMMID}
	g++ -shared -o ${DLLSMMID} ${OBJ_DLLSMMID} -DDLL_EXPORTS ../../_lib/resip/lib/librutil.a  ../../_lib/resip/lib/libresip.a
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
