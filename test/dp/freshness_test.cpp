/*
 * freshness_test.cpp
 *
 *  Created on: 2012-10-8
 *      Author: "Yan Shankai"
 */

#include "libyskalgrthms/dp/freshness_tmplt.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void test_max_freshness_dp() {
	bool is_iter = false;
	const unsigned int n = 10;
	const unsigned int k = 7;
	DefaultElement* elements = new DefaultElement[n];
	DefaultElement* result = new DefaultElement[k];
	const char* fruits[n] = { "apple", "banana", "cherry", "blackberry",
			"chestnut", "coconut", "cumquat", "fig", "grape", "hawthorn" };

	const int scores[n] = { 10, 9, 8, 7, 10, 5, 12, 17, 20, 22 };
	const int coe[n] = { 8, 6, 4, 6, 3, 3, 7, 9, 2, 1 };

	srand((unsigned int) time(NULL));
	for (unsigned int i = 0; i < n; i++) {
		elements[i].identifier = (char*) fruits[i];
//		elements[i].score = rand() % 1000;
//		elements[i].coefficient = rand() % 10;
		elements[i].score = scores[i];
		elements[i].coefficient = coe[i];
	}
	printf("We choose %u from the %u fruits:{ ", k, n);
	for (unsigned int i = 0; i < n; i++) {
		printf("[%s,%i,%i] ", elements[i].identifier, elements[i].score,
				elements[i].coefficient);
	}
	printf("}\nAre: {");

	int max_freshness_score = 0;
	if (is_iter) {
		max_freshness_score = max_freshness_dp_iter<DefaultElement,
				DefaultVaryFuncType>(elements, n, result, k, default_vary_func);
	} else {
		max_freshness_score = max_freshness_dp_rec<DefaultElement,
				DefaultVaryFuncType>(elements, n, result, k, default_vary_func);
	}

	for (unsigned int i = 0; i < k; i++) {
		printf("[%s,%i,%i] ", result[i].identifier, result[i].score,
				result[i].coefficient);
	}
	printf("}\nAnd the maximum freshness value is: %i\n", max_freshness_score);
	delete[] elements;
	delete[] result;
}
