/*
 * quicksort_test.cpp
 *
 *  Created on: 2011-11-27
 *      Author: Yan Shankai
 */

#include "libyskalgrthms/sort/quicksort_tmplt.h"
#include <stdio.h>

void test_quicksort() {
	bool is_asc = false;
	bool is_unique = true;
	const unsigned int size = 14;
	unsigned int a[size] = { 7, 2, 5, 44, 6, 12, 22, 15, 10, 23, 42, 18, 21, 17 };
//	unsigned int a[size] = { 7, 2, 5, 44, 5, 2, 2, 5, 10, 23, 44, 10, 22, 7 };
	unsigned int index[size];
	unsigned int length;

	for (unsigned int i = 0; i < size; i++)
		index[i] = i;

	printf("before quicksort:\t");
	for (unsigned int i = 0; i < size; i++)
		printf("%u, ", a[i]);

	printf("\nafter quicksort:\t");
	length = quicksort<unsigned int>(a, size, is_asc, is_unique, index);
	for (unsigned int i = 0; i < length; i++)
		printf("%u, ", a[i]);
	printf("\nsorted index:\t");
	for (unsigned int i = 0; i < length; i++)
		printf("%u, ", index[i]);
	printf("\n");
}
