APP=ecosim
VERSION=0.01
PREFIX?=/usr/local
SOURCEDIR=src

all:
	gcc -O2 ${COMMANDLINEE} -c ${SOURCEDIR}/eco_system.c -o eco_system.o
	gcc -O2 ${COMMANDLINEE} -c ${SOURCEDIR}/eco_commandline.c -o eco_commandline.o
	gcc -O2 ${COMMANDLINEE} -I/usr/include -o ${APP} *.o -lz -lm -lpthread
	rm *.o
debug:
	gcc -g ${COMMANDLINEE} -c ${SOURCEDIR}/eco_system.c -o eco_system.o
	gcc -g ${COMMANDLINEE} -c ${SOURCEDIR}/eco_commandline.c -o eco_commandline.o
	gcc -g ${COMMANDLINEE} -I/usr/include -o ${APP} *.o -lz -lm -lpthread
	rm *.o
source:
	tar -cvf ../${APP}_${VERSION}.orig.tar ../${APP}-${VERSION} --exclude-vcs
	gzip -f9n ../${APP}_${VERSION}.orig.tar
install:
	mkdir -p ${DESTDIR}/usr
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 755 -s ${APP} ${DESTDIR}${PREFIX}/bin
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man/man1
	install -m 644 man/${APP}.1.gz ${DESTDIR}${PREFIX}/share/man/man1
	install -m 755 --strip ${APP} ${DESTDIR}${PREFIX}/bin
uninstall:
	rm -f ${PREFIX}/share/man/man1/${APP}.1.gz
	rm -rf ${PREFIX}/share/${APP}
	rm -f ${PREFIX}/bin/${APP}
clean:
	rm -f ${APP}
