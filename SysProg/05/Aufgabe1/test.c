#include "mystdlib.h"
#include <stdio.h>

int main() {
	char *buf1 = mymalloc(900000);
	if (buf1 == NULL) {
		printf("Error 404, allocated memory not found...\n");
		return -1;
	}
	myfree(buf1);

	char *buf2 = mymalloc(500000);
	char *buf3 = mymalloc(400000);
	char *buf4 = mymalloc(300000);

	if (buf2 == NULL || buf3 == NULL) {
		printf("Fix your mymalloc!\n");
		return -1;
	}

	if (buf4 == NULL) {
		printf("This was supposed to happen. Very good!\n");
	} else {
		printf("Nope. That's wrong. Very wrong.\n");
		return -1;
	}

	myfree(buf2);
	myfree(buf3);
	myfree(buf4);
	
	char *buf5 = mymalloc(900000);
	if (buf5 == NULL) {
		printf("You got so far, but your error free journey ends here. Because an error occured.\n");
		return -1;
	}

	char *buf6 = myrealloc(buf5, 500000);
	myfree(buf6);

	printf("Congrats, you passed all tests. Your malloc and free seem to work\n");
	
}
