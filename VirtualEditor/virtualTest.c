#include <stdio.h>

int adderObfuscated(int para01, int para02) {
	int res = 0;
	res = para01 + para02;
	res = res * 100;
	return res;
}

int main()
{
    printf("%d", adderObfuscated(1,2));
    return 0;
}
