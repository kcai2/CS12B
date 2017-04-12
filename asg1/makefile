#$Id: makefile,v 1.1 2015-01-20 01:26:51-08 - - $
#Authors:
#Cedric Linares (cslinare@ucsc.edu)
#Kevin Cai (kcai2@ucsc.edu)

export PATH := ${PATH}:/afs/cats.ucsc.edu/courses/cmps012b-wm/bin

JAVASRC    = jcal.java misclib.java
SOURCES    = ${JAVASRC} Makefile README
MAINCLASS  = jcal
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES}
JARFILE    = jcal
LISTING    = Listing.ps
CLASS      = cmps012b-wm.s13
PROJ       = asg1

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm Manifest
	chmod +x ${JARFILE}

%.class : %.java
	- checksource $<
	- cid + $<
	javac $<

clean :
	- rm ${JARCLASSES} Manifest

spotless : clean
	- rm ${JARFILE}

ci : ${SOURCES}
	- checksource ${SOURCES}
	cid + ${SOURCES}

lis : ${SOURCES}
	mkpspdf ${LISTING} ${SOURCES}

submit : ${SOURCES}
	submit ${CLASS} ${PROJ} ${SOURCES}
	ls -goatr /afs/cats.ucsc.edu/class/${CLASS}/${PROJ}/${USER}
	ls -goatr ${SOURCES}
	
again:
	gmake --no-print-directory spotless ci all lis
