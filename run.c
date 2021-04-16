#include <stdio.h>
#include "awk.def"
#include	<math.h>
#define RECSIZE 256 /* 512 CoCo mem fix */
#include "awk.h"

#define FILENUM	10
struct
{
	FILE *fp;
	char *fname;
} files[FILENUM];
FILE *popen();

extern lobj execute(), nodetoobj(), fieldel(), dopa2(), program();
#define PA2NUM	24 /* 29 CoCo mem fix */
int pairstack[PA2NUM], paircnt=0;
node *winner = (node *)NULL;
#define MAXTMP 16 /* 20  CoCo mem fix */
cell tmps[MAXTMP];
static cell nullval ={0,0,0.0,NUM,0};
obj	true	={ OBOOL, BTRUE, 0 };
obj	false	={ OBOOL, BFALSE, 0 };

/*dumpnode(n) node *n;
{
	extern char *printname[];
	fprintf(stderr, "%04x + %d - %s %04x %04x %04x %04x %04x\n", 
		n, n->nobj-FIRSTTOKEN, printname[n->nobj-FIRSTTOKEN],
		n->nnext, n->narg[0], n->narg[1]), n->narg[2], n->narg[3];
}*/

run()
{
	obj o;
	OBJ2LNG(o) = execute(winner);
	return(o.otype);
}

lobj execute(u) node *u;
{
	lobj (*proc)();
	obj x;
	node *a;
	/* extern char *printname[]; */

	/* fprintf(stderr, " - %d\n", u->nobj-FIRSTTOKEN); */

	if (u==(node *)NULL)
		return(OBJ2LNG(true));
	for (a = u; ; a = a->nnext) {
		if (cantexec(a))
			return(nodetoobj(a));
		if (a->ntype==NPA2)
			proc=dopa2;
		else {
			if (notlegal(a->nobj))
				error(FATAL, "illegal statement %o", a);
			proc = proctab[a->nobj-FIRSTTOKEN];
		}
		/*dumpnode(a);*/
		OBJ2LNG(x) = (*proc)(a->narg,a->nobj);
		if (isfld(x)) fldbld();
		if (isexpr(a))  
			return(OBJ2LNG(x));
		/* a statement, goto next statement */
		if (isjump(x))
			return(OBJ2LNG(x));
		if (a->nnext == (node *)NULL)
			return(OBJ2LNG(x));
		tempfree(OBJ2LNG(x));
	}
}

lobj program(a, n) node **a;
{
	obj x;
	if (a[0] != NULL) {
		OBJ2LNG(x) = execute(a[0]);
		if (isexit(x))
			return(OBJ2LNG(true));
		if (isjump(x))
			error(FATAL, "unexpected break, continue or next");
		tempfree(OBJ2LNG(x));
	}
	while (getrec()) {
		recloc->tval &= ~NUM;
		recloc->tval |= STR;
		++nrloc->fval;
		nrloc->tval &= ~STR;
		nrloc->tval |= NUM;
		OBJ2LNG(x) = execute(a[1]);
		if (isexit(x)) break;
		tempfree(OBJ2LNG(x));
	}
	tempfree(OBJ2LNG(x));
	if (a[2] != NULL) {
		OBJ2LNG(x) = execute(a[2]);
		if (isbreak(x) || isnext(x) || iscont(x))
			error(FATAL, "unexpected break, continue or next");
		tempfree(OBJ2LNG(x));
	}
	return(OBJ2LNG(true));
}

lobj array(a,n) node **a;
{
	obj x, y;
	extern lobj arrayel();

	OBJ2LNG(x) = execute(a[1]);
	OBJ2LNG(y) = arrayel(a[0], OBJ2LNG(x));
	tempfree(OBJ2LNG(x));
	return(OBJ2LNG(y));
}

lobj arrayel(a,_b) node *a; lobj _b;
{
	char *s;
	cell *x;
	/* int i; */
	obj y;
	obj b; OBJ2LNG(b) = _b;

	s = getsval(b.optr);
	x = (cell *) a;
	if (!(x->tval&ARR)) {
		xfree(x->sval);
		x->tval &= ~STR;
		x->tval |= ARR;
		x->sval = (char *) mksymtab();
	}
	y.optr = stsymtab(s, tostring(""), 0.0, STR, x->sval);
	y.otype = OCELL;
	y.osub = CVAR;
	return(OBJ2LNG(y));
}

lobj matchop(a,n) node **a;
{
	obj x;
	char *s;
	int i;

	OBJ2LNG(x) = execute(a[0]);
	if (isstr(x)) s = x.optr->sval;
	else	s = getsval(x.optr);
	tempfree(OBJ2LNG(x));
	i = match(a[1], s);
	if (n==MATCH && i==1 || n==NOTMATCH && i==0)
		return(OBJ2LNG(true));
	else
		return(OBJ2LNG(false));
}

lobj boolop(a,n) node **a;
{
	obj x, y;
	int i;

	OBJ2LNG(x) = execute(a[0]);
	i = istrue(x);
	tempfree(OBJ2LNG(x));
	switch (n) {
	default:
		error(FATAL, "unknown boolean operator %d", n);
	case BOR:
		if (i) return(OBJ2LNG(true));
		OBJ2LNG(y) = execute(a[1]);
		i = istrue(y);
		tempfree(OBJ2LNG(y));
		if (i) return(OBJ2LNG(true));
		else return(OBJ2LNG(false));
	case AND:
		if ( !i ) return(OBJ2LNG(false));
		OBJ2LNG(y) = execute(a[1]);
		i = istrue(y);
		tempfree(OBJ2LNG(y));
		if (i) return(OBJ2LNG(true));
		else return(OBJ2LNG(false));
	case NOT:
		if (i) return(OBJ2LNG(false));
		else return(OBJ2LNG(true));
	}
}

lobj relop(a,n) node **a;
{
	int i;
	obj x, y;
	awkfloat j;

	OBJ2LNG(x) = execute(a[0]);
	OBJ2LNG(y) = execute(a[1]);
	if (x.optr->tval&NUM && y.optr->tval&NUM) {
		j = x.optr->fval - y.optr->fval;
		i = j<0? -1: (j>0? 1: 0);
	} else {
		i = strcmp(getsval(x.optr), getsval(y.optr));
	}
	tempfree(OBJ2LNG(x));
	tempfree(OBJ2LNG(y));
	switch (n) {
	default:
		error(FATAL, "unknown relational operator %d", n);
	case LT:	if (i<0) return(OBJ2LNG(true));
			else return(OBJ2LNG(false));
	case LE:	if (i<=0) return(OBJ2LNG(true));
			else return(OBJ2LNG(false));
	case NE:	if (i!=0) return(OBJ2LNG(true));
			else return(OBJ2LNG(false));
	case EQ:	if (i==0) return(OBJ2LNG(true));
			else return(OBJ2LNG(false));
	case GE:	if (i>=0) return(OBJ2LNG(true));
			else return(OBJ2LNG(false));
	case GT:	if (i>0) return(OBJ2LNG(true));
			else return(OBJ2LNG(false));
	}
}

tempfree(_a) lobj _a;
{
	obj a; OBJ2LNG(a) = _a;
	if (!istemp(a)) return;
	xfree(a.optr->sval);
	a.optr->tval = 0;
}

lobj gettemp()
{
	int i;
	obj x;

	for (i=0; i<MAXTMP; i++)
		if (tmps[i].tval==0)
			break;
	if (i==MAXTMP)
		error(FATAL, "out of temporaries in gettemp");
	x.optr = &tmps[i];
	/* tmps[i] = nullval; */
	tmps[i].nval = 0;
	tmps[i].sval = 0;
	tmps[i].fval = 0.0;
	tmps[i].tval = NUM;
	tmps[i].nextval = 0;
	x.otype = OCELL;
	x.osub = CTEMP;
	return(OBJ2LNG(x));
}

lobj indirect(a,n) node **a;
{
	obj x;
	int m;
	cell *fieldadr();

	OBJ2LNG(x) = execute(a[0]);
	m = getfval(x.optr);
	tempfree(OBJ2LNG(x));
	x.optr = fieldadr(m);
	x.otype = OCELL;
	x.osub = CFLD;
	return(OBJ2LNG(x));
}

lobj substr(a, nnn) node **a;
{
	char *s, *p, temp[100];
	obj x;
	int k, m, n;

	OBJ2LNG(x) = execute(a[0]);
	s = getsval(x.optr);
	k = strlen(s) + 1;
	tempfree(OBJ2LNG(x));
	OBJ2LNG(x) = execute(a[1]);
	m = getfval(x.optr);
	if (m <= 0)
		m = 1;
	else if (m > k)
		m = k;
	tempfree(OBJ2LNG(x));
	if (a[2] != nullstat) {
		OBJ2LNG(x) = execute(a[2]);
		n = getfval(x.optr);
		tempfree(OBJ2LNG(x));
	}
	else
		n = k - 1;
	if (n < 0)
		n = 0;
	else if (n > k - m)
		n = k - m;
	dprintf("substr: m=%d, n=%d, s=%s\n", m, n, s);
	p = temp;
	s += m - 1;
	while (n-- > 0)
		*p++ = *s++;
	*p = '\0';
	OBJ2LNG(x) = gettemp();
	setsval(x.optr, temp);
	return(OBJ2LNG(x));
}

lobj sindex(a, nnn) node **a;
{
	obj x, y;
	char *s1, *s2, *p1, *p2, *q;

	OBJ2LNG(x) = execute(a[0]);
	s1 = getsval(x.optr);
	tempfree(OBJ2LNG(x));
	OBJ2LNG(y) = execute(a[1]);
	s2 = getsval(y.optr);
	tempfree(OBJ2LNG(y));

	OBJ2LNG(x) = gettemp();
	for (p1 = s1; *p1 != '\0'; p1++) {
		for (q=p1, p2=s2; *p2 != '\0' && *q == *p2; q++, p2++)
			;
		if (*p2 == '\0') {
			setfval(x.optr, (awkfloat) (p1 - s1 + 1));	/* origin 1 */
			return(OBJ2LNG(x));
		}
	}
	setfval(x.optr, 0.0);
	return(OBJ2LNG(x));
}

char *format(s,a) char *s; node *a;
{
	char *buf, *p, fmt[100], *t, *os;
	obj x;
	int flag = 0;
	awkfloat xf;

	os = s;
	p = buf = (char *)malloc(RECSIZE);
	while (*s) {
		if (*s != '%') {
			*p++ = *s++;
			continue;
		}
		if (*(s+1) == '%') {
			*p++ = '%';
			*p++ = '%';
			s += 2;
			continue;
		}
		for (t=fmt; (*t++ = *s) != '\0'; s++)
			if (*s >= 'a' && *s <= 'z' && *s != 'l')
				break;
		*t = '\0';
		if (t > fmt + 100)
			error(FATAL, "format item %.20s... too long", os);
		switch (*s) {
		case 'f': case 'e': case 'g':
			flag = 1;
			break;
		case 'd':
			flag = 2;
			if(*(s-1) == 'l') break;
			*(t-1) = 'l';
			*t = 'd';
			*++t = '\0';
			break;
		case 'o': case 'x':
			flag = *(s-1)=='l' ? 2 : 3;
			break;
		case 's':
			flag = 4;
			break;
		default:
			flag = 0;
			break;
		}
		if (flag == 0) {
			sprintf(p, "%s", fmt);
			p += strlen(p);
			continue;
		}
		if (a == NULL)
			error(FATAL, "not enough arguments in printf(%s)", os);
		OBJ2LNG(x) = execute(a);
		a = a->nnext;
		if (flag != 4)	/* watch out for converting to numbers! */
			xf = getfval(x.optr);
		if (flag==1) sprintf(p, fmt, xf);
		else if (flag==2) sprintf(p, fmt, (long)xf);
		else if (flag==3) sprintf(p, fmt, (int)xf);
		else if (flag==4) sprintf(p, fmt, x.optr->sval==NULL ? "" : getsval(x.optr));
		tempfree(OBJ2LNG(x));
		p += strlen(p);
		s++;
	}
	*p = '\0';
	return(buf);
}

lobj asprintf(a,n) node **a;
{
	obj x;
	node *y;
	char *s;

	y = a[0]->nnext;
	OBJ2LNG(x) = execute(a[0]);
	s = format(getsval(x.optr), y);
	tempfree(OBJ2LNG(x));
	OBJ2LNG(x) = gettemp();
	x.optr->sval = s;
	x.optr->tval = STR;
	return(OBJ2LNG(x));
}

lobj arith(a,n) node **a;
{
	awkfloat i,j;
	obj x,y,z;

	OBJ2LNG(x) = execute(a[0]);
	i = getfval(x.optr);
	tempfree(OBJ2LNG(x));
	if (n != UMINUS) {
		OBJ2LNG(y) = execute(a[1]);
		j = getfval(y.optr);
		tempfree(OBJ2LNG(y));
	}
	OBJ2LNG(z) = gettemp();
	switch (n) {
	default:
		error(FATAL, "illegal arithmetic operator %d", n);
	case ADD:
		setfval(z.optr, i+j);
		break;
	case MINUS:
		setfval(z.optr, i-j);
		break;
	case MULT:
		setfval(z.optr, i*j);
		break;
	case DIVIDE:
		if (j == 0)
			error(FATAL, "division by zero");
		setfval(z.optr, i/j);
		break;
	case MOD:
		if (j == 0)
			error(FATAL, "division by zero");
		setfval(z.optr, i-j*(long)(i/j));
		break;
	case UMINUS:
		setfval(z.optr, -i);
		break;
	}
	return(OBJ2LNG(z));
}

lobj incrdecr(a, n) node **a;
{
	obj x, z;
	int k;
	awkfloat xf;

	OBJ2LNG(x) = execute(a[0]);
	xf = getfval(x.optr);
	k = (n == PREINCR || n == POSTINCR) ? 1 : -1;
	if (n == PREINCR || n == PREDECR) {
		setfval(x.optr, xf + k);
		return(OBJ2LNG(x));
	}
	OBJ2LNG(z) = gettemp();
	setfval(z.optr, xf);
	setfval(x.optr, xf + k);
	tempfree(OBJ2LNG(x));
	return(OBJ2LNG(z));
}


lobj assign(a,n) node **a;
{
	obj x, y;
	awkfloat xf, yf;

	OBJ2LNG(x) = execute(a[0]);
	OBJ2LNG(y) = execute(a[1]);
	if (n == ASSIGN) {	/* ordinary assignment */
		if (y.optr->tval&STR) setsval(x.optr, y.optr->sval);
		if (y.optr->tval&NUM) setfval(x.optr, y.optr->fval);
		tempfree(OBJ2LNG(y));
		return(OBJ2LNG(x));
	}
	xf = getfval(x.optr);
	yf = getfval(y.optr);
	switch (n) {
	case ADDEQ:
		setfval(x.optr, xf + yf);
		break;
	case SUBEQ:
		setfval(x.optr, xf - yf);
		break;
	case MULTEQ:
		setfval(x.optr, xf * yf);
		break;
	case DIVEQ:
		if (yf == 0)
			error(FATAL, "division by zero");
		setfval(x.optr, xf / yf);
		break;
	case MODEQ:
		if (yf == 0)
			error(FATAL, "division by zero");
		setfval(x.optr, xf - yf*(long)(xf/yf));
		break;
	default:
		error(FATAL, "illegal assignment operator %d", n);
		break;
	}
	tempfree(OBJ2LNG(y));
	return(OBJ2LNG(x));
}

lobj cat(a,q) node **a;
{
	obj x,y,z;
	int n;
	char *s;

	OBJ2LNG(x) = execute(a[0]);
	OBJ2LNG(y) = execute(a[1]);
	getsval(x.optr);
	getsval(y.optr);
	n = strlen(x.optr->sval) + strlen(y.optr->sval);
	s = (char *)malloc(n+1);
	strcpy(s, x.optr->sval);
	strcat(s, y.optr->sval);
	tempfree(OBJ2LNG(y));
	OBJ2LNG(z) = gettemp();
	z.optr->sval = s;
	z.optr->tval = STR;
	tempfree(OBJ2LNG(x));
	return(OBJ2LNG(z));
}

lobj pastat(a,n) node **a;
{
	obj x;

	if (a[0]==nullstat)
		OBJ2LNG(x )= OBJ2LNG(true);
	else
		OBJ2LNG(x) = execute(a[0]);
	if (istrue(x)) {
		tempfree(OBJ2LNG(x));
		OBJ2LNG(x) = execute(a[1]);
	}
	return(OBJ2LNG(x));
}

lobj dopa2(a,n) node **a;
{
	obj x;

	if (pairstack[n]==0) {
		OBJ2LNG(x) = execute(a[0]);
		if (istrue(x))
			pairstack[n] = 1;
		tempfree(OBJ2LNG(x));
	}
	if (pairstack[n] == 1) {
		OBJ2LNG(x) = execute(a[1]);
		if (istrue(x))
			pairstack[n] = 0;
		tempfree(OBJ2LNG(x));
		OBJ2LNG(x) = execute(a[2]);
		return(OBJ2LNG(x));
	}
	return(OBJ2LNG(false));
}

lobj aprintf(a,n) node **a;
{
	obj x;

	OBJ2LNG(x) = asprintf(a,n);
	if (a[1]==NULL) {
		printf(x.optr->sval);
		tempfree(OBJ2LNG(x));
		return(OBJ2LNG(true));
	}
	redirprint(x.optr->sval, (int)a[1], a[2]);
	return(OBJ2LNG(x));
}

lobj split(a,nnn) node **a;
{
	obj x;
	cell *ap;
	register char *s, *p;
	char *t, temp[100], num[5];
	register int sep;
	int n;

	OBJ2LNG(x) = execute(a[0]);
	s = getsval(x.optr);
	tempfree(OBJ2LNG(x));
	if (a[2] == nullstat)
		sep = **FS;
	else {
		OBJ2LNG(x) = execute(a[2]);
		sep = getsval(x.optr)[0];
		tempfree(OBJ2LNG(x));
	}
	n = 0;
	ap = (cell *) a[1];
	frsymtab(ap);
	dprintf("split: s=|%s|, a=%s, sep=|%c|\n", s, ap->nval, sep);
	ap->tval &= ~STR;
	ap->tval |= ARR;
	ap->sval = (char *) mksymtab();
	/* here we go */
	for (;;) {
		if (sep == ' ')
			while (*s == ' ' || *s == '\t' || *s == '\n')
				s++;
		if (*s == '\0')
			break;
		n++;
		for (p=s, t=temp; (*t = *p) != '\0'; p++, t++)
			if (*p == sep
			  || sep == ' ' && (*p == '\t' || *p == '\n')
			  || sep == '\t' && *p == '\n')
				break;
		*t = '\0';
		dprintf("n=%d, s=|%s|, temp=|%s|\n", n, s, temp);
		sprintf(num, "%d", n);
		stsymtab(num, tostring(temp), 0.0, STR, ap->sval);
		if (*p == '\0')	/* all done */
			break;
		s = p + 1;
	}
	OBJ2LNG(x) = gettemp();
	x.optr->tval = NUM;
	x.optr->fval = n;
	return(OBJ2LNG(x));
}

lobj ifstat(a,n) node **a;
{
	obj x;

	OBJ2LNG(x) = execute(a[0]);
	if (istrue(x)) {
		tempfree(OBJ2LNG(x));
		OBJ2LNG(x) = execute(a[1]);
	}
	else if (a[2] != nullstat) {
		tempfree(OBJ2LNG(x));
		OBJ2LNG(x) = execute(a[2]);
	}
	return(OBJ2LNG(x));
}

lobj whilesta(a,n) node **a;
{
	obj x;

	for (;;) {
		OBJ2LNG(x) = execute(a[0]);
		if (!istrue(x)) return(OBJ2LNG(x));
		tempfree(OBJ2LNG(x));
		OBJ2LNG(x) = execute(a[1]);
		if (isbreak(x)) {
			OBJ2LNG(x) = OBJ2LNG(true);
			return(OBJ2LNG(x));
		}
		if (isnext(x) || isexit(x))
			return(OBJ2LNG(x));
		tempfree(OBJ2LNG(x));
	}
}

lobj forstat(a,n) node **a;
{
	obj x;

	tempfree(execute(a[0]));
	for (;;) {
		if (a[1]!=nullstat) {
			OBJ2LNG(x) = execute(a[1]);
			if (!istrue(x)) return(OBJ2LNG(x));
			else tempfree(OBJ2LNG(x));
		}
		OBJ2LNG(x) = execute(a[3]);
		if (isbreak(x)) {	/* turn off break */
			OBJ2LNG(x) = OBJ2LNG(true);
			return(OBJ2LNG(x));
		}
		if (isnext(x) || isexit(x))
			return(OBJ2LNG(x));
		tempfree(OBJ2LNG(x));
		tempfree(execute(a[2]));
	}
}

lobj instat(a, n) node **a;
{
	cell *vp, *arrayp, *cp, **tp;
	obj x;
	int i;

	vp = (cell *) a[0];
	arrayp = (cell *) a[1];
	if (!(arrayp->tval & ARR))
		error(FATAL, "%s is not an array", arrayp->nval);
	tp = (cell **) arrayp->sval;
	for (i = 0; i < MAXSYM; i++) {	/* this routine knows too much */
		for (cp = tp[i]; cp != NULL; cp = cp->nextval) {
			xfree(vp->sval);
			vp->sval = tostring(cp->nval);
			vp->tval = STR;
			OBJ2LNG(x) = execute(a[2]);
			if (isbreak(x)) {
				OBJ2LNG(x) = OBJ2LNG(true);
				return(OBJ2LNG(x));
			}
			if (isnext(x) || isexit(x))
				return(OBJ2LNG(x));
			tempfree(OBJ2LNG(x));
		}
	}
}

lobj jump(a,n) node **a;
{
	obj x;

	x.otype = OJUMP;
	switch (n) {
	default:
		error(FATAL, "illegal jump type %d", n);
		break;
	case EXIT:
		x.osub = JEXIT;
		break;
	case NEXT:
		x.osub = JNEXT;
		break;
	case BREAK:
		x.osub = JBREAK;
		break;
	case CONTINUE:
		x.osub = JCONT;
		break;
	}
	return(OBJ2LNG(x));
}

lobj fncn(a,n) node **a;
{
	obj x;
	awkfloat u;
	int t;

	t = (int) a[0];
	OBJ2LNG(x) = execute(a[1]);
	if (t == FLENGTH)
		u = (awkfloat) strlen(getsval(x.optr));
	else if (t == FLOG)
		u = log(getfval(x.optr));
	else if (t == FINT)
		u = (awkfloat) (long) getfval(x.optr);
	else if (t == FEXP)
		u = exp(getfval(x.optr));
	else if (t == FSQRT)
		u = sqrt(getfval(x.optr));
	else
		error(FATAL, "illegal function type %d", t);
	tempfree(OBJ2LNG(x));
	OBJ2LNG(x) = gettemp();
	setfval(x.optr, u);
	return(OBJ2LNG(x));
}

lobj print(a,n) node **a;
{
	register node *x;
	obj y;
	char s[RECSIZE];

	s[0] = '\0';
	for (x=a[0]; x!=NULL; x=x->nnext) {
		OBJ2LNG(y) = execute(x);
		strcat(s, getsval(y.optr));
		tempfree(OBJ2LNG(y));
		if (x->nnext==NULL)
			strcat(s, *ORS);
		else
			strcat(s, *OFS);
	}
	if (strlen(s) >= RECSIZE)
		error(FATAL, "string %.20s ... too long to print", s);
	if (a[1]==nullstat) {
		printf("%s", s);
		return(OBJ2LNG(true));
	}
	redirprint(s, (int)a[1], a[2]);
	return(OBJ2LNG(false));
}

lobj nullproc() {}

lobj nodetoobj(a) node *a;
{
	obj x;

	x.optr = (cell *) a->nobj;
	x.otype = OCELL;
	x.osub = a->subtype;
	if (isfld(x)) fldbld();
	return(OBJ2LNG(x));
}

redirprint(s, a, b) char *s; node *b;
{
	register int i;
	obj x;

	OBJ2LNG(x) = execute(b);
	getsval(x.optr);
	for (i=0; i<FILENUM; i++)
		if (strcmp(x.optr->sval, files[i].fname) == 0)
			goto doit;
	for (i=0; i<FILENUM; i++)
		if (files[i].fp == 0)
			break;
	if (i >= FILENUM)
		error(FATAL, "too many output files %d", i);
	if (a == '|')	/* a pipe! */
		files[i].fp = popen(x.optr->sval, "w");
	else if (a == APPEND)
		files[i].fp = fopen(x.optr->sval, "a");
	else
		files[i].fp = fopen(x.optr->sval, "w");
	if (files[i].fp == NULL)
		error(FATAL, "can't open file %s", x.optr->sval);
	files[i].fname = tostring(x.optr->sval);
doit:
	fprintf(files[i].fp, "%s", s);
	tempfree(OBJ2LNG(x));
}

#include "proctab.c"
/*#include "printname.c"*/ /* not required unless debugging can leave out to save 800 bytes */
