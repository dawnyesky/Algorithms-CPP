/*
 * josephus.cpp
 *
 *  Created on: 2012-9-27
 *      Author: "Yan Shankai"
 */

#include <stdio.h>
unsigned int josephus(unsigned int n, unsigned int m, unsigned int k) { //n表示总共人数，m表示报到第几个出局，k表示开始位置（k<=n）
	if (k > n)
		return 0;
	unsigned int pos = 0; //f(1)的情况，现在假设k放在下标为1的数组位置
	for (unsigned int i = 1; i < n - 1; i++) {
		pos = (pos + m) % i + 1;
	}
	return (pos + k) % n;
}
