BIN = ./Proxy

SRC_BIN = \
	main.C

SRC_ALL = \
	${SRC_COM}\
	${SRC_BIN}

OBJ_BIN = ${SRC_BIN:.C=.o}

UNIFRAMEINC = \
	-I.\
	-I../_lib/xframe/include/comserv\
	-I../_lib/xframe/include/msg\
	-I../_lib/xframe/include/kernel\
	-I../_lib/xframe/include/task\
	-I../_lib/xframe/include\
        -I../_lib/proxy/include\
	-I./psasip\
	-I./proxytask\
	-I./registertask\
        -I./receiver\
        -I./sender

CFLAGS = -g ${UNIFRAMEINC} -Wno-deprecated


exe: ${BIN}

all: ${BIN}

${BIN}: ${OBJ_BIN}
#	g++ -o ${BIN} ${OBJ_BIN} -L../_lib/resip/lib -lresipares -lrutil -lresip -L../_lib/callserver/lib -lpsasip -lproxytask -lregtask -lmsglib_sip -L../_lib/xframe/lib -lmysqlclient_r -lcrypto -lssl -lxframe
#	refcheck .
#	g++ -o ${BIN} ${OBJ_BIN}  -L../_lib/resip/lib -lresipares -lrutil -lresip -L../_lib/proxy/lib -lreceiver -lpsasip -lregtask -lmsglib_sip -L/usr/local/lib/ -lboost_system -lboost_thread -L../_lib/httpserver/lib -lserver -L../_lib/xframe/lib -lmysqlclient_r -lcrypto -lssl -lxframe 
	g++ -o ${BIN} ${OBJ_BIN} -L../_lib/httpserver/lib /usr/local/lib/libboost_system.a -lpthread ../_lib/proxy/lib/libreceiver.a ../_lib/proxy/lib/libsender.a ../_lib/proxy/lib/libpsahttp.a ../_lib/proxy/lib/libregtask.a -L../_lib/xframe/lib -lmysqlclient_r -lcrypto -lssl -lxframe
.C.o:   ${SRC_ALL}
	g++ -o $@ -c ${CFLAGS}  $<

clean:
	rm -f ${BIN} ${OBJ_BIN} 

DLLDIR = ../_bin
install:
	mkdir -p ${DLLDIR}/
	cp ${BIN} ${DLLDIR}/
