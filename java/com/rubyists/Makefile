#LIB=liburl_escape.so
LIB=liburl_escape.jnilib  # OS X

all: liburl_escape.jnilib URLEscape.class

clean:
	rm -f URLEscape.o URLEscape.class $(LIB)

#URLEscape.h: URLEscape.java URLEscape.class
#	javah URLEscape

URLEscape.o: URLEscape.c URLEscape.h
	gcc -O3 -I/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers -c URLEscape.c

liburl_escape.jnilib: URLEscape.o
	gcc -dynamiclib -framework JavaVM -o $(LIB) URLEscape.o

URLEscape.class: URLEscape.java
	javac URLEscape.java
