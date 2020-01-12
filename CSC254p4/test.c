#include <stdio.h>
#include <stdlib.h>
#include "func1.c"
#include "func2.c"

int main(int argc, char* argv[]) {
	int i=4;
	print1();
	print2();
	print2();
	print2();
	int o=9;
	while(o<=16)
	{
		print2();
		o=o+2;
	}
}