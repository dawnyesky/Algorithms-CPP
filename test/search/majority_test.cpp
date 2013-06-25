/*
 * majority_test.cpp
 *
 *  Created on: 2012-9-26
 *      Author: "Yan Shankai"
 */

#include "libyskalgrthms/search/majority_tmplt.h"
#include <stdio.h>

void test_majority() {
	const unsigned int size = 14;
	unsigned int array[size] = { 7, 2, 2, 44, 5, 2, 2, 5, 2, 2, 44, 10, 2, 2 };
	unsigned int* majority_element = majority<unsigned int>(array, size);
	printf("The majority element of the array: [");
	unsigned int i = 0;
	for (i = 0; i < size - 1; i++)
		printf("%u, ", array[i]);
	if (majority_element != NULL) {
		printf("%u] is: %u\n", array[i], *majority_element);
	} else {
		printf("%u] is not existed\n", array[i]);
	}
}
