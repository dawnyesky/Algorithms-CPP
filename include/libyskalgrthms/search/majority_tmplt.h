/*
 * majority_tmplt.h
 *
 *  Created on: 2012-9-26
 *      Author: "Yan Shankai"
 */

#ifndef MAJORITY_TMPLT_H_
#define MAJORITY_TMPLT_H_

#include <stdlib.h>

/*
 * description: 找出目标数组中多数元素的候选值
 * 	  method: 减治法(减可变规模)数组中同时删除两个不相同的元素，数组中的多数元素不变
 *  parameters: array: 	目标数组
 *  			   length:	数组长度
 *  			  		  m:	待测试元素的下标
 *      return: 多数元素的候选值
 */
template<typename ElementType>
ElementType* candidate(ElementType array[], unsigned int length,
		unsigned int m) {
	unsigned int i = m;
	ElementType* c = &array[m];
	unsigned int count = 1;
	while (i < length && count > 0) {
		i++;
		//对于跟待测元素相同的则累加，否则减1，当因为count==0而跳出循环的时候
		//相当于把之前出现的元素全部删除然后把剩余数组递归求解
		if (array[i] == *c) {
			count++;
		} else {
			count--;
		}
	}
	if (i == length)
		return c;
	else
		return candidate(array, length, i + 1);
}

/*
 * description: 找出目标数组中的多数元素(出现次数占元素个数的一半以上)
 * 	  method: 先找出候选值，再对候选值进行计数
 *  parameters: array: 	目标数组
 *  			   length:	数组长度
 *      return: 多数元素
 */
template<typename ElementType>
ElementType* majority(ElementType array[], unsigned int length) {
	ElementType* c = candidate(array, length, 0);
	unsigned int count = 0;
	for (unsigned int i = 0; i < length; i++) {
		if (array[i] == *c) {
			count++;
		}
	}

	if (count > length / 2) {
		return c;
	} else
		return NULL;
}

#endif /* MAJORITY_TMPLT_H_ */
