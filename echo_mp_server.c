#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUF_SIZE 50

void read_childProcess(int);

int main(int argc, const char* args[])
{
	int serv_sock, clnt_sock = 0;
	struct sockaddr_in serv_adr, clnt_adr;
	memset(&serv_adr, 0, sizeof serv_adr);
	memset(&clnt_adr, 0, sizeof clnt_adr);
	pid_t pid = 0;
	socklen_t adr_size = 0;
	char buf[BUF_SIZE] = {'\0',};
	if(argc !=2)
	{
		fprintf(stdout, "Usage : %s <Port> \r\n", args[0]);
		exit(EXIT_FAILURE);
	}
	struct sigaction act = {NULL, 0U, 0};
	act.sa_handler = read_childProcess;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	int state = sigaction(SIGCHLD, &act, 0);

	//socket
	//
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(args[1]));

	//bind
	//
	int bind_state = bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof serv_adr);
	if(bind_state == -1)
	{
		fprintf(stdout, "bind() error");
		exit(EXIT_FAILURE);
	}
	//listen
	//
	int listen_state = listen(serv_sock, 5);
	if(listen_state == -1)
	{
		fprintf(stdout, "listen() error");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		adr_size = sizeof clnt_adr;
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_size);
		if(clnt_sock == -1)
		{
			continue;
		}else
		{
			puts("new clients is connected..");
		}
		pid = fork();
		if(pid == -1) //A child process is failure
		{
			close(clnt_sock);
			continue;
		}
		if(pid == 0)	// making a child process
		{
			close(serv_sock);
			int str_length = 0;
			while((str_length = read(clnt_sock, buf, BUF_SIZE)) != 0){
				write(clnt_sock, buf, str_length);
			}
			close(clnt_sock);
			puts("A clients is disconnected...");
			exit(EXIT_FAILURE);

		}
		else close(clnt_sock);
	}
	close(serv_sock);

	return 0;
}
void read_childProcess(int sig)
{
	pid_t pid = 0;
	int status = 0;
	pid = waitpid(-1, &status, WNOHANG);
}
