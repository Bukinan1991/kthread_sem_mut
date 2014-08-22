#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
main()
{
    pid_t pid, old_ppid, new_ppid;
    pid_t child, parent;
    parent = getpid();
    if(child == 0)
    {
	old_ppid = getppid();
	sleep(5);
	new_ppid = getppid();
    }
    else
    {
	sleep(5);
	exit(0);
    }
    printf("Perent: %d\n", parent);
    printf("Child: %d\n", getpid());
    printf("Child_old: %d\n", old_ppid);
    printf("Child_new: %d\n", new_ppid);
}