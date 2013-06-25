/*
 * arrange_combine.cpp
 *
 *  Created on: 2011-11-29
 *      Author: Yan Shankai
 */

#include "libyskalgrthms/math/arrange_combine.h"

unsigned int arrange(unsigned int m, unsigned int n) {
	unsigned int result = 1;
	for (unsigned int i = m; i > m - n; i--) {
		result *= i;
	}
	return result;
}

unsigned int combine(unsigned int m, unsigned int n) {
	unsigned int result = 1;
	unsigned int i;
	for (i = m; i > m - n; i--) {
		result *= i;
	}
	for (i = n; i > 1; i--) {
		result /= i;
	}
	return result;
}
