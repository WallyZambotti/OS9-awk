dcc -r cstartdata.a
dcc -s -r awk.g.c
dcc -s -r awk.lx.c
dcc -s -r b.c
dcc -s -r lib.c
dcc -s -r main.c
dcc -s -r parse.c
dcc -s -r popen.c
dcc -s -r proctab.c
dcc -s -r run.c
dcc -s -r token.c
dcc -s -r tran.c
dcc -s -r popen.c
dcc -s -r perror.c
dcc -s -r getlogin.c
rlink -x -o=awk cstartdata_int.r awk.g.r awk.lx.r b.r lib.r main.r parse.r run.r token.r tran.r popen.r perror.r getlogin.r -l=/dd/lib/mlib.l -l=/dd/lib/clib.l
cat awk 'awk$DATA' >awk_int
rlink -x -o=awk cstartdata_ext.r awk.g.r awk.lx.r b.r lib.r main.r parse.r run.r token.r tran.r popen.r perror.r getlogin.r -l=/dd/lib/mlib.l -l=/dd/lib/clib.l
