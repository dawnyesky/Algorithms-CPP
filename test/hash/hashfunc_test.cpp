/*
 * hashfunc_test.cpp
 *
 *  Created on: 2011-11-27
 *      Author: Yan Shankai
 */

#include <stdio.h>

extern unsigned int RSHash(char* str, unsigned int len);

void test_RSHash() {
	char data[3] = { 'a', 'b', 'c'};
	unsigned int hash_code = RSHash(data, 3);
	printf("the data is: ");
	for (int i = 0; i < 3; i++) {
		printf("%i, ", data[i]);
	}
	unsigned int b = 378551;
	unsigned int a = 63689;
	printf("\ncal hash code is: %u", a * b * b * (data[0] * a * b + data[1]) + data[2]);
	printf("\nthe hash code is: %u", hash_code%10000);
}
