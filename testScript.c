#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n, min, max;
    char name[50];
    
    printf("输入数据个数: ");
    scanf("%d", &n);
    
    printf("输入最小值: ");
    scanf("%d", &min);
    
    printf("输入最大值: ");
    scanf("%d", &max);
    
    printf("输入文件名: ");
    scanf("%s", name);
    
    // 创建文件
    FILE *f = fopen(name, "w");
    
    // 写文件头
    fprintf(f, "#define SIZE %d\n\n", n);
    fprintf(f, "int data[SIZE] = {");
    
    srand(time(NULL));
    
    // 生成随机数
    for(int i = 0; i < n; i++) {
        int num = min + rand() % (max - min + 1);
        fprintf(f, "%d", num);
        if(i < n-1) {
            fprintf(f, ", ");
        }
    }
    
    fprintf(f, "};\n");
    fclose(f);
    
    printf("完成! 生成了 %d 个 %d 到 %d 的随机数到 %s\n", n, min, max, name);
    
    return 0;
}
