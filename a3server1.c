#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_LINE 4096

int main(int argc, char *argv[])
{
	int sockfd, ret;
	struct sockaddr_in serverAddr;
	int newSocket;
	struct sockaddr_in newAddr;
	socklen_t addr_size;
	char buffer[MAX_LINE];
	pid_t childpid;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	char path[MAX_LINE];
	if(sockfd < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[2]);
	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %s\n", argv[1]);
	if(listen(sockfd, 10) == 0)
	{
		printf("[+]Listening....\n");
	}
	else
	{
		printf("[-]Error in binding.\n");
	}
	while(1)
	{
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0)
		{
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		if((childpid = fork()) == 0)
		{
      printf("Server worker PID: %ld\n", (long)getpid());
      system("date");
			close(sockfd);
			while(1)
			{
        memset(buffer, 0, sizeof(buffer));
				recv(newSocket, buffer, MAX_LINE, 0);
        buffer[strcspn(buffer, "\n")] = 0;
        printf("%s\n", buffer);
				if(strcmp(buffer, "end") == 0)
				{
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
          close(newSocket);
          printf("Server worker terminated.\n");
					break;
				}
				else
				{
					FILE* fp = popen(buffer, "r");
          memset(buffer, 0, sizeof(buffer));
					while (fgets(path, MAX_LINE, fp) != NULL)
					{
						send(newSocket, path, strlen(path), 0);
					}
					
          fclose(fp);
				}
			}
		}
	}
	close(newSocket);
	return 0;
}