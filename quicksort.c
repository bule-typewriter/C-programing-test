#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 简单数据生成
void gen_data(int n, int min, int max, char* name) {
    FILE *f = fopen(name, "w");
    fprintf(f, "int n = %d;\n", n);
    fprintf(f, "int data[] = {");
    
    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        int num = min + rand() % (max - min + 1);
        fprintf(f, "%d", num);
        if(i < n-1) fprintf(f, ", ");
    }
    fprintf(f, "};\n");
    fclose(f);
    printf("生成数据: %s\n", name);
}

// 交换函数
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// 随机pivot分区
int part_rand(int arr[], int l, int r) {
    int p = l + rand() % (r - l + 1);
    swap(&arr[p], &arr[r]);
    int x = arr[r];
    int i = l - 1;
    
    for(int j = l; j < r; j++) {
        if(arr[j] <= x) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[r]);
    return i+1;
}

// 三数取中分区
int part_mid(int arr[], int l, int r) {
    int m = l + (r - l) / 2;
    
    // 三数排序
    if(arr[l] > arr[m]) swap(&arr[l], &arr[m]);
    if(arr[l] > arr[r]) swap(&arr[l], &arr[r]);
    if(arr[m] > arr[r]) swap(&arr[m], &arr[r]);
    
    swap(&arr[m], &arr[r]); // 把中位数放到右边
    int x = arr[r];
    int i = l - 1;
    
    for(int j = l; j < r; j++) {
        if(arr[j] <= x) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[r]);
    return i+1;
}

// 1. 递归 + 随机pivot
void quick_rec_rand(int arr[], int l, int r) {
    if(l < r) {
        int p = part_rand(arr, l, r);
        quick_rec_rand(arr, l, p-1);
        quick_rec_rand(arr, p+1, r);
    }
}

// 2. 递归 + 三数取中
void quick_rec_mid(int arr[], int l, int r) {
    if(l < r) {
        int p = part_mid(arr, l, r);
        quick_rec_mid(arr, l, p-1);
        quick_rec_mid(arr, p+1, r);
    }
}

// 3. 非递归 + 随机pivot
void quick_iter_rand(int arr[], int n) {
    int *stack = malloc(n * 2 * sizeof(int));
    int top = -1;
    
    stack[++top] = 0;
    stack[++top] = n-1;
    
    while(top >= 0) {
        int r = stack[top--];
        int l = stack[top--];
        
        int p = part_rand(arr, l, r);
        
        if(p-1 > l) {
            stack[++top] = l;
            stack[++top] = p-1;
        }
        
        if(p+1 < r) {
            stack[++top] = p+1;
            stack[++top] = r;
        }
    }
    free(stack);
}

// 4. 非递归 + 三数取中
void quick_iter_mid(int arr[], int n) {
    int *stack = malloc(n * 2 * sizeof(int));
    int top = -1;
    
    stack[++top] = 0;
    stack[++top] = n-1;
    
    while(top >= 0) {
        int r = stack[top--];
        int l = stack[top--];
        
        int p = part_mid(arr, l, r);
        
        if(p-1 > l) {
            stack[++top] = l;
            stack[++top] = p-1;
        }
        
        if(p+1 < r) {
            stack[++top] = p+1;
            stack[++top] = r;
        }
    }
    free(stack);
}

// 检查排序是否正确
int check_sort(int arr[], int n) {
    for(int i = 1; i < n; i++) {
        if(arr[i] < arr[i-1]) return 0;
    }
    return 1;
}

// 复制数组
void copy_arr(int from[], int to[], int n) {
    for(int i = 0; i < n; i++) to[i] = from[i];
}

// 时间测试函数
void test_time(int data[], int n) {
    int *temp = malloc(n * sizeof(int));
    
    // 测试1: 递归 + 随机pivot
    copy_arr(data, temp, n);
    clock_t start = clock();
    quick_rec_rand(temp, 0, n-1);
    clock_t end = clock();
    double time1 = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
    printf("递归+随机: %.2f ms (%s)\n", time1, check_sort(temp, n) ? "正确" : "错误");
    
    // 测试2: 递归 + 三数取中
    copy_arr(data, temp, n);
    start = clock();
    quick_rec_mid(temp, 0, n-1);
    end = clock();
    double time2 = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
    printf("递归+三数: %.2f ms (%s)\n", time2, check_sort(temp, n) ? "正确" : "错误");
    
    // 测试3: 非递归 + 随机pivot
    copy_arr(data, temp, n);
    start = clock();
    quick_iter_rand(temp, n);
    end = clock();
    double time3 = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
    printf("非递归+随机: %.2f ms (%s)\n", time3, check_sort(temp, n) ? "正确" : "错误");
    
    // 测试4: 非递归 + 三数取中
    copy_arr(data, temp, n);
    start = clock();
    quick_iter_mid(temp, n);
    end = clock();
    double time4 = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
    printf("非递归+三数: %.2f ms (%s)\n", time4, check_sort(temp, n) ? "正确" : "错误");
    
    free(temp);
}

// 从文件读取数据的函数
int read_data_from_file(char* filename, int** data_ptr) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }
    
    int n;
    // 读取 n 的值
    if (fscanf(file, "int n = %d;", &n) != 1) {
        printf("文件格式错误: 无法读取 n\n");
        fclose(file);
        return 0;
    }
    
    // 分配内存
    *data_ptr = malloc(n * sizeof(int));
    
    // 跳过 "int data[] = {"
    char buffer[100];
    fgets(buffer, sizeof(buffer), file); // 读取剩余部分
    
    // 读取数组数据
    for (int i = 0; i < n; i++) {
        int value;
        if (fscanf(file, "%d,", &value) != 1) {
            printf("文件格式错误: 无法读取第 %d 个数据\n", i);
            free(*data_ptr);
            fclose(file);
            return 0;
        }
        (*data_ptr)[i] = value;
    }
    
    fclose(file);
    return n;
}

int main() {
    srand(time(NULL));
    
    int choice;
    printf("1. 生成测试数据\n");
    printf("2. 运行性能测试\n");
    printf("选择: ");
    scanf("%d", &choice);
    
    if(choice == 1) {
        int n, min, max;
        char name[50];
        
        printf("数据个数: ");
        scanf("%d", &n);
        printf("最小值: ");
        scanf("%d", &min);
        printf("最大值: ");
        scanf("%d", &max);
        printf("文件名: ");
        scanf("%s", name);
        
        gen_data(n, min, max, name);
        
    } else if(choice == 2) {
        char name[50];
        printf("数据文件名: ");
        scanf("%s", name);
        
        int* data = NULL;
        int n = read_data_from_file(name, &data);
        
        if (n > 0 && data != NULL) {
            printf("数据量: %d\n", n);
            printf("=== 性能测试结果 ===\n");
            
            // 多次测试取平均
            int tests = 3;
            for(int t = 1; t <= tests; t++) {
                printf("\n测试 %d:\n", t);
                test_time(data, n);
            }
            
            free(data);
        } else {
            printf("读取数据失败!\n");
        }
    }
    
    return 0;
}
