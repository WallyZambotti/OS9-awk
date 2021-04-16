# OS9-Awk

A port of the Unix V7/32V version of awk.

Hand built for the CoCo OS9/Nitros9 Level II.

Due to memory requirements will only run on OS9 level II.

Provides all the original files from the Unix archive.

https://minnie.tuhs.org/cgi-bin/utree.pl?file=V7/usr/src/cmd/awk

The lex and yacc generated files were generated on a Linux cross development system using the
Unix V7/32V versions of lex and yacc.  However they have been further hand modified to work 
under OS9.  They can be rebuilt under OS9 with lex and yacc but as there is no ed for OS9
there is little point.

Also there is no version of ed for OS9 to custom build token.c so this was also built on
the cross development system.

Because of this the OS9 makefile does not attempt to recreate the awk.g.c & awk.lx.c from the
yacc & lex scripts or token.c from the ed script.

In addtion proctab.c which is created by proc has been hand optimized to mimimize the memory
foot print.  So while proc (from this repo) does build and work on OS9 this step has been
ommitted for simplicity.

The makefile thus represents the final stage of the build after yacc,lex, ed & proc have already
been run and awk.g.c, awk.lx.c, token.c & proctab.c have already been generated.

Additional files popen.c, perror.c & getlogin.c were provided for OS9 to UNIX compatibility.

In addition requires a modified verion of rlink that splits initialised data away from the OS9
module into a seperate binary file (awk$DATA).  A custom cstartdata.a is provided that
recombines the two at run time.  This was required to rescue 12K of memory from the awk
address space and allow enough memory for awk to run.

The binary(s) are provided:

awk
awk$DATA

Both are required in the cmds directory and both require execute permissions.
