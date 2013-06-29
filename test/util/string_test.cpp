/*
 * string_test.cpp
 *
 *  Created on: 2012-3-9
 *      Author: caspar
 */

#include "libyskalgrthms/util/string.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void test_atoi() {

}

void test_itoa() {
	srand((unsigned int) time(NULL));
	int digit = rand() % 100;
	char *result = itoa(digit, 10);
	printf("The string of the digit %d is \"%s\"\n", digit, result);
	if (result != NULL)
		delete[] result;
}

void test_ivtoa() {
	int n = 5;
	int *digit = new int[n];
	srand((unsigned int) time(NULL));
	printf("The string of the digits ");
	for (int i = 0; i < n; i++) {
		digit[i] = rand() % 100;
		printf("%d ", digit[i]);
	}
	char *ints_str = ivtoa(digit, n, ",", 10);
	printf("is: \"%s\"\n", ints_str);
	if (ints_str != NULL)
		delete[] ints_str;
}
