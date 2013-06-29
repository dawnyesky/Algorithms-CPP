/*
 * string.cpp
 *
 *  Created on: 2011-12-4
 *      Author: Yan Shankai
 */

#include <math.h>
#include <string.h>
#include "libyskalgrthms/util/string.h"

unsigned int ysk_atoi(char *character, unsigned int length) {
	unsigned int digit = 0;
	for (unsigned int i = length - 1, j = 0; i >= 0; i--, j++) {
		digit += (character[i] - '0') * pow(10, j);
	}
	return digit;
}

char* itoa(int integer, unsigned int radix) {
	if (radix != 10)
		return NULL;
	char rev_int_str[12] = { '\0' };
	int positive_int = integer;
	char num_str;
	if (integer < 0) {
		positive_int = -integer;
	}
	for (int i = 0; i < 10; i++) {
		num_str = '0' + positive_int % radix;
		positive_int = positive_int / radix;
		rev_int_str[i] = num_str;
		if (positive_int == 0)
			break;
	}
	unsigned int str_len = strlen(rev_int_str);
	char *int_str = new char[str_len + 2];
	memset(int_str, '\0', str_len + 2);
	int start_pos = 0;
	if (integer < 0) {
		int_str[0] = '-';
		start_pos = 1;
	}
	for (unsigned int i = 0; i < str_len; i++) {
		int_str[start_pos + i] = rev_int_str[str_len - 1 - i];
	}
	return int_str;
}

char* ivtoa(int* int_vec, unsigned int int_num, const char* separator,
		unsigned int radix) {
	if (radix != 10)
		return NULL;
	char* ints_str = new char[100];
	memset(ints_str, '\0', 100);
	for (unsigned int i = 0; i < int_num; i++) {
		char *int_str = itoa(int_vec[i], radix);
		strcat(ints_str, int_str);
		strcat(ints_str, separator);
		if (int_str != NULL)
			delete int_str;
	}
	size_t ints_str_len = strlen(ints_str);
	size_t separator_len = strlen(separator);
	size_t offset = ints_str_len - separator_len;
	if (separator_len > 0) {
		memset(ints_str + offset, '\0', separator_len);
	}
	return ints_str;
}
