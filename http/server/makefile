LIBVER=-x32
UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
LIBVER=-x64
endif

LIB_DIR=../../_lib
EXE_DIR=../../_bin
ifndef INSTALL_DIR
INSTALL_DIR=../../_lib/httpserver
endif

INC_SERVER=\
        -I./\
        -I/usr/local/include\

CC=g++
cc=g++
OSFLAGS=

UNICFLAGS=${MG} -Wno-deprecated ${OSFLAGS} $(INC_SERVER) ${X86_64DEF}

SRC_SERVER=\
	connection.cpp   connection_manager.cpp   mime_types.cpp\
	reply.cpp    request_handler.cpp    request_parser.cpp\
	server.cpp
	

# All uniframe sources here.


OBJ_SERVER=\
	${SRC_SERVER:.cpp=.o} 


.cpp.o:   ${SRC_SERVER} 
	${CC} -o $@ -c ${UNICFLAGS}  $<

BINNAME=server
BINNAME_LIB=${EXE_DIR}/lib/libserver.a
BINNAME_SO=${EXE_DIR}/lib/libserver.so

all: 
        
	make ${BINNAME}

${BINNAME}: ${OBJ_SERVER}
	ar crv $(BINNAME_LIB) ${OBJ_SERVER}
	ranlib $(BINNAME_LIB)
	$(CC) -shared -o ${BINNAME_SO} ${OBJ_SERVER}  -export-dynamic 
	tar cf ${EXE_DIR}/include.tar\
		connection.hpp\
		connection_manager.hpp\
                header.hpp\
                mime_types.hpp\
                reply.hpp\
                request_handler.hpp\
                request.hpp\
                request_parser.hpp\
                server.hpp
clean: 
 
	rm -f ${BINNAME_LIB} ${OBJ_SERVER}

install:
	
	mkdir -p ${INSTALL_DIR}
	mkdir -p ${INSTALL_DIR}/bin/
	mkdir -p ${INSTALL_DIR}/include/
	mkdir -p ${INSTALL_DIR}/lib/
	
	tar xf ${EXE_DIR}/include.tar --directory=${INSTALL_DIR}/include/
	cp ${BINNAME_LIB} ${INSTALL_DIR}/lib/
	cp ${BINNAME_SO} ${INSTALL_DIR}/lib/
