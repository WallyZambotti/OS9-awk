export CC=dcc
export CFLAGS=-r
export YFLAGS=-d
OS9DEST=~/ovcc-ag16/vhds/VCCEmuDisk.vhd,cmds

all:	awk

FILES=cstartdata.r awk.g.r awk.lx.r b.r lib.r main.r parse.r run.r token.r tran.r getlogin.r popen.r perror.r
SOURCE=cstartdata.a awk.g.c awk.lx.c b.c lib.c main.c parse.c run.c token.c tran.c getlogin.c popen.c perror.c

%.r : %.c
	$(CC) $(CFLAGS) $<

%.r : %.a
	$(CC) $(CFLAGS) $<

awk:	$(FILES)
	rlink -M=4K -x -o=awk $(FILES) -l=/dd/lib/mlib.l -l=/dd/lib/clib.l

token.c:	awk.h
	ed - <tokenscript
	rm temp

proctab.c:	proc
	./proc > proctab.c
proc:	awk.h proc.r token.r
	$(CC) -s -f=proc proc.c token.r

os9copy:
	chmod +rwx awk 'awk$$DATA'
	os9 del $(OS9DEST)/awk
	os9 del $(OS9DEST)/'awk$$DATA'	
	os9 copy awk $(OS9DEST)/awk
	os9 copy 'awk$$DATA' $(OS9DEST)/'awk$$DATA'
