# include <stdio.h>
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 156 /* 200 CoCo mem fix */
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin, *yyout;
extern int yylineno;
struct yysvf { 
	int yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
# define A 2
# define str 4
# define chc 6
# define sc 8
# define reg 10
# define comment 12
#include	"awk.h"
#include	"awk.def"
#undef	input	/* defeat lex */
extern int	yylval;
extern int	mustfld;

int	lineno =	1;
#ifdef	DEBUG
#	define	RETURN(x)	{if (dbg) ptoken(x); return(x); }
#else
#	define	RETURN(x)	return(x)
#endif
#define	CADD	cbuf[clen++]=yytext[0]; if(clen>=CBUFLEN-1) {yyerror("string too long", cbuf); BEGIN A;}
#define	CBUFLEN	150
char	cbuf[CBUFLEN];
int	clen, cflag;
# define YYNEWLINE 10
yylex(){
int nstr, tmp; extern int yyprevious;
tmp = yybgin-yysvec-1;
/*fprintf(stderr, "a %x %x %x %x\n", yybgin, yysvec, yysvec + 3, tmp);*/
switch (tmp) {	/* witchcraft */
	case 0:
		BEGIN A;
		/* fprintf(stderr, "b %x %x %x\n", yybgin, yysvec, yysvec + 3); */
		break;
	case sc:
		BEGIN A;
		/* fprintf(stderr, "c %x %x %x\n", yybgin, yysvec, yysvec + 3); */
		RETURN('}');
	/* default:
		fprintf(stderr, "Impossible!\n"); */
	}
yyout = stdout;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
	lineno++;
break;
case 2:
lineno++;
break;
case 3:
	;
break;
case 4:
lineno++;
break;
case 5:
	RETURN(BOR);
break;
case 6:
RETURN(XBEGIN);
break;
case 7:
	RETURN(XEND);
break;
case 8:
RETURN(EOF);
break;
case 9:
	RETURN(AND);
break;
case 10:
	RETURN(NOT);
break;
case 11:
	{ yylval = NE; RETURN(RELOP); }
break;
case 12:
	{ yylval = MATCH; RETURN(MATCHOP); }
break;
case 13:
	{ yylval = NOTMATCH; RETURN(MATCHOP); }
break;
case 14:
	{ yylval = LT; RETURN(RELOP); }
break;
case 15:
	{ yylval = LE; RETURN(RELOP); }
break;
case 16:
	{ yylval = EQ; RETURN(RELOP); }
break;
case 17:
	{ yylval = GE; RETURN(RELOP); }
break;
case 18:
	{ yylval = GT; RETURN(RELOP); }
break;
case 19:
	{ yylval = APPEND; RETURN(RELOP); }
break;
case 20:
	{ yylval = INCR; RETURN(INCR); }
break;
case 21:
	{ yylval = DECR; RETURN(DECR); }
break;
case 22:
	{ yylval = ADDEQ; RETURN(ASGNOP); }
break;
case 23:
	{ yylval = SUBEQ; RETURN(ASGNOP); }
break;
case 24:
	{ yylval = MULTEQ; RETURN(ASGNOP); }
break;
case 25:
	{ yylval = DIVEQ; RETURN(ASGNOP); }
break;
case 26:
	{ yylval = MODEQ; RETURN(ASGNOP); }
break;
case 27:
	{ yylval = ASSIGN; RETURN(ASGNOP); }
break;
case 28:
{	if (atoi(yytext+1)==0) {
				yylval = lookup("$record", symtab);
				RETURN(STRING);
			} else {
				yylval = fieldadr(atoi(yytext+1));
				RETURN(FIELD);
			}
		}
break;
case 29:
{ RETURN(INDIRECT); }
break;
case 30:
	{ mustfld=1; yylval = stsymtab(yytext, NULL, 0.0, NUM, symtab); RETURN(VAR); }
break;
case 31:
{
		yylval = stsymtab(yytext, NULL, atof(yytext), CON|NUM, symtab); RETURN(NUMBER); }
break;
case 32:
{ BEGIN sc; lineno++; RETURN(';'); }
break;
case 33:
	{ BEGIN sc; RETURN(';'); }
break;
case 34:
	{ lineno++; RETURN(';'); }
break;
case 35:
	{ lineno++; RETURN(NL); }
break;
case 36:
RETURN(WHILE);
break;
case 37:
	RETURN(FOR);
break;
case 38:
	RETURN(IF);
break;
case 39:
	RETURN(ELSE);
break;
case 40:
	RETURN(NEXT);
break;
case 41:
	RETURN(EXIT);
break;
case 42:
RETURN(BREAK);
break;
case 43:
RETURN(CONTINUE);
break;
case 44:
{ yylval = PRINT; RETURN(PRINT); }
break;
case 45:
{ yylval = PRINTF; RETURN(PRINTF); }
break;
case 46:
{ yylval = SPRINTF; RETURN(SPRINTF); }
break;
case 47:
{ yylval = SPLIT; RETURN(SPLIT); }
break;
case 48:
RETURN(SUBSTR);
break;
case 49:
RETURN(INDEX);
break;
case 50:
	RETURN(IN);
break;
case 51:
{ yylval = FLENGTH; RETURN(FNCN); }
break;
case 52:
	{ yylval = FLOG; RETURN(FNCN); }
break;
case 53:
	{ yylval = FINT; RETURN(FNCN); }
break;
case 54:
	{ yylval = FEXP; RETURN(FNCN); }
break;
case 55:
	{ yylval = FSQRT; RETURN(FNCN); }
break;
case 56:
{ yylval = stsymtab(yytext, tostring(""), 0.0, STR, symtab); RETURN(VAR); }
break;
case 57:
	{ BEGIN str; clen=0; }
break;
case 58:
	{ BEGIN comment; }
break;
case 59:
{ BEGIN A; lineno++; RETURN(NL); }
break;
case 60:
;
break;
case 61:
	{ yylval = yytext[0]; RETURN(yytext[0]); }
break;
case 62:
{ BEGIN chc; clen=0; cflag=0; }
break;
case 63:
{ BEGIN chc; clen=0; cflag=1; }
break;
case 64:
RETURN(QUEST);
break;
case 65:
RETURN(PLUS);
break;
case 66:
RETURN(STAR);
break;
case 67:
RETURN(OR);
break;
case 68:
RETURN(DOT);
break;
case 69:
RETURN('(');
break;
case 70:
RETURN(')');
break;
case 71:
RETURN('^');
break;
case 72:
RETURN('$');
break;
case 73:
{	if (yytext[1]=='n') yylval = '\n';
			else if (yytext[1] == 't') yylval = '\t';
			else yylval = yytext[1];
			RETURN(CHAR);
		}
break;
case 74:
{ BEGIN A; unput('/'); }
break;
case 75:
	{ yyerror("newline in regular expression"); lineno++; BEGIN A; }
break;
case 76:
	{ yylval = yytext[0]; RETURN(CHAR); }
break;
case 77:
	{ BEGIN A; cbuf[clen]=0; yylval = stsymtab(cbuf, tostring(cbuf), 0.0, CON|STR, symtab); RETURN(STRING); }
break;
case 78:
	{ yyerror("newline in string"); lineno++; BEGIN A; }
break;
case 79:
{ cbuf[clen++]='"'; }
break;
case 80:
{ cbuf[clen++]='\n'; }
break;
case 81:
{ cbuf[clen++]='\t'; }
break;
case 82:
{ cbuf[clen++]='\\'; }
break;
case 83:
	{ CADD; }
break;
case 84:
{ cbuf[clen++]=']'; }
break;
case 85:
{ BEGIN reg; cbuf[clen]=0; yylval = tostring(cbuf);
		if (cflag==0) { RETURN(CCL); }
		else { RETURN(NCCL); } }
break;
case 86:
	{ yyerror("newline in character class"); lineno++; BEGIN A; }
break;
case 87:
	{ CADD; }
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

input()
{
	register c;
	extern char *lexprog;

	/* fprintf(stderr, ">%s<\n", lexprog); */

	if (yysptr > yysbuf)
		c = U(*--yysptr);
	else if (yyin == NULL)
		c = *lexprog++;
	else
		c = getc(yyin);
	if (c == '\n')
	{
			yylineno++;
			c = YYNEWLINE; /* WDZ This needs to be what YY is expecting a newline to be */
	}
	else if (c == EOF)
		c = 0;
	/* fprintf(stderr, ".%c", c); */
	return(c);
}

startreg()
{
	BEGIN reg;
}

int yyvstop[] = {
0,

61,
0,

3,
61,
0,

35,
0,

10,
61,
0,

57,
61,
0,

58,
61,
0,

29,
61,
0,

61,
0,

61,
0,

61,
0,

61,
0,

61,
0,

61,
0,

61,
0,

31,
61,
0,

61,
0,

14,
61,
0,

27,
61,
0,

18,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

56,
61,
0,

61,
0,

33,
61,
0,

12,
61,
0,

3,
61,
0,

1,
35,
0,

58,
61,
0,

83,
0,

78,
0,

77,
83,
0,

83,
0,

87,
0,

86,
0,

87,
0,

85,
87,
0,

76,
0,

75,
0,

72,
76,
0,

69,
76,
0,

70,
76,
0,

66,
76,
0,

65,
76,
0,

68,
76,
0,

74,
76,
0,

64,
76,
0,

62,
76,
0,

76,
0,

71,
76,
0,

67,
76,
0,

60,
0,

59,
0,

11,
0,

13,
0,

29,
0,

28,
0,

26,
0,

9,
0,

24,
0,

20,
0,

22,
0,

21,
0,

23,
0,

31,
0,

25,
0,

31,
0,

31,
0,

34,
0,

15,
0,

16,
0,

17,
0,

19,
0,

56,
0,

56,
0,

56,
0,

30,
56,
0,

56,
0,

4,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

38,
56,
0,

50,
56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

5,
0,

32,
0,

2,
0,

79,
0,

82,
0,

80,
0,

81,
0,

84,
0,

63,
0,

73,
0,

31,
0,

56,
0,

7,
56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

54,
56,
0,

37,
56,
0,

56,
0,

53,
56,
0,

56,
0,

52,
56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

56,
0,

39,
56,
0,

41,
56,
0,

56,
0,

56,
0,

40,
56,
0,

56,
0,

56,
0,

56,
0,

55,
56,
0,

56,
0,

56,
0,

6,
56,
0,

56,
0,

42,
56,
0,

56,
0,

49,
56,
0,

56,
0,

44,
56,
0,

47,
56,
0,

56,
0,

56,
0,

36,
56,
0,

56,
0,

56,
0,

51,
56,
0,

45,
56,
0,

56,
0,

48,
56,
0,

8,
56,
0,

56,
0,

46,
56,
0,

43,
56,
0,
0};
# define YYTYPE int
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	3,15,	0,0,	
0,0,	0,0,	0,0,	0,0,	
13,78,	0,0,	3,16,	3,17,	
0,0,	0,0,	0,0,	0,0,	
13,78,	13,79,	30,96,	39,106,	
0,0,	5,56,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	5,56,	5,57,	0,0,	
0,0,	0,0,	3,18,	3,19,	
3,20,	3,21,	3,22,	3,23,	
0,0,	0,0,	0,0,	3,24,	
3,25,	0,0,	3,26,	3,27,	
3,28,	3,29,	6,58,	23,85,	
0,0,	0,0,	5,58,	13,78,	
25,87,	0,0,	0,0,	0,0,	
3,30,	3,31,	3,32,	3,33,	
26,89,	0,0,	3,34,	3,35,	
5,56,	18,80,	3,36,	22,84,	
13,78,	24,86,	25,88,	7,60,	
28,92,	31,97,	32,98,	3,37,	
26,90,	3,38,	35,102,	7,60,	
7,61,	5,56,	33,99,	33,100,	
36,103,	37,104,	38,105,	0,0,	
59,128,	3,39,	0,0,	0,0,	
0,0,	0,0,	0,0,	3,40,	
3,41,	0,0,	3,42,	3,43,	
4,53,	4,54,	3,44,	102,137,	
6,59,	3,45,	44,112,	3,46,	
5,59,	3,47,	8,62,	8,63,	
3,48,	46,116,	44,113,	41,108,	
3,49,	43,111,	7,60,	40,107,	
42,109,	3,50,	3,51,	3,52,	
4,18,	4,19,	4,55,	4,21,	
4,22,	4,23,	18,81,	45,114,	
42,110,	4,24,	4,25,	7,60,	
4,26,	4,27,	4,28,	47,117,	
11,64,	45,115,	48,118,	48,119,	
49,121,	50,122,	59,129,	48,120,	
11,64,	11,65,	4,30,	4,31,	
4,32,	4,33,	51,123,	51,124,	
74,133,	4,35,	103,138,	105,139,	
4,36,	82,82,	7,62,	7,63,	
59,130,	53,125,	91,95,	107,140,	
108,141,	4,37,	59,131,	4,38,	
109,142,	111,145,	114,148,	11,66,	
115,149,	51,123,	113,146,	11,67,	
11,68,	11,69,	11,70,	4,39,	
82,82,	11,71,	11,72,	11,64,	
53,125,	4,40,	4,41,	53,126,	
4,42,	4,43,	113,147,	116,150,	
4,44,	117,151,	91,95,	4,45,	
110,143,	4,46,	11,73,	4,47,	
11,64,	21,82,	4,48,	110,144,	
62,129,	62,132,	4,49,	119,154,	
120,155,	121,156,	12,66,	4,50,	
4,51,	4,52,	12,67,	12,68,	
12,69,	12,70,	137,157,	118,152,	
12,71,	12,72,	62,130,	139,158,	
21,82,	118,153,	11,74,	11,75,	
62,131,	11,76,	140,159,	141,160,	
142,161,	143,162,	146,163,	148,164,	
150,165,	12,73,	151,166,	152,167,	
21,83,	21,83,	21,83,	21,83,	
21,83,	21,83,	21,83,	21,83,	
21,83,	21,83,	153,168,	154,169,	
55,126,	155,170,	156,171,	157,172,	
158,173,	159,174,	160,175,	11,77,	
55,126,	55,127,	163,176,	164,177,	
166,178,	12,74,	12,75,	167,179,	
12,76,	27,91,	27,91,	27,91,	
27,91,	27,91,	27,91,	27,91,	
27,91,	27,91,	27,91,	29,93,	
168,180,	29,94,	29,94,	29,94,	
29,94,	29,94,	29,94,	29,94,	
29,94,	29,94,	29,94,	170,181,	
171,182,	173,183,	175,184,	177,185,	
178,186,	180,187,	12,77,	55,126,	
181,188,	183,189,	29,95,	83,83,	
83,83,	83,83,	83,83,	83,83,	
83,83,	83,83,	83,83,	83,83,	
83,83,	184,190,	187,191,	190,192,	
55,126,	0,0,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	29,95,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	34,101,	34,101,	34,101,	
34,101,	75,134,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	75,134,	75,106,	93,93,	
93,93,	93,93,	93,93,	93,93,	
93,93,	93,93,	93,93,	93,93,	
93,93,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
95,135,	0,0,	95,135,	0,0,	
93,95,	95,136,	95,136,	95,136,	
95,136,	95,136,	95,136,	95,136,	
95,136,	95,136,	95,136,	0,0,	
0,0,	0,0,	0,0,	0,0,	
75,134,	135,136,	135,136,	135,136,	
135,136,	135,136,	135,136,	135,136,	
135,136,	135,136,	135,136,	0,0,	
0,0,	0,0,	0,0,	0,0,	
93,95,	75,134,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
0,	0,		0,	
0,	0,		0,	
-1,	0,		0,	
-95,	yysvec+3,	0,	
-20,	0,		0,	
-16,	yysvec+5,	0,	
-74,	0,		0,	
-22,	yysvec+7,	0,	
0,	0,		0,	
0,	0,		0,	
-143,	0,		0,	
-182,	yysvec+11,	0,	
-7,	0,		0,	
0,	yysvec+13,	0,	
0,	0,		yyvstop+1,
0,	0,		yyvstop+3,
0,	0,		yyvstop+6,
8,	0,		yyvstop+8,
0,	0,		yyvstop+11,
0,	0,		yyvstop+14,
200,	0,		yyvstop+17,
10,	0,		yyvstop+20,
13,	0,		yyvstop+22,
12,	0,		yyvstop+24,
13,	0,		yyvstop+26,
19,	0,		yyvstop+28,
229,	0,		yyvstop+30,
15,	0,		yyvstop+32,
241,	0,		yyvstop+34,
8,	0,		yyvstop+37,
16,	0,		yyvstop+39,
17,	0,		yyvstop+42,
25,	0,		yyvstop+45,
278,	0,		yyvstop+48,
13,	yysvec+34,	yyvstop+51,
10,	yysvec+34,	yyvstop+54,
19,	yysvec+34,	yyvstop+57,
8,	yysvec+34,	yyvstop+60,
9,	0,		yyvstop+63,
9,	yysvec+34,	yyvstop+65,
8,	yysvec+34,	yyvstop+68,
16,	yysvec+34,	yyvstop+71,
10,	yysvec+34,	yyvstop+74,
8,	yysvec+34,	yyvstop+77,
34,	yysvec+34,	yyvstop+80,
16,	yysvec+34,	yyvstop+83,
29,	yysvec+34,	yyvstop+86,
34,	yysvec+34,	yyvstop+89,
44,	yysvec+34,	yyvstop+92,
25,	0,		yyvstop+95,
149,	0,		yyvstop+97,
0,	0,		yyvstop+100,
160,	0,		yyvstop+103,
0,	0,		yyvstop+106,
-259,	0,		yyvstop+109,
0,	0,		yyvstop+112,
0,	0,		yyvstop+114,
0,	0,		yyvstop+116,
58,	0,		yyvstop+119,
0,	0,		yyvstop+121,
0,	0,		yyvstop+123,
120,	0,		yyvstop+125,
0,	0,		yyvstop+127,
0,	0,		yyvstop+130,
0,	0,		yyvstop+132,
0,	0,		yyvstop+134,
0,	0,		yyvstop+137,
0,	0,		yyvstop+140,
0,	0,		yyvstop+143,
0,	0,		yyvstop+146,
0,	0,		yyvstop+149,
0,	0,		yyvstop+152,
0,	0,		yyvstop+155,
66,	0,		yyvstop+158,
-400,	0,		yyvstop+161,
0,	0,		yyvstop+163,
0,	0,		yyvstop+166,
0,	0,		yyvstop+169,
0,	0,		yyvstop+171,
0,	0,		yyvstop+173,
0,	0,		yyvstop+175,
156,	0,		yyvstop+177,
263,	0,		yyvstop+179,
0,	0,		yyvstop+181,
0,	0,		yyvstop+183,
0,	0,		yyvstop+185,
0,	0,		yyvstop+187,
0,	0,		yyvstop+189,
0,	0,		yyvstop+191,
0,	0,		yyvstop+193,
101,	yysvec+27,	yyvstop+195,
0,	0,		yyvstop+197,
363,	0,		yyvstop+199,
0,	yysvec+29,	yyvstop+201,
385,	0,		0,	
0,	0,		yyvstop+203,
0,	0,		yyvstop+205,
0,	0,		yyvstop+207,
0,	0,		yyvstop+209,
0,	0,		yyvstop+211,
0,	yysvec+34,	yyvstop+213,
36,	yysvec+34,	yyvstop+215,
94,	yysvec+34,	yyvstop+217,
0,	yysvec+34,	yyvstop+219,
84,	yysvec+34,	yyvstop+222,
0,	0,		yyvstop+224,
70,	yysvec+34,	yyvstop+226,
62,	yysvec+34,	yyvstop+228,
61,	yysvec+34,	yyvstop+230,
99,	yysvec+34,	yyvstop+232,
63,	yysvec+34,	yyvstop+234,
0,	yysvec+34,	yyvstop+236,
82,	yysvec+34,	yyvstop+239,
68,	yysvec+34,	yyvstop+242,
77,	yysvec+34,	yyvstop+244,
79,	yysvec+34,	yyvstop+246,
96,	yysvec+34,	yyvstop+248,
119,	yysvec+34,	yyvstop+250,
101,	yysvec+34,	yyvstop+252,
118,	yysvec+34,	yyvstop+254,
112,	yysvec+34,	yyvstop+256,
0,	0,		yyvstop+258,
0,	yysvec+51,	0,	
0,	0,		yyvstop+260,
0,	yysvec+53,	0,	
0,	yysvec+55,	0,	
0,	0,		yyvstop+262,
0,	0,		yyvstop+264,
0,	0,		yyvstop+266,
0,	0,		yyvstop+268,
0,	0,		yyvstop+270,
0,	0,		yyvstop+272,
0,	0,		yyvstop+274,
0,	0,		yyvstop+276,
401,	0,		0,	
0,	yysvec+135,	yyvstop+278,
153,	yysvec+34,	yyvstop+280,
0,	yysvec+34,	yyvstop+282,
160,	yysvec+34,	yyvstop+285,
141,	yysvec+34,	yyvstop+287,
123,	yysvec+34,	yyvstop+289,
139,	yysvec+34,	yyvstop+291,
125,	yysvec+34,	yyvstop+293,
0,	yysvec+34,	yyvstop+295,
0,	yysvec+34,	yyvstop+298,
141,	yysvec+34,	yyvstop+301,
0,	yysvec+34,	yyvstop+303,
140,	yysvec+34,	yyvstop+306,
0,	yysvec+34,	yyvstop+308,
128,	yysvec+34,	yyvstop+311,
136,	yysvec+34,	yyvstop+313,
142,	yysvec+34,	yyvstop+315,
153,	yysvec+34,	yyvstop+317,
143,	yysvec+34,	yyvstop+319,
146,	yysvec+34,	yyvstop+321,
154,	yysvec+34,	yyvstop+323,
185,	yysvec+34,	yyvstop+325,
195,	yysvec+34,	yyvstop+327,
158,	yysvec+34,	yyvstop+329,
161,	yysvec+34,	yyvstop+331,
0,	yysvec+34,	yyvstop+333,
0,	yysvec+34,	yyvstop+336,
150,	yysvec+34,	yyvstop+339,
155,	yysvec+34,	yyvstop+341,
0,	yysvec+34,	yyvstop+343,
156,	yysvec+34,	yyvstop+346,
159,	yysvec+34,	yyvstop+348,
178,	yysvec+34,	yyvstop+350,
0,	yysvec+34,	yyvstop+352,
183,	yysvec+34,	yyvstop+355,
199,	yysvec+34,	yyvstop+357,
0,	yysvec+34,	yyvstop+359,
223,	yysvec+34,	yyvstop+362,
0,	yysvec+34,	yyvstop+364,
192,	yysvec+34,	yyvstop+367,
0,	yysvec+34,	yyvstop+369,
199,	yysvec+34,	yyvstop+372,
202,	yysvec+34,	yyvstop+374,
0,	yysvec+34,	yyvstop+377,
189,	yysvec+34,	yyvstop+380,
194,	yysvec+34,	yyvstop+382,
0,	yysvec+34,	yyvstop+384,
241,	yysvec+34,	yyvstop+387,
204,	yysvec+34,	yyvstop+389,
0,	yysvec+34,	yyvstop+391,
0,	yysvec+34,	yyvstop+394,
220,	yysvec+34,	yyvstop+397,
0,	yysvec+34,	yyvstop+399,
0,	yysvec+34,	yyvstop+402,
222,	yysvec+34,	yyvstop+405,
0,	yysvec+34,	yyvstop+407,
0,	yysvec+34,	yyvstop+410,
0,	0,	0};
#define yytop 465
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
int yylineno =1;
# define YYU(x) x
/* # define NLSTATE yyprevious=YYNEWLINE */
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register int yyt;
	register struct yywork *yyk;
	struct yysvf *yyz;
	int yych;
	int yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		/* fprintf(stderr, "%x %x %x %x\n", yyestate, yystate, yybgin, yybgin-yysvec); */
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == 0){		/* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == 0)break;
				}
			*yylastch++ = yych = input();
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( yyt > 0){
				yyt = yyr + yych;
				yyk = yycrank + yyt;
				if (yyt <= yytop && yyk->verify+yysvec == yystate){
					if(yyk->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyk->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if(yyt < 0) {		/* r < yycrank */
				yyt = yyr = -yyt;
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				yyk = yycrank + yyt;
				if(yyt <= yytop && yyk->verify+yysvec == yystate){
					if(yyk->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyk->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
				yyk = yycrank + yyt;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyk->verify+yysvec == yystate){
					if(yyk->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyk->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != 0){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#ifdef __cplusplus
yyback(int *p, int m)
#else
yyback(p, m)
	int *p;
#endif
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
#ifdef __cplusplus
yyoutput(int c)
#else
yyoutput(c)
  int c;
#endif
{
	output(c);
	}
#ifdef __cplusplus
yyunput(int c)
#else
yyunput(c)
   int c;
#endif
{
	unput(c);
	}

