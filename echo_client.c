#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024
int main(unsigned int argc, const char* args[])
{
        int sock = 0;
        struct sockaddr_in serv_addr;
        char message[BUF_SIZE] = {'0',};
        int str_len = 0;
	
        if(argc != 3) {
                printf("Usage : %s <IP> <PORT> \r\n", args[0]);
                exit(EXIT_FAILURE); //exit(1);
        }
        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(socket == -1){
                puts("socket() Error\r\n");
        }
        memset(&serv_addr, 0, sizeof serv_addr);
        serv_addr.sin_addr.s_addr = inet_addr(args[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(atoi(args[2]));
        int connect_state = connect(sock, (struct sicjaddr*)&serv_addr, sizeof(serv_addr));
        
	while(1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message,"q\n") || !strcmp(message, "Q\n"))
			break;

		write(sock, message, strlen(message));
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
        printf("Message from server : %s \r\n", message);

	}

        return 0;
}

