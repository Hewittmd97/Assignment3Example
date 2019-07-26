#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define MAX_LINE 4096

int main(int argc, char *argv[])
{

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[MAX_LINE];
  char ch, file_name[128];
  FILE *fp;
  strcpy(file_name, argv[3]);
  fp = fopen(file_name, "r");
  
	if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[2]);

	if((ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0)
  {
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1)
  {
		printf("Client: \t");
		fgets(buffer, 255, (FILE*)fp);
    printf("%s\n", buffer);
		send(clientSocket, buffer, strlen(buffer), 0);
    buffer[strcspn(buffer, "\n")] = 0;
		if(strcmp(buffer, "end") == 0)
    {
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 1024, 0) < 0)
    {
			printf("[-]Error in receiving data.\n");
		}
    else
    {
			printf("Server: \t%s\n", buffer);
      sleep(1);
		}
   bzero(buffer, sizeof(buffer));
	}
	return 0;
}