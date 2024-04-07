#include <cstdio>
#include <cstring>
#include <cstdint>

// 定义一个紧凑排列的结构体, 用于张量头部信息
struct __attribute__((packed)) tensor_header_t {
    unsigned int flag;   // 标志位
    unsigned int ndim;   // 维数
    unsigned int dtype;  // 类型
};

int main(){
    FILE* f = fopen("test.tensor", "rb");

    // 定义一个张量头部结构体，并从文件中读取头部信息
    tensor_header_t header{0};
    fread(&header, 1, sizeof(header), f);

    // 检查标志位，确保文件格式正确
    if(header.flag != 0xFCCFE2E2) {
        printf("Invalid format for 0x%08X\n", header.flag);
        fclose(f);
        return 1;
    }

    // 打印出头部信息
    printf("flag = 0x%08X, ndim = %d, dtype = %d\n", header.flag, header.ndim, header.dtype);

    // 定义一个数组, 用于存储张量的形状信息
    unsigned int shape[16];
    size_t volumn_size = 1;

    // 从中文中读取张量的形状信息
    fread(shape, 1, sizeof(int)*header.ndim, f);
    for(int i=0; i<header.ndim; ++i){
        volumn_size *= shape[i];  // 计算总的数据量
        printf("shape[%d] = %d\n", i, shape[i]);  // 打印每一维的大小
    }

    // 根据计算出的数据量分配内存，用于存储张量数据
    int64_t* pdata = new int64_t[volumn_size];

    // 从文件中读取张量数据
    fread(pdata, 1, sizeof(int64_t) *volumn_size, f);
    for(int i=0; i<volumn_size; ++i)
        printf("%ld ", pdata[i]);
    printf("\n");

    delete [] pdata;

    fclose(f);
    return 0;
}
