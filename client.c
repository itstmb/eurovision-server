
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>

#define BUFFER_SIZE 300

int main(void)
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("socket failed\n");
		exit(0);
	}

	portno = 8888;
	server = gethostbyname("localhost");
	if (server == NULL)
	{
		printf("gethostbyname failed\n");
		exit(0);
	}

	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((void*)&serv_addr.sin_addr.s_addr, (void*)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("connect failed\n");
		exit(0);
	}

	printf("[Client] Connected\n");

  char query[BUFFER_SIZE];
  char result[BUFFER_SIZE];

  while (1)
  {
		memset(query, 0, BUFFER_SIZE);
		fgets(query, BUFFER_SIZE, stdin);
		if(strncmp(query, "exit", 4) == 0)
		{
				break;
		}

    n = write(sockfd, query, BUFFER_SIZE);
		if (n < 0) {
			printf("can't write\n");
		}

		bzero(result, BUFFER_SIZE);
		n = read(sockfd, result, BUFFER_SIZE);
		if (n < 0) {
			printf("can't read back from server\n");
		}

		printf("%s", result);
		sleep(1);
  }
	close(sockfd);
  return 0;
}
