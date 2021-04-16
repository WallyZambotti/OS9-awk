/* perror.c
 */
#include <stdio.h>
#include <errno.h>

int perror(str)
char *str;
{
	/*extern int errno;*/
	if (!str)
		str = "error";
	printf("%s: %d\n", str, errno);
}
