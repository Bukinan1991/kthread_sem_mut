#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int i = 0;
    while(i < 2)
    {
	FILE *fd = fopen("/etc/shadow", "r");
	if(fd != NULL)
	{
	    i = 3;
	    fclose(fd);
	}
    }
    printf("Udacha\n");
    return 0;
}