/*
 * freshness_tmplt.h
 *
 *  Created on: 2012-10-8
 *      Author: "Yan Shankai"
 */

#ifndef FRESHNESS_TMPLT_H_
#define FRESHNESS_TMPLT_H_

#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <libyskalgrthms/math/arrange_combine.h>

struct DefaultElement {
	char* identifier; //标识符
	int score; //得分
	int coefficient; //分数递减系数
};

typedef int (*DefaultVaryFuncType)(DefaultElement* element, unsigned int step);

/*
 * description: 默认分数递减函数
 *  parameters: element: 	元素对象
 *  			   	 step:	已经过去的步数(天数)
 *      return: 元素对象经过递减以后所得的分数，如果得分降为小于等于0时返回0
 */
template<typename ElementType>
int positive_vary_func(ElementType* element, unsigned int step) {
	return (element->score > step * element->coefficient) ?
			(element->score - step * element->coefficient) : 0;
}

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
 * 	  method: 自顶向下，没有优化过的暴力树搜索
 * 	  				 递归式：F(i)=max{F(i-1)+f(a[k])-v(k,i)}
 * 	  				 	 a[k]属于ALL_ELEMENT-S(i-1)
 * 	  				 	 F(i)表示第i次选择第k个元素时总新鲜度，f(a[k])表示第k个元素的新鲜度初始值，v(k,i)表示第k个元素在i次选择以后新鲜度的递减量，S(i)表示经过i次选择后的元素集合
 *  parameters: array: 	待选元素数组
 *  			   length:	数组长度
 *  			   result:  选择的序列
 *  			  		  k:	需要选择的序列长度
 *  			  		  i:  已选择的元素个数
 *  			  		  j:	当前递归节点在递归树同一层的偏移量
 *      return: 最高新鲜度(如果剩下的元素的新鲜度到了第k天已经小于零，默认返回剩下的第一个元素)
 */
template<typename ElementType, typename VaryFuncType>
int rec_mf_bf(ElementType array[], unsigned int index_array[],
		unsigned int length, unsigned int n, unsigned int k, unsigned int i,
		unsigned int j, unsigned int** path_index,
		unsigned int** selected_index, VaryFuncType vary_func) {
	int freshness = std::numeric_limits<int>::min();
	unsigned int index = 0;

	//搜索树的叶子节点的直接父节点
	if (k == 1) {
		for (unsigned int t = 0; t < length; t++) {
			int sub_freshness = vary_func(array + t, i);
			//找出剩下的元素中在最后一天里新鲜度最大的
			if (sub_freshness > freshness) {
				index = t;
				freshness = sub_freshness;
			}
		}

		//记录选择的元素
		path_index[j * arrange(n - i, k - 1)][i] = index_array[index];
		selected_index[j * arrange(n - i, k - 1)][i] = index_array[index];
		return freshness;
	}

	//搜索树其余节点
	for (unsigned int t = 0; t < length; t++) {
		swap(array, index_array, 0, t); //把选择的元素与第一个元素交换，然后在剩余的元素中递归进行选择，最后再交换回来
		int sub_freshness = rec_mf_bf(array + 1, index_array + 1, length - 1, n,
				k - 1, i + 1, j * (n - i) + t, path_index, selected_index,
				vary_func) + vary_func(array, i);
		swap(array, index_array, t, 0);
		if (sub_freshness > freshness) { //找出局部新鲜度最大的
			index = t;
			freshness = sub_freshness;
		}
	}

	//记录选择的节点
	for (unsigned int t = 0; t < length; t++) {
		for (unsigned int l = (j * (n - i) + t) * arrange(n - i - 1, k - 2);
				l < (j * (n - i) + t + 1) * arrange(n - i - 1, k - 2); l++) {
			path_index[l][i] = index_array[t];
		}
	}
	for (unsigned int t = j * arrange(n - i, k - 1);
			t < (j + 1) * arrange(n - i, k - 1); t++) {
		selected_index[t][i] = index_array[index];
	}
	return freshness;
}

/*
 * description: 找出新鲜度最高的有序序列
 * 	  method: 暴力树搜索
 *  parameters: array: 	待选元素数组
 *  			   length:	数组长度
 *  			   result:  选择的序列
 *  			  		  k:	需要选择的序列长度
 *      return: 最高新鲜度
 */
template<typename ElementType, typename VaryFuncType>
int max_freshness_bf_rec(ElementType array[], const unsigned int length,
		ElementType result[], const unsigned int k, VaryFuncType vary_func) {
	//输入参数不合法
	if (k > length || k == 0)
		return 0;

	//由于在算法中需要临时交换元素的位置，所以需要与原元素数组对应的索引数组来辅助
	unsigned int* index_array = new unsigned int[length];
	for (unsigned int i = 0; i < length; i++) {
		index_array[i] = i;
	}

	//搜索路径总数
	unsigned int path_num = arrange(length, k - 1);
	//搜索路径
	unsigned int** path_index = new unsigned int*[path_num];
	//每次分支上作的选择，同属于一个父节点的分支选择值是相同的
	unsigned int** selected_index = new unsigned int*[path_num];
	for (unsigned int i = 0; i < path_num; i++) {
		path_index[i] = new unsigned int[k];
		selected_index[i] = new unsigned int[k];
	}

	int max_freshness_value = rec_mf_bf(array, index_array, length, length, k,
			0, 0, path_index, selected_index, vary_func);

	//通过搜索路径矩阵和选择分支矩阵合并得出选择的路径
	for (unsigned int i = 0; i < path_num; i++) {
		//如果此路径上的分支不是要选择的则继续寻找下一条分支
		if (path_index[i][0] != selected_index[i][0])
			continue;
		//继续对比剩下的分支是否与选择的一致
		unsigned int j = 0;
		for (j = 0; j < k; j++) {
			if (path_index[i][j] != selected_index[i][j])
				break;
		}
		//此路径下的所有分支与选择的分支是一致的，此路径为选择的路径
		if (j == k) {
			for (unsigned int t = 0; t < k; t++)
				result[t] = array[path_index[i][t]];
		}
	}

	//清理资源
	for (unsigned int i = 0; i < path_num; i++) {
		delete[] path_index[i];
		delete[] selected_index[i];
	}
	delete[] path_index;
	delete[] selected_index;
	return max_freshness_value;
}

/*
 * description: 递归找出新鲜度最高的有序序列
 * 	  method: 自顶向下，没有优化过的暴力树搜索
 * 	  				 递归式：F(i)=max{F(i-1)+f(a[k])-v(k,i)}
 * 	  				 	 a[k]属于ALL_ELEMENT-S(i-1)
 * 	  				 	 F(i)表示第i次选择第k个元素时总新鲜度，f(a[k])表示第k个元素的新鲜度初始值，v(k,i)表示第k个元素在i次选择以后新鲜度的递减量，S(i)表示经过i次选择后的元素集合
 *  parameters: array: 	待选元素数组
 *  			   length:	数组长度
 *  			   result:  选择的序列
 *  			  		  k:	需要选择的序列长度
 *  			  		  i:  已选择的元素个数
 *  			  		  j:	当前递归节点在递归树同一层的偏移量
 *      return: 最高新鲜度(如果剩下的元素的新鲜度到了第k天已经小于零，默认返回剩下的第一个元素)
 */
template<typename ElementType, typename VaryFuncType>
int rec_mf_bab(ElementType array[], unsigned int index_array[],
		unsigned int length, unsigned int n, unsigned int k, unsigned int i,
		unsigned int j, int* current_mf, int* max_coe, unsigned int** path_index,
		unsigned int** selected_index, VaryFuncType vary_func) {
	int freshness = std::numeric_limits<int>::min();
	unsigned int index = 0;

	//搜索树的叶子节点的直接父节点
	if (k == 1) {
		for (unsigned int t = 0; t < length; t++) {
			int sub_freshness = vary_func(array + t, i);
			//找出剩下的元素中在最后一天里新鲜度最大的
			if (sub_freshness > freshness) {
				index = t;
				freshness = sub_freshness;
			}
		}

		//记录选择的元素
		path_index[j * arrange(n - i, k - 1)][i] = index_array[index];
		selected_index[j * arrange(n - i, k - 1)][i] = index_array[index];
		return freshness;
	}

	//搜索树其余节点
	for (unsigned int t = 0; t < length; t++) {
		swap(array, index_array, 0, t); //把选择的元素与第一个元素交换，然后在剩余的元素中递归进行选择，最后再交换回来
		int sub_freshness = rec_mf_bf(array + 1, index_array + 1, length - 1, n,
				k - 1, i + 1, j * (n - i) + t, path_index, selected_index,
				vary_func) + vary_func(array, i);
		swap(array, index_array, t, 0);
		if (sub_freshness > freshness) { //找出局部新鲜度最大的
			index = t;
			freshness = sub_freshness;
		}
	}

	//用分支限定法对不必要的分支进行剪枝


	//记录选择的节点
	for (unsigned int t = 0; t < length; t++) {
		for (unsigned int l = (j * (n - i) + t) * arrange(n - i - 1, k - 2);
				l < (j * (n - i) + t + 1) * arrange(n - i - 1, k - 2); l++) {
			path_index[l][i] = index_array[t];
		}
	}
	for (unsigned int t = j * arrange(n - i, k - 1);
			t < (j + 1) * arrange(n - i, k - 1); t++) {
		selected_index[t][i] = index_array[index];
	}
	return freshness;
}

/*
 * description: 找出新鲜度最高的有序序列
 * 	  method: 树搜索策略，爬山法
 *  parameters: array: 	待选元素数组
 *  			   length:	数组长度
 *  			   result:  选择的序列
 *  			  		  k:	需要选择的序列长度
 *      return: 最高新鲜度
 */
template<typename ElementType, typename VaryFuncType>
int max_freshness_bab(ElementType array[], const unsigned int length,
		ElementType result[], const unsigned int k, VaryFuncType vary_func) {
	//输入参数不合法
	if (k > length || k == 0)
		return 0;

	//由于在算法中需要临时交换元素的位置，所以需要与原元素数组对应的索引数组来辅助
	unsigned int* index_array = new unsigned int[length];
	for (unsigned int i = 0; i < length; i++) {
		index_array[i] = i;
	}

	//搜索路径总数
	unsigned int path_num = arrange(length, k - 1);
	//搜索路径
	unsigned int** path_index = new unsigned int*[path_num];
	//每次分支上作的选择，同属于一个父节点的分支选择值是相同的
	unsigned int** selected_index = new unsigned int*[path_num];
	for (unsigned int i = 0; i < path_num; i++) {
		path_index[i] = new unsigned int[k];
		selected_index[i] = new unsigned int[k];
	}

	int* current_mf = 0;
	int* max_coe = 0;
	int max_freshness_value = rec_mf_bab(array, index_array, length, length, k,
			0, 0, current_mf, max_coe, path_index, selected_index, vary_func);

	//通过搜索路径矩阵和选择分支矩阵合并得出选择的路径
	for (unsigned int i = 0; i < path_num; i++) {
		//如果此路径上的分支不是要选择的则继续寻找下一条分支
		if (path_index[i][0] != selected_index[i][0])
			continue;
		//继续对比剩下的分支是否与选择的一致
		unsigned int j = 0;
		for (j = 0; j < k; j++) {
			if (path_index[i][j] != selected_index[i][j])
				break;
		}
		//此路径下的所有分支与选择的分支是一致的，此路径为选择的路径
		if (j == k) {
			for (unsigned int t = 0; t < k; t++)
				result[t] = array[path_index[i][t]];
		}
	}

	//清理资源
	for (unsigned int i = 0; i < path_num; i++) {
		delete[] path_index[i];
		delete[] selected_index[i];
	}
	delete[] path_index;
	delete[] selected_index;
	return max_freshness_value;
}

#endif /* FRESHNESS_TMPLT_H_ */
