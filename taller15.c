/*
 * daemonize.c
 * This example daemonizes a process, writes a few log messages,
 * sleeps 20 seconds and terminates afterwards.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>

static void skeleton_daemon()
{
	pid_t pid;

	pid = fork();

	if (pid < 0){
        exit(EXIT_FAILURE);
	}

	if (pid > 0){
		exit(EXIT_SUCCESS);
	}

	if (setsid() < 0){
		exit(EXIT_FAILURE);
	}
    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
	pid = fork();

    /* An error occurred */
	if (pid < 0){
		exit(EXIT_FAILURE);
	}
    /* Success: Let the parent terminate */
    	if (pid > 0){
        	exit(EXIT_SUCCESS);
	}
    /* Set new file permissions */
    	umask(0);

	int archivo = open("log_cpu.log",O_TRUNC | O_CREAT);

	setsid();

	
    /* Change the working directory to the root directory */
    /* or another appropriated directory */
	chdir("/");

    /* Close all open file descriptors */
    int x;
	for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    	{
 	       close (x);
	}

    /* Open the log file */
	openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

int main()
{
	skeleton_daemon();

	while (1)
	{
        	skeleton_daemon();
        	syslog (LOG_NOTICE, "First daemon started.");
        	sleep (20);

		char * buf = (char *)malloc(sizeof(char)*100);

		FILE *fp = popen("top -bn2 | grep '%Cpu' | tail -1", "r");
		fread(buf,100,1, fp);
		puts(buf);

        	break;
	}

	syslog (LOG_NOTICE, "First daemon terminated.");
	closelog();

	return EXIT_SUCCESS;
}
