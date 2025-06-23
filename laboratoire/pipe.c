#include "../minishell.h"

void read_from_pipe(int fd, size_t size)
{
    char *buff;
    ssize_t bytes;
    buff = malloc(sizeof(char) * size + 1);
    bytes = read(fd, buff, size);
    buff[bytes] = '\0';
    printf("[%s]\n", buff);
    free(buff);
}

void write_to_pipe(int fd, char *str, size_t size)
{
    write(fd, str, size);
}

// int main()
// {
//     pid_t pid;
//     int fd[2];
//     char *str = "zwizooo";

//     if (pipe(fd))
//         printf("Pipe failed.\n");
//     //printf("%d %d\n", fd[0], fd[1]);
//     pid = fork();
//     if (pid > (pid_t)0)
//     {
//         close(fd[0]);
//         write_to_pipe(fd[1], str, strlen(str));
//     }
//     else if (pid < (pid_t)0)
//     {    
//         printf("Fork failed.\n");
//         return (-1);
//     }
//     else if (pid == (pid_t)0)
//     {
//         close(fd[1]);
//         read_from_pipe(fd[0], strlen(str));
//         return (0);
//     }
//     return(0);
// }

// int main()
// {
//     pid_t pid;
//     char *str1 = "hello from parent";
//     char *str2 = "hi from child";
//     int pipe1[2];
//     int pipe2[2];

//     if (pipe(pipe1) || pipe(pipe2))
//         printf("Pipe failed\n");
//     pid = fork();
//     if (pid > (pid_t)0)
//     {
//         close(pipe1[0]);
//         close(pipe2[1]);
//         write_to_pipe(pipe1[1], str1, strlen(str1));
//         read_from_pipe(pipe2[0], strlen(str2));
//         close(pipe1[1]);
//         close(pipe2[0]);
//         return 0;
//     }
//     else if (pid == (pid_t)0)
//     {
//         close(pipe1[1]);
//         close(pipe2[0]);
//         read_from_pipe(pipe1[0], strlen(str1));
//         write_to_pipe(pipe2[1], str2, strlen(str2));
//         close(pipe1[0]);
//         close(pipe2[1]);
//         return 0;
//     }
//     else if (pid < (pid_t)0)
//     {    
//         printf("Fork failed.\n");
//         return (-1);
//     }
//     return 0;
// }

typedef struct {
    char buffer[1024];
    int write_pos;
    int read_pos;
} myPipe;

void	my_write(myPipe *pipe, char *str)
{
	if (pipe == NULL || str == NULL)
		return ;
	while (str[pipe->write_pos])
	{
		pipe->buffer[pipe->write_pos] = str[pipe->write_pos];
		pipe->write_pos++;
	}
	pipe->buffer[pipe->write_pos++] = '\0';
}

void	my_read(myPipe *pipe, char *dest)
{
	if (pipe == NULL || dest == NULL)
		return ;
	while (pipe->buffer[pipe->read_pos])
	{
		dest[pipe->read_pos] = pipe->buffer[pipe->read_pos];
		pipe->read_pos++;
	}
	dest[pipe->read_pos++] = '\0';
}

int	main()
{
	myPipe	pipe = {.read_pos = 0, .write_pos = 0};
	char buff[100];
	my_write(&pipe, "hello\n");
	my_read(&pipe, buff);
	printf("%s", buff);
	return 0;
}
