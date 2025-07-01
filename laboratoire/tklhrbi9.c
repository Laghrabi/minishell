#include <stdio.h>
#include <unistd.h>

int main()
{
    const char *path = "./pipe.c";
    printf("%s", path);
    if (access(path, F_OK | W_OK) == 0)
        printf("The file exists.\n");
    else
        perror("File check");
    if (access(path, R_OK) == 0)
        printf("You have read permission.\n");
    else
        perror("Read check");
    if (access(path, W_OK) == 0)
        printf("You have write permission.\n");
    else
        perror("Write check");
    return 0;
}
