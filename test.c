#include <stdio.h>
#include <unistd.h>

int main() 
{
	write(1, "out\n", 4);
	write(2, "err\n", 4);
}
