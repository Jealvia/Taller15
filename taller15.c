#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, char* argv[])
{
	FILE *fp= NULL;
	FILE *fp1= NULL;
	pid_t process_id = 0;
	pid_t sid = 0;
	char * buf = (char *)malloc(sizeof(char)*100);
	memset (buf,0,sizeof(char)*100);
	process_id = fork();

	if (process_id < 0)
	{
		printf("Fallo en el fork\n");

		exit(1);
	}

	if (process_id > 0)
	{
		printf("Numero del proceso hijo %d \n", process_id);
		exit(0);
	}
	umask(0);
	sid = setsid();
	if(sid < 0)
	{
		exit(1);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	fp = fopen("log_cpu.log", "w");
	while (1)
	{	
		sleep(2);		
		fp1 = popen("top -bn2 | grep '%Cpu' | tail -1", "r");
		fread(buf,100,1, fp1);
		fwrite(buf, strlen(buf),1,fp);
		memset (buf,0,sizeof(char)*100);
		
	}
	fclose(fp);
	return (0);
}
