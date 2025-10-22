// sort_parallel.c - 并行归并排序
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "testScript.h"  // 包含生成的数据

// 合并函数
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    
    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    free(L); free(R);
}

// 并行归并排序
void parallel_merge_sort(int arr[], int l, int r, int depth) {
    if (l < r) {
        int m = l + (r - l) / 2;
        
        // 控制并行深度，避免创建过多线程
        if (depth < 4) {  // 只在前4层并行
            #pragma omp parallel sections
            {
                #pragma omp section
                parallel_merge_sort(arr, l, m, depth + 1);
                
                #pragma omp section  
                parallel_merge_sort(arr, m + 1, r, depth + 1);
            }
        } else {
            // 深度太大就串行
            parallel_merge_sort(arr, l, m, depth + 1);
            parallel_merge_sort(arr, m + 1, r, depth + 1);
        }
        
        merge(arr, l, m, r);
    }
}

// 串行归并排序（用于对比）
void serial_merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        serial_merge_sort(arr, l, m);
        serial_merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main() {
    // 复制数据，避免修改原始数据
    int *arr = malloc(n * sizeof(int));
    for(int i = 0; i < n; i++) arr[i] = data[i];
    
    printf("数据量: %d\n", n);
    
    // 测试并行版本
    double start = omp_get_wtime();
    parallel_merge_sort(arr, 0, n - 1, 0);
    double parallel_time = omp_get_wtime() - start;
    
    printf("并行时间: %.3f 秒\n", parallel_time);
    
    // 重新复制数据测试串行版本
    for(int i = 0; i < n; i++) arr[i] = data[i];
    
    start = omp_get_wtime();
    serial_merge_sort(arr, 0, n - 1);
    double serial_time = omp_get_wtime() - start;
    
    printf("串行时间: %.3f 秒\n", serial_time);
    printf("加速比: %.2f 倍\n", serial_time / parallel_time);
    
    free(arr);
    return 0;
}
