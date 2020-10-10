#include <stdio.h>

int adderObfuscated(int para01, int para02) {
	int res = 0;

	for (int i = 0; i >= 0;) {
		switch (i)
		{
		case 0:
			res = para01 + para02;
			i = 2;
			break;
		case 1:
			return res;
		case 2:
			res = res * 100;
			i = 1;
			break;
		}
	}
	return 0;
}

int main()
{
    printf("%d", adderObfuscated(1,2));
    return 0;
}
