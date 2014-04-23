help()
{
   echo
   echo "Usage: build all [m51] [o10] [g]"
   echo "                 [m50] [o9]"
   echo "       build clean"
   echo "       build install [install dir]"
   echo
}

BUILD_PWD=$PWD
PUBLISH_DIR=$PWD/_bin
if [ $# -eq 0 ]; then
   help;
else
   MYSQLVER=mysql5.1
   MYSQLVER_B=m51
   ORACLEVER=oracle10
   ORACLEVER_B=o10
   case "$1"
   in
      all)
         for i in $*; do
            case "$i"
            in
               all|clean|clean.all|install|pack)
                  ;;
               o9)
                  ORACLEVER=oracle9;
                  ORACLEVER_B=o9;;
	       o10)
		  ORACLEVER=oracle10;
		  ORACLEVER_B=o10;;
               m50)
                  MYSQLVER=mysql5.0;
                  MYSQLVER_B=m50;;
	       m51)
		 MYSQLVER=mysql5.1;
		 MYSQLVER_B=m51;;
               g)
                  M_G="MG=-g";;
               *)
                  help;
                  exit 1;;
            esac;
         done

         cd xframe;
         echo;
         echo "======== build xframe ( "$MYSQLVER", "$ORACLEVER") ====================";
         make $M_G MYSQLVER=$MYSQLVER ORACLEVER=$ORACLEVER DBVER_STR=$MYSQLVER_B"_"$ORACLEVER_B INSTALL_DIR=../_lib/xframe;
         if [ $? -ne 0 ]
         then
           exit 1;
         fi
	 make install;
	 cd ..;

         echo;
         echo "======== build jsoncpp       ====================";
         
         cd jsoncpp/;
         python /usr/bin/scons platform=linux-gcc;
         mkdir -p ../_lib/json/;
         mkdir -p ../_lib/json/include;
         cp -r ./include/ ../_lib/json/;
         cd ..;

         echo;
         echo "======== build interface sip ====================";
	 cd proxy/interface/sip;
	 make all;
	 if [ $? -ne 0 ]
         then
           exit 1;
         fi
 	 make install;
	 cd -;

         echo;
         echo "======== build psasip ====================";

	 cd proxy/psasip;
	 make all;
         if [ $? -ne 0 ]
         then
           exit 1;
         fi
	 make install;
	 cd -;

         echo;
         echo "======== build register task ====================";

	 cd proxy/registertask;
	 make all;
         if [ $? -ne 0 ]
         then
           exit 1;
         fi
	 make install;
	 cd -;

         echo "======== build callserver  ====================";

	 cd proxy
	 make all;
         if [ $? -ne 0 ]
         then
           exit 1;
         fi
         make install;
         cd -;

         echo;
         echo "======== build fackappserver  ====================";


	;;
      clean|clean.all)
         cd xframe;
         make clean;
         cd ..;
	
         cd proxy/interface/sip;
         make clean;
         cd -;

         cd proxy/psasip;
         make clean;
         cd -;

         cd proxy/registertask;
         make clean;
         cd -;

         cd proxy/;
         make clean;
	 cd -;

	 rm -Rf $PUBLISH_DIR/app/*
	 rm -Rf $PUBLISH_DIR/lib/*
	 rm -Rf $PUBLISH_DIR/bin/*
	 rm -Rf $PUBLISH_DIR/log/*
	 rm -Rf $PUBLISH_DIR/shell/*
	 rm -f $PUBLISH_DIR/Proxy
	 rm -f $PUBLISH_DIR/reTurnServer
	 rm -f $PUBLISH_DIR/AppServer
	 rm -f $PUBLISH_DIR/install.sh
	;;
      install)
        if [ $# -eq 2 ]; then
            echo
            echo Install to $2
            echo
	    INSTALL_DIR=$2
	else
	    INSTALL_DIR=$PUBLISH_DIR
        fi

	mkdir -p $INSTALL_DIR/
        mkdir -p $INSTALL_DIR/etc/
        mkdir -p $INSTALL_DIR/lib/
        mkdir -p $INSTALL_DIR/log/
	rm -Rf $INSTALL_DIR/log/*
        mkdir -p $INSTALL_DIR/bin/
        mkdir -p $INSTALL_DIR/app/
	mkdir -p $INSTALL_DIR/shell/

        if [ $INSTALL_DIR != $PUBLISH_DIR ]
        then
	        mkdir -p $INSTALL_DIR/
		rm -Rf $INSTALL_DIR/*
	        mkdir -p $INSTALL_DIR/etc/
        	mkdir -p $INSTALL_DIR/lib/
	        mkdir -p $INSTALL_DIR/log/
	        mkdir -p $INSTALL_DIR/bin/
        	mkdir -p $INSTALL_DIR/app/
		mkdir -p $INSTALL_DIR/shell/
		cp $PUBLISH_DIR/app/* $INSTALL_DIR/app/
		cp $PUBLISH_DIR/etc/* $INSTALL_DIR/etc/
		cp $PUBLISH_DIR/lib/* $INSTALL_DIR/lib/
		cp $PUBLISH_DIR/Proxy  $INSTALL_DIR/
	fi
        cp ./_lib/xframe/lib/libxframe.so $INSTALL_DIR/lib/
        cp ./_lib/xframe/lib/libxcomserv.so $INSTALL_DIR/lib/
	cp ./_lib/xframe/bin/xframev	$INSTALL_DIR/bin/
	cp ./_lib/xframe/bin/xclient    $INSTALL_DIR/bin/
        cp ./_lib/resip/lib/libresip-1.8.so $INSTALL_DIR/lib/
        cp ./_lib/resip/lib/libresipares-1.8.so $INSTALL_DIR/lib/
        cp ./_lib/resip/lib/librutil-1.8.so $INSTALL_DIR/lib/
        cp ./_lib/resip/lib/libdum-1.8.so $INSTALL_DIR/lib/
        cp ./_lib/resip/lib/libreTurnClient-1.8.so $INSTALL_DIR/lib/
	cp ./_lib/resip/sbin/reTurnServer  $INSTALL_DIR/
	cp ./jsoncpp/libs/linux-gcc-4.4.7/*.a $INSTALL_DIR/lib/
	cp ./jsoncpp/libs/linux-gcc-4.4.7/*.so $INSTALL_DIR/lib/
        cp /usr/local/lib/libboost_system.so $INSTALL_DIR/lib/
        cp /usr/local/lib/libboost_system.a $INSTALL_DIR/lib/
        cp /usr/local/lib/libboost_system.so.1.55.0 $INSTALL_DIR/lib/
        cp ./_lib/httpserver/lib/libserver.so $INSTALL_DIR/lib/
        cp ./_lib/httpserver/lib/libserver.a $INSTALL_DIR/lib/
        # SVNVERSION=`LANG=;svn info | grep Revision | cut -b11-`
        # OSNAME=`uname -s`
        # FILENAME="callserver_"$SVNVERSION"_"$OSNAME"_"`$INSTALL_DIR/bin/xframev b`
	 
	# echo "FILENAME=$FILENAME" >> $INSTALL_DIR/install.sh
	# cat ./_shell/install.m >> $INSTALL_DIR/install.sh
	# chmod 755 $INSTALL_DIR/install.sh
	 
	# cp ./_shell/watcher.m  $INSTALL_DIR/shell/
	
	# cd ${INSTALL_DIR};
	# rm -f $FILENAME.tgz
	# tar cvfz $FILENAME.tgz  bin lib app etc log shell CallServer install.sh reTurnServer --exclude=.svn;
	# cd -
 
         echo install and pack ok;
	;;
      *)
         help;
	;;
   esac
fi

