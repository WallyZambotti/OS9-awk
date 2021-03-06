/*#include "awk.def"*/
/* lobj nullproc();
extern lobj program();
extern lobj boolop();
extern lobj relop();
extern lobj array();
extern lobj indirect();
extern lobj substr();
extern lobj sindex();
extern lobj asprintf();
extern lobj arith();
extern lobj incrdecr();
extern lobj cat();
extern lobj pastat();
extern lobj dopa2();
extern lobj matchop();
extern lobj aprintf();
extern lobj print();
extern lobj split();
extern lobj assign();
extern lobj ifstat();
extern lobj whilestat();
extern lobj forstat();
extern lobj instat();
extern lobj jump();
extern lobj fncn(); */
lobj (*proctab[75])() = {
/* 0FIRSTTOKEN*/	nullproc,
/*1 FINAL*/	nullproc,
/*2 FATAL*/	nullproc,
/*3 LT*/	relop,
/*4 LE*/	relop,
/*5 GT*/	relop,
/*6 GE*/	relop,
/*7 EQ*/	relop,
/*8 NE*/	relop,
/*9 MATCH*/	matchop,
/*10 NOTMATCH*/	matchop,
/*11 APPEND*/	nullproc,
/*12 ADD*/	arith,
/*13 MINUS*/	arith,
/*14 MULT*/	arith,
/*15 DIVIDE*/	arith,
/*16 MOD*/	arith,
/*17 UMINUS*/	arith,
/*18 ASSIGN*/	assign,
/*19 ADDEQ*/	assign,
/*20 SUBEQ*/	assign,
/*21 MULTEQ*/	assign,
/*22 DIVEQ*/	assign,
/*23 MODEQ*/	assign,
/*24 JUMP*/	nullproc,
/*25 XBEGIN*/	nullproc,
/*26 XEND*/	nullproc,
/*27 NL*/	nullproc,
/*28 PRINT*/	print,
/*29 PRINTF*/	aprintf,
/*30 SPRINTF*/	asprintf,
/*31 SPLIT*/	split,
/*32 IF*/	ifstat,
/*33 ELSE*/	nullproc,
/*34 WHILE*/	whilesta,
/*35 FOR*/	forstat,
/*36 IN*/	instat,
/*37 NEXT*/	jump,
/*38 EXIT*/	jump,
/*39 BREAK*/	jump,
/*40 CONTINUE*/	jump,
/*41 PROGRAM*/	program,
/*42 PASTAT*/	pastat,
/*43 ASTAT2*/	dopa2,
/*44 ASGNOP*/	nullproc,
/*45 BOR*/	boolop,
/*46 AND*/	boolop,
/*47 NOT*/	boolop,
/*48 NUMBER*/	nullproc,
/*49 VAR*/	nullproc,
/*50 ARRAY*/	array,
/*51 FNCN*/	fncn,
/*52 SUBSTR*/	substr,
/*53 LSUBSTR*/	nullproc,
/*54 INDEX*/	sindex,
/*55 RELOP*/	nullproc,
/*56 MATCHOP*/	nullproc,
/*57 OR*/	nullproc,
/*58 STRING*/	nullproc,
/*59 DOT*/	nullproc,
/*60 CCL*/	nullproc,
/*61 NCCL*/	nullproc,
/*62 CHAR*/	nullproc,
/*63 CAT*/	cat,
/*64 STAR*/	nullproc,
/*65 PLUS*/	nullproc,
/*66 QUEST*/	nullproc,
/*67 POSTINCR*/	incrdecr,
/*68 PREINCR*/	incrdecr,
/*69 POSTDECR*/	incrdecr,
/*70 PREDECR*/	incrdecr,
/*71 INCR*/	nullproc,
/*72 DECR*/	nullproc,
/*73 FIELD*/	nullproc,
/*74 INDIRECT*/	indirect,
};
