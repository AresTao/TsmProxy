LIBVER=-x32
UNAME_I=${shell uname -i}
ifeq ($(UNAME_I), x86_64)
X86_64DEF=-fPIC -DX86_64
LIBVER=-x64
endif

###############################################################################
#
#    makefile for building xFrame LIB under Linux
#
#    copyright(c) 2005-2013, by BUPT.
#    all rights reserved. 
###############################################################################

###############################################################################
# Section 0: Common Settings Here which is defined in makefile.common
###############################################################################

LIB_DIR=./_lib
EXE_DIR=./_bin

SQLITEVER=sqlite3
ifndef ORACLEVER
ORACLEVER=oracle10
endif
ifndef MYSQLVER
MYSQLVER=mysql5.1
endif
ifndef DBVER_STR
DBVER_STR=m51_o10
endif
ifndef INSTALL_DIR
INSTALL_DIR=../_lib/xframe
endif

ORACLE_DIR=$(LIB_DIR)/${ORACLEVER}${LIBVER}
MYSQL_DIR=$(LIB_DIR)/${MYSQLVER}${LIBVER}
SQLITE_DIR=$(LIB_DIR)/${SQLITEVER}${LIBVER}


OPENSSL_DIR=$(LIB_DIR)/openssl1.0.0${LIBVER}

###############################################################################
# Set all includes here
###############################################################################
INC_DB_MYSQL   = -I$(MYSQL_DIR)/include
INC_DB_ORACLE  = -I$(ORACLE_DIR)/include
INC_DB_SQLITE  = -I$(SQLITE_DIR)/include
INC_DB	       = $(INC_DB_MYSQL) 
INC_OPENSSL    = -I$(OPENSSL_DIR)/include

INC_UNIFRAME=\
	-I./\
	-Iinclude/\
	-Iinclude/comserv\
	-Iinclude/kernel\
	-Iinclude/task\
	-Iinclude/msg\
	-Iinclude/_compat\
	${INC_DB}\
	${INC_OPENSSL}

###############################################################################
# Choose one of the following segments about machine
#   use '#' to disable that is not expected.
###############################################################################
# the following lines for SUN
#CC=CC
#cc=cc
#OSFLAGS=
#SYSLIB=-lsocket -lnsl

# the following lines for HP
#CC=aCC
#cc=aCC 
#OSFLAGS=-DHPOS -D_HP_aCC_NonAA 
#SYSLIB=-lnsl -lsec -lpthread -lrt

# the following lines for Linux
CC=g++
cc=g++
OSFLAGS=
SYSLIB=-lrt -L$(OPENSSL_DIR) -lssl -lcrypto

###############################################################################
# Set the compiler FLAGS for  ALL Platforms
# !! You'd better not modify it !!
###############################################################################

UNICFLAGS=${MG} -Wno-deprecated ${OSFLAGS} $(INC_UNIFRAME) ${X86_64DEF}

###############################################################################
# Set all Basic libs  here
#   °üÀ¨ ÏµÍ³LIBÉèÖÃ£¬ UniFrame LIBÉèÖÃ
###############################################################################
# database LIB
DBLIB_MYSQL  = -L$(MYSQL_DIR) -lmysqlclient_r

ifeq ($(ORACLEVER),oracle10)
DBLIB_ORACLE = -L$(ORACLE_DIR) -lclntsh -lnnz10 
else
DBLIB_ORACLE = -L$(ORACLE_DIR) -lclntsh -lwtc9
endif

DBLIB        = $(DBLIB_MYSQL) $

###############################################################################
# Section 5:  Specify all SOURCEs 
#  Add SRC_C_xxx for the CPP moudles which you are dealing with
#  Add SRC_c_xxx for the C moudles which you are dealing with
###############################################################################
# The following is for UniFrame Source
SRC_C_COMSERV=\
	comserv/base64.C   comserv/cmd5.C   comserv/comtypedef_vchar.C\
	comserv/dataconvert.C   comserv/db.C    comserv/dbmysql.C\
	comserv/des.C	 comserv/env.C		comserv/fifoabstract.C\
	comserv/func.C	  comserv/hashtable.C	   comserv/info.C\
	comserv/socket.C   comserv/udpsocket.C\
	comserv/threadcondition.C  comserv/threadif.C	    comserv/threadlock.C\
	comserv/threadmutex.C	   comserv/threadmutexRW.C  comserv/threadmutexRecursive.C\
	comserv/tinystr.C   comserv/tinyxml.C  comserv/tinyxmlerror.C	 comserv/tinyxmlparser.C
	
SRC_C_TASK=\
	task/abstracttask.C	  task/taskthread.C

SRC_C_MSG=\
	msg/msg.C	msg/msgutil.C	msg/msgdatadef.C	msg/msgdef.C

SRC_C_KERNEL=\
	kernel/framectrl.C	kernel/generalobj.C	kernel/generalthread.C\
	kernel/kernalthread.C  kernel/taskselector.C\
	kernel/timer.C    kernel/timerqueue.C  framemng.C

# All uniframe sources here.

SRC_C=\
	${SRC_C_COMSERV}\
	${SRC_C_MSG}\
	${SRC_C_TASK}\
	${SRC_C_KERNEL}


SRC_XFRAME=\
	${SRC_C_COMSERV}\
	${SRC_C_MSG}\
	${SRC_C_TASK}\
	${SRC_C_KERNEL}
	
SRC_XFRAME_c=

OBJ_XFRAME=\
	${SRC_XFRAME:.C=.o} \
	$(SRC_XFRAME_c:.c=.o)

OBJ_COMSERV=\
	${SRC_C_COMSERV:.C=.o}

.C.o:   ${SRC_C} 
	${CC} -o $@ -c ${UNICFLAGS}  $<

.c.o:  ${SRC_c}
	${cc} -o $@ -c ${UNICFLAGS}  $<

###############################################################################
# Section 6:  Specify Out Put Files here
###############################################################################

# --------------BIN file name here------------------------------
BINNAME=xframe
BINNAME_EXE=${EXE_DIR}/bin/xframe
BINNAME_SO=${EXE_DIR}/lib/libxframe.so
BINNAME_LIB=${EXE_DIR}/lib/libxframe.a
COMSERV_LIB=${EXE_DIR}/lib/libxcomserv.a
COMSERV_SO=${EXE_DIR}/lib/libxcomserv.so
###############################################################################
#  Section 7: Begin to build !                                                # 
#   !! Do not modify it !!                                                    #
###############################################################################

SVNINFO="\"`LANG=;svn info | grep Revision | cut -b11-`\""

all: 
	echo "#ifndef _VERSION_H" > include/kernel/version.h
	echo "#define _VERSION_H" >> include/kernel/version.h
	echo "#define SVNVERSION "${SVNINFO} >> include/kernel/version.h
	echo "#define DBVERSION \""${MYSQLVER}", "${ORACLEVER}", "${SQLITEVER}"\"" >> include/kernel/version.h
	echo "#define DBVERSION_B \""${DBVER_STR}"\"" >> include/kernel/version.h
	echo "#endif" >> include/kernel/version.h
	g++ -o ./_bin/xframev -Iinclude/kernel xframev.C -ldl
	make ${BINNAME}
	(cd xclient && make all)

${BINNAME}: ${OBJ_XFRAME} ${OBJ_COMSERV}
	ar crv $(COMSERV_LIB) ${OBJ_COMSERV}
	ranlib $(COMSERV_LIB)
	#$(CC) -o ${BINNAME_EXE} ${OBJ} ${DBLIB} ${SYSLIB} -Wl,-export-dynamic
	ar crv $(BINNAME_LIB) ${OBJ_XFRAME}
	ranlib $(BINNAME_LIB)
	$(CC) -shared -o ${BINNAME_SO} ${OBJ_XFRAME} ${DBLIB} ${SYSLIB} -export-dynamic 
	$(CC) -shared -o ${COMSERV_SO} ${OBJ_COMSERV} ${DBLIB} ${SYSLIB} -export-dynamic
	tar cf ${EXE_DIR}/include.tar\
		include/framemng.h\
		include/kernel/version.h\
		include/kernel/framectrl.h\
		include/kernel/generalobj.h\
		include/kernel/generalthread.h\
		include/kernel/kernalthread.h\
		include/kernel/taskselector.h\
		include/kernel/timerqueue.h\
		include/kernel/timer.h\
		include/task/abstracttask.h\
		include/task/taskthread.h\
		include/msg/msgnamedef.h\
		include/msg/msgdatadef.h\
		include/msg/msgutil.h\
		include/msg/msgdef.h\
		include/msg/msg.h\
		include/msg/xmlmsgconvertor.h\
		include/comserv/base64.h\
		include/comserv/cexpirebuffer.h\
		include/comserv/clist.h\
		include/comserv/cmd5.h\
		include/comserv/comconst.h\
		include/comserv/comtypedef.h\
		include/comserv/comtypedef_compat.h\
		include/comserv/comtypedef_fchar.h\
		include/comserv/comtypedef_vchar.h\
		include/comserv/cvector.h\
		include/comserv/defs.h\
		include/comserv/dataconvert.h\
		include/comserv/db.h\
		include/comserv/dbmysql.h\
		include/comserv/defs.h\
		include/comserv/env.h\
		include/comserv/fdset.h\
		include/comserv/fifo.h\
		include/comserv/fifoabstract.h\
		include/comserv/fifotimelimit.h\
		include/comserv/fifosorted.h\
		include/comserv/func.h\
		include/comserv/info.h\
		include/comserv/socket.h\
		include/comserv/threadif.h\
		include/comserv/threadcondition.h\
		include/comserv/threadlockable.h\
		include/comserv/threadlock.h\
		include/comserv/threadmutex.h\
		include/comserv/threadmutexRW.h\
		include/comserv/threadcondition.h\
		include/comserv/threadlock.h\
		include/comserv/tinystr.h\
		include/comserv/tinyxml.h\
		include/comserv/udpsocket.h\
		include/comserv/unihashtable.h\
		include/_compat/msgdef_uninet.h

clean:
	rm -f ${BINNAME_EXE} ${BINNAME_SO} ${BINNAME_LIB} ${COMSERV_LIB} ${OBJ_XFRAME} ${OBJ_COMSERV} ${EXE_DIR}/include.tar \
		include/msg/msgdatadef.xsd*\
		include/kernel/version.h\
		_bin/linux/xframev
	(cd xclient && make clean)

install:
	mkdir -p ${INSTALL_DIR}
	mkdir -p ${INSTALL_DIR}/bin/
	cp _bin/xframev ${INSTALL_DIR}/bin/
	rm -rf ${INSTALL_DIR}/include
	tar xf ${EXE_DIR}/include.tar --directory=${INSTALL_DIR}/
	mkdir -p ${INSTALL_DIR}/include/mysql
	mkdir -p ${INSTALL_DIR}/include/openssl
	mkdir -p ${INSTALL_DIR}/lib/
	mkdir -p ${INSTALL_DIR}/lib/mysql
	mkdir -p ${INSTALL_DIR}/lib/openssl
	cp ${BINNAME_LIB} ${INSTALL_DIR}/lib/
	cp ${COMSERV_LIB} ${INSTALL_DIR}/lib/
	cp ${BINNAME_SO} ${INSTALL_DIR}/lib/
	cp ${COMSERV_SO} ${INSTALL_DIR}/lib/
	cp _lib/${MYSQLVER}${LIBVER}/include/*  ${INSTALL_DIR}/include/mysql/ -R
	cp _lib/${MYSQLVER}${LIBVER}/lib*  ${INSTALL_DIR}/lib/
	cp _lib/openssl1.0.0${LIBVER}/include/openssl  ${INSTALL_DIR}/include/ -R
	cp _lib/openssl1.0.0${LIBVER}/lib* ${INSTALL_DIR}/lib/
	cp refcheck ${INSTALL_DIR}/bin
	chmod 755 ${INSTALL_DIR}/bin/refcheck
	(cd xclient && make install)	
# --------- end of makefile -----------------------
