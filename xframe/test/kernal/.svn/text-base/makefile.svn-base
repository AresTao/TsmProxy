BIN = ./kernaltest

SRC_BIN = \
	testselftask.C\
	testrecvtask.C\
	testsendtask.C\
	main.C

SRC_ALL = \
	${SRC_COM}\
	${SRC_BIN}

OBJ_BIN = ${SRC_BIN:.C=.o}

UNIFRAMEINC = \
	-I.\
	-I../../include\
	-I../../include/comserv\
	-I../../include/msg\
	-I../../include/kernel\
	-I../../include/task\
	-I../../include

CFLAGS = -g ${UNIFRAMEINC} -Wno-deprecated


exe: ${BIN}

all: ${BIN}

${BIN}: ${OBJ_BIN}
	g++ -o ${BIN} ${OBJ_BIN} -L../../_bin/lib -lxframe -lcrypto
#	refcheck .

.C.o:   ${SRC_ALL}
	g++ -o $@ -c ${CFLAGS}  $<

clean:
	rm -f ${BIN} ${OBJ_BIN} 

DLLDIR = .
install:
	mkdir -p ${DLLDIR}/
	cp ${BIN} ${DLLDIR}/
