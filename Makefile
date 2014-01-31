.DEFAULT_GOAL = all

CXX = g++
CXXFLAGS = -g -O2
CPPFLAGS =
LDFLAGS =
LDLIBS =
FLEX = flex

WARNINGS = -Wall -Wextra
CXXFLAGS += ${WARNINGS}

# GNU make allows pattern rules to work like this, but not normal rules
%.hpp %.cpp : %.lpp
	${FLEX} --header-file=$*.hpp -o $*.cpp $<

%.o : %.cpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c -o $@ $<
% : %.o
	${CXX} ${LDFLAGS} $^ ${LDLIBS} -o $@

all: c2html.html

c2html: c2html.o c-lex.o
c-lex.o: c-lex.cpp c-lex.hpp
c2html.o: c2html.cpp c-lex.hpp

clean:
	rm -f c2html c2html.html *.o
distclean:
	rm -f c-lex.[ch]pp

%.html : %.cpp c2html
	./c2html $< > $@

tar:
	git archive --prefix=c2html/ -o c2html.tar.gz HEAD
