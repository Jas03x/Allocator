
#include <cstdio>

int main(int argc, char* argv[])
{
	int status = 0;

	if (argc != 2)
	{
		status = -1;
		printf("usage: allocator.exe <size_in_gb>\n");
	}

	return 0;
}
