#include <stdlib.h>
#include <stdio.h>

int main(){
	int lol = 5;
	printf("%d", __sync_val_compare_and_swap(&lol, 5, 1));
	printf("%d", lol);
	return 0;
}
