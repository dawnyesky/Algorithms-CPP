/*
 * digit_util_test.cpp
 *
 *  Created on: 2012-3-4
 *      Author: caspar
 */

#include <stdio.h>
#include "libyskalgrthms/math/digit_util.h"

void test_to_int() {
	double digit = 4.63;
	printf("%f to int is: %u\n", digit, double2int(digit));
}
