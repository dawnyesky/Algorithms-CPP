/*
 * quicksort_tmplt.h
 *
 *  Created on: 2011-11-28
 *      Author: Yan Shankai
 */

#ifndef QUICKSORT_TMPLT_H_
#define QUICKSORT_TMPLT_H_

#include <stdlib.h>
#include <set>

template<typename ElementType>
void swap(ElementType array[], unsigned int m, unsigned int n,
		unsigned int index_array[] = NULL) {
	ElementType temp = array[m];
	array[m] = array[n];
	array[n] = temp;

	if (index_array != NULL) {
		unsigned int temp_index = index_array[m];
		index_array[m] = index_array[n];
		index_array[n] = temp_index;
	}
}

template<typename ElementType>
unsigned int partition_it(ElementType array[], unsigned int left,
		unsigned int right, const ElementType &pivot,
		unsigned int index_array[] = NULL) {
	int leftPtr = left - 1;
	int rightPtr = right;
	while (true) {
		while (array[++leftPtr] < pivot)
			// 最多移到pivot
			;
		while (rightPtr > 0 && array[--rightPtr] > pivot)
			// 最多移到首元素
			;
		if (leftPtr >= rightPtr)
			break;
		else
			swap<ElementType>(array, leftPtr, rightPtr, index_array);
	}
	swap<ElementType>(array, leftPtr, right, index_array);
	return leftPtr;
}

template<typename ElementType>
void rec_quicksort(ElementType array[], unsigned int left, unsigned int right,
		unsigned int index_array[] = NULL) {
	if (left >= right)
		return;
	else {
		ElementType pivot = array[right];
		unsigned int partition = partition_it(array, left, right, pivot,
				index_array);
		if (partition != 0) //无符号整数左边界
			rec_quicksort<ElementType>(array, left, partition - 1, index_array);
		if (partition != 0xFFFFFFFF) //无符号整数右边界
			rec_quicksort<ElementType>(array, partition + 1, right,
					index_array);
	}
}

/*
 * description: 前移去重算法
 *  parameters: array: 	 	需要前移的目的地址
 *  			  sub_array:	需要前移的数组
 *  			  length：		需要前移的数组长度
 *      return: 前移去重后数组的长度
 */
template<typename ElementType>
unsigned int move_forward_unique(ElementType array[], ElementType sub_array[],
		unsigned int move_length, unsigned int index_array[] = NULL,
		unsigned int sub_index_array[] = NULL) {
	unsigned int i;
	if (move_length == 0)
		return 0;
	for (i = 0; i < move_length - 1; i++) { //遍历需要前移的数组
		if (sub_array[i] == sub_array[i + 1]) { //与后面一个元素比较， 如果相等则去重
			unsigned int sub_length = 0; //子数组长度
			unsigned int offset = 0; //重复的个数
			while (i + 1 + offset != move_length
					&& sub_array[i + 1 + (++offset)] == sub_array[i])
				//找到后面不重复的第一个元素
				;
			array[i] = sub_array[i]; //先把重复元素的第一个元素前移保留
			if (index_array != NULL)
				index_array[i] = sub_index_array[i];
			if (index_array != NULL)
				sub_length = move_forward_unique(&array[i + 1],
						&sub_array[i + 1 + offset],
						move_length - (i + 1 + offset), &index_array[i + 1],
						&sub_index_array[i + 1 + offset]);
			else
				sub_length = move_forward_unique(&array[i + 1],
						&sub_array[i + 1 + offset],
						move_length - (i + 1 + offset)); //把后面的子数组递归进行前移去重
			return i + 1 + sub_length;
		}
		array[i] = sub_array[i]; //不相等则前移
		if (index_array != NULL)
			index_array[i] = sub_index_array[i];
	}
	array[i] = sub_array[i]; //最后一个元素只需要前移
	if (index_array != NULL)
		index_array[i] = sub_index_array[i];
	return move_length; //子数组没有重复元素
}

/*
 * description: 去重算法
 *  parameters: array: 	待去重数组
 *  			  length：	数组长度
 *      return: 去重后数组的长度
 */
template<typename ElementType>
unsigned int unique(ElementType array[], unsigned int length,
		unsigned int index_array[] = NULL) {
	return move_forward_unique<ElementType>(array, array, length, index_array,
			index_array);
}

/*
 * description: 快排算法
 *  parameters: array: 			待排序数组
 *  			  length：			数组长度
 *  			  unique：			是否需要去重操作，若要对类对象去重必须重写==操作符
 *  			  index_array: 	索引数组，必须提供一个等长的数组来存储索引的变化。
 *      return: 排好序的数组长度
 */
template<typename ElementType>
unsigned int quicksort(ElementType array[], unsigned int length, bool is_asc,
		bool is_unique, unsigned int index_array[] = NULL) {
	if (length <= 1) {
		return length;
	}
	rec_quicksort<ElementType>(array, 0, length - 1, index_array);
	if (!is_asc) {
		for (unsigned int i = 0; i < length / 2; i++) {
			swap<ElementType>(array, i, length - i - 1, index_array);
		}
	}
	if (!is_unique) {
		return length;
	} else {
		return unique<ElementType>(array, length, index_array);
	}
}

#endif /* QUICKSORT_TMPLT_H_ */
