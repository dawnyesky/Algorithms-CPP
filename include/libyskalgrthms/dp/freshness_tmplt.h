/*
 * freshness_tmplt.h
 *
 *  Created on: 2012-10-20
 *      Author: "Yan Shankai"
 */

#ifndef FRESHNESS_TMPLT_H_
#define FRESHNESS_TMPLT_H_
#include <stdio.h>
#include <stdlib.h>
#include "libyskalgrthms/math/arrange_combine.h"
#include "libyskalgrthms/sort/quicksort_tmplt.h"

struct DefaultElement {
	char* identifier; //标识符
	int score; //得分
	int coefficient; //分数递减系数

	bool operator<(const DefaultElement& element) {
		if (this->coefficient == element.coefficient) {
			return this->score < element.score;
		} else {
			return this->coefficient < element.coefficient;
		}
	}
	bool operator>(const DefaultElement& element) {
		if (this->coefficient == element.coefficient) {
			return this->score > element.score;
		} else {
			return this->coefficient > element.coefficient;
		}
	}
	bool operator==(const DefaultElement& element) {
		return this->coefficient == element.coefficient
				&& this->score == element.score;
	}
};

typedef int (*DefaultVaryFuncType)(DefaultElement* element, unsigned int step);

/*
 * description: 默认分数递减函数
 *  parameters: element: 	元素对象
 *  			   	 step:	已经过去的步数(天数)
 *      return: 元素对象经过递减以后所得的分数，可以为负数
 */
template<typename ElementType>
int default_vary_func(ElementType* element, unsigned int step) {
	return element->score - step * element->coefficient;
}

template<typename ElementType>
void swap(ElementType array[], unsigned int index_array[], unsigned int m,
		unsigned int n) {
	ElementType temp = array[m];
	array[m] = array[n];
	array[n] = temp;

	unsigned int temp_index = index_array[m];
	index_array[m] = index_array[n];
	index_array[n] = temp_index;
}

/*
 * description: 递归找出新鲜度最高的有序序列
 * 	  method: 自顶向下，动态规划
 * 	  				 递归式：dp(i,j)=max{dp(i-1,j), dp(i-1,j-1)-v(i,j)}
 * 	  				 	 dp(i,j)表示从已排序的前i个元素里面选择j个的最大总新鲜度，v(i,j)表示第i个元素在j次选择以后新鲜度的递减量
 *  parameters: array: 	待选元素数组
 *  			  		  i:  可选元素数量
 *  			  		  j:	需要选择的元素数量
 *      return: 最高新鲜度
 */
template<typename ElementType, typename VaryFuncType>
int rec_mf_dp(ElementType array[], unsigned int i, unsigned int j,
		unsigned int** selected_index, int** max_freshness,
		VaryFuncType vary_func) {
	if (i == 0) {
		selected_index[i][j] = i;
		max_freshness[i][j] = array[i].score;
		return max_freshness[i][j];
	} else if (j == 0) {
		if (array[i].score
				> rec_mf_dp(array, i - 1, j, selected_index, max_freshness,
						vary_func)) {
			selected_index[i][j] = i;
			max_freshness[i][j] = array[i].score;
		} else {
			selected_index[i][j] = selected_index[i - 1][j];
			max_freshness[i][j] = max_freshness[i - 1][j];
		}
		return max_freshness[i][j];
	} else if (i == j) {
		selected_index[i][j] = i;
		max_freshness[i][j] = rec_mf_dp(array, i - 1, j - 1, selected_index,
				max_freshness, vary_func) + vary_func(array + i, j);
		return max_freshness[i][j];
	} else {
		max_freshness[i][j] = rec_mf_dp(array, i - 1, j - 1, selected_index,
				max_freshness, vary_func) + vary_func(array + i, j);
		if (max_freshness[i][j]
				> rec_mf_dp(array, i - 1, j, selected_index, max_freshness,
						vary_func)) {
			selected_index[i][j] = i;
		} else {
			selected_index[i][j] = selected_index[i - 1][j];
			max_freshness[i][j] = max_freshness[i - 1][j];
		}
		return max_freshness[i][j];
	}
}

/*
 * description: 找出新鲜度最高的有序序列
 * 	  method: 动态规划，自底向上，递归
 *  parameters: array: 	待选元素数组
 *  			   length:	数组长度
 *  			   result:  选择的序列
 *  			  		  k:	需要选择的序列长度
 *      return: 最高新鲜度
 */
template<typename ElementType, typename VaryFuncType>
unsigned int max_freshness_dp_rec(ElementType array[],
		const unsigned int length, ElementType result[], const unsigned int k,
		VaryFuncType vary_func) {
	//输入参数不合法
	if (k > length || k == 0)
		return 0;

	//max_freshness[i][j]表示在前i个元素里选择j个元素得到的最大新鲜度
	int** max_freshness = new int*[length];
	//selected_index[i][j]表示在前i个元素里选择第j个元素时作出的选择
	unsigned int** selected_index = new unsigned int*[length];
	for (unsigned int i = 0; i < length; i++) {
		max_freshness[i] = new int[length];
		selected_index[i] = new unsigned int[length];
	}

	//先对元素数组根据分数递减系数由大到小排列
	quicksort<ElementType>(array, length, false, false);

	int max_freshness_value = rec_mf_dp<ElementType, VaryFuncType>(array,
			length - 1, k - 1, selected_index, max_freshness,
			default_vary_func);

	//构造最优解
	int i = length - 1;
	int j = k - 1;
	while (i > j && j >= 0) {
		if (selected_index[i][j] != selected_index[i - 1][j]) {
			result[j] = array[selected_index[i][j]];
			i--;
			j--;
		} else {
			i--;
		}
	}
	if (j >= 0) {
		for (int k = 0; k <= j; k++) {
			result[k] = array[selected_index[k][k]];
		}
	}

	//清理资源
	for (unsigned int i = 0; i < length; i++) {
		delete[] max_freshness[i];
		delete[] selected_index[i];
	}
	delete[] max_freshness;
	delete[] selected_index;
	return max_freshness_value;
}

/*
 * description: 找出新鲜度最高的有序序列
 * 	  method: 动态规划，自底向上，迭代
 *  parameters: array: 	待选元素数组
 *  			   length:	数组长度
 *  			   result:  选择的序列
 *  			  		  k:	需要选择的序列长度
 *      return: 最高新鲜度
 */
template<typename ElementType, typename VaryFuncType>
unsigned int max_freshness_dp_iter(ElementType array[],
		const unsigned int length, ElementType result[], const unsigned int k,
		VaryFuncType vary_func) {
	//输入参数不合法
	if (k > length || k == 0)
		return 0;

	//由于在算法中需要临时交换元素的位置，所以需要与原元素数组对应的索引数组来辅助
	unsigned int* index_array = new unsigned int[length];
	for (unsigned int i = 0; i < length; i++) {
		index_array[i] = i;
	}

	//max_freshness[i][j]表示在前i个元素里选择j个元素得到的最大新鲜度
	int max_freshness[length][length];
	//selected_index[i][j]表示在前i个元素里选择第j个元素时作出的选择
	unsigned int selected_index[length][length];

	//先对元素数组根据分数递减系数由大到小排列
	quicksort<ElementType>(array, length, false, false);
//	printf("After sort:\n");
//	for (unsigned int i = 0; i < length; i++) {
//		printf("[%s,%i,%i] ", array[i].identifier, array[i].score,
//				array[i].coefficient);
//	}
//	printf("\n");

	//自底向上迭代计算
	selected_index[0][0] = 0;
	max_freshness[0][0] = array[0].score;
	for (unsigned int i = 1; i < length; i++) {
		unsigned int j = 0;

		if (array[i].score > max_freshness[i - 1][j]) {
			selected_index[i][j] = i;
			max_freshness[i][j] = array[i].score;
		} else {
			selected_index[i][j] = selected_index[i - 1][j];
			max_freshness[i][j] = max_freshness[i - 1][j];
		}

		for (j = 1; j < i; j++) {
			max_freshness[i][j] = max_freshness[i - 1][j - 1]
					+ vary_func(array + i, j);
			if (max_freshness[i][j] > max_freshness[i - 1][j]) {
				selected_index[i][j] = i;
			} else {
				selected_index[i][j] = selected_index[i - 1][j];
				max_freshness[i][j] = max_freshness[i - 1][j];
			}
		}

		selected_index[i][j] = i;
		max_freshness[i][j] = max_freshness[i - 1][j - 1]
				+ vary_func(array + i, j);
	}

	//构造最优解
	int i = length - 1;
	int j = k - 1;
	while (i > j && j >= 0) {
		if (selected_index[i][j] != selected_index[i - 1][j]) {
			result[j] = array[selected_index[i][j]];
			i--;
			j--;
		} else {
			i--;
		}
	}
	if (j >= 0) {
		for (int k = 0; k <= j; k++) {
			result[k] = array[selected_index[k][k]];
		}
	}

//	printf("max_freshness: \n");
//	for (unsigned int u = 0; u < length; u++) {
//		for (unsigned int v = 0; v < length; v++) {
//			printf("%u, ", max_freshness[u][v]);
//		}
//		printf("\n");
//	}
//
//	printf("selected_index: \n");
//	for (unsigned int u = 0; u < length; u++) {
//		for (unsigned int v = 0; v < length; v++) {
//			printf("%u, ", selected_index[u][v]);
//		}
//		printf("\n");
//	}

	return max_freshness[length - 1][k - 1];
}

#endif /* FRESHNESS_TMPLT_H_ */
