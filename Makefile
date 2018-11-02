# Makefile
# 04-Jan-15 Markku-Juhani O. Saarinen <mjos@iki.fi>

BIN	= xtest
OBJS	= main.o \
	kuznechik_8bit.o\
	mycrypt.o


DIST	= kuznechik

CC	= g++
CFLAGS	= -Wall -Ofast -march=native
LIBS	=
LDFLAGS	=
INCS	=

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -rf $(DIST)-*.t?z $(OBJS) $(BIN) *~

dist:	clean
	cd ..; \
	tar cfvJ $(DIST)/$(DIST)-`date -u "+%Y%m%d%H%M00"`.txz $(DIST)/*
