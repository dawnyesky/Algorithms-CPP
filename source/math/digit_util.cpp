/*
 * digit_util.cpp
 *
 *  Created on: 2012-3-4
 *      Author: caspar
 */

#include <math.h>

double rounds(double digit, unsigned int precision) {
	if (digit > 0) {
		unsigned int m = digit * pow(10, precision + 1);
		return m % 10 < 5 ?
				(m / 10) / (double) pow(10, precision) :
				(m / 10 + 1) / (double) pow(10, precision);
	} else {
		return 0;
	}
}

unsigned int double2int(double digit) {
	return rounds(digit, 0);
}
