#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateTestData(int dataSize, int minValue, int maxValue, const char* filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Cannot create file %s\n", filename);
        return;
    }
    
    // 写入头文件保护
    fprintf(file, "#ifndef TEST_DATA_H\n");
    fprintf(file, "#define TEST_DATA_H\n\n");
    
    // 写入数组大小定义
    fprintf(file, "#define DATA_SIZE %d\n\n", dataSize);
    
    // 写入测试数据数组
    fprintf(file, "int test_data[DATA_SIZE] = {\n");
    
    srand(time(NULL)); // 设置随机种子
    
    for (int i = 0; i < dataSize; i++) {
        // 生成指定范围内的随机整数
        int value = minValue + rand() % (maxValue - minValue + 1);
        fprintf(file, "    %d", value);
        
        if (i < dataSize - 1) {
            fprintf(file, ",");
        }
        
        // 每10个数字换一行
        if ((i + 1) % 10 == 0) {
            fprintf(file, "\n");
        }
    }
    
    fprintf(file, "\n};\n\n");
    fprintf(file, "#endif // TEST_DATA_H\n");
    
    fclose(file);
    printf("Test data generated successfully!\n");
    printf("Data size: %d, Range: [%d, %d]\n", dataSize, minValue, maxValue);
    printf("Output file: %s\n", filename);
}

int main() {
    int dataSize, minValue, maxValue;
    char filename[100];
    
    printf("=== Test Data Generator ===\n");
    
    // 获取用户输入
    printf("Enter data size: ");
    scanf("%d", &dataSize);
    
    printf("Enter minimum value: ");
    scanf("%d", &minValue);
    
    printf("Enter maximum value: ");
    scanf("%d", &maxValue);
    
    printf("Enter output filename (without extension): ");
    scanf("%s", filename);
    
    // 添加.h扩展名
    char fullFilename[110];
    snprintf(fullFilename, sizeof(fullFilename), "%s.h", filename);
    
    // 验证输入有效性
    if (dataSize <= 0) {
        printf("Error: Data size must be positive\n");
        return 1;
    }
    
    if (minValue > maxValue) {
        printf("Error: Minimum value cannot be greater than maximum value\n");
        return 1;
    }
    
    // 生成测试数据
    generateTestData(dataSize, minValue, maxValue, fullFilename);
    
    return 0;
}
