#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int is_dir(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}


int main(void)
{
    char *str = "hey";
    printf("%d\n", is_dir(str));
}
