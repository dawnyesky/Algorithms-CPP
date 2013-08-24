/*
 * string.h
 *
 *  Created on: 2011-12-4
 *      Author: Yan Shankai
 */

#ifndef STRING_H_
#define STRING_H_

unsigned int ysk_atoi(char *character, unsigned int length);
char* itoa(int integer, unsigned int radix = 10);
char* ivtoa(int* int_vec, unsigned int int_num, const char* separator = "",
		unsigned int radix = 10);

char* trim(char* string, const char* delimeters, unsigned int& string_len);
char* trim_left(char* string, const char* delimeters, unsigned int& string_len);
char* trim_right(char* string, const char* delimeters,
		unsigned int& string_len);

#endif /* STRING_H_ */
