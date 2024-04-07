
#include <stdio.h>
#include <string.h>

// 未使用packed属性的zip头部结构定义，可能会因内存对齐而占用更多空间
struct zip_header_nopacked_t{
    unsigned int flag;                // 文件头标志，用于识别ZIP文件（固定值0x04034b50）
    unsigned short version;           // 创建ZIP文件的程序版本
    unsigned short type;              // 加密类型和其他标志
    unsigned short mode;              // 文件模式和权限（UNIX系统）
    unsigned short last_modify_time;  // 最后修改时间
    unsigned short last_modify_date;  // 最后修改日期
    unsigned int crc32;               // 文件内容的CRC32校验码
    unsigned int compress_size;       // 压缩后的文件大小
    unsigned int raw_size;            // 原始文件大小
    unsigned short name_size;         // 文件名长度
    unsigned short extra_size;        // 额外字段的长度
};

// 使用packed属性的zip头部结构定义，确保成员紧密排列，避免内存对齐引入的额外空间
struct __attribute__((packed)) zip_header_t{
    unsigned int flag;                // 文件头标志，用于识别ZIP文件（固定值0x04034b50）
    unsigned short version;           // 创建ZIP文件的程序版本
    unsigned short type;              // 加密类型和其他标志
    unsigned short mode;              // 文件模式和权限（UNIX系统）
    unsigned short last_modify_time;  // 最后修改时间，使用DOS时间格式
    unsigned short last_modify_date;  // 最后修改日期，使用DOS日期格式
    unsigned int crc32;               // 文件内容的CRC32校验码
    unsigned int compress_size;       // 压缩后的文件大小
    unsigned int raw_size;            // 原始文件大小
    unsigned short name_size;         // 文件名长度，即文件名所占的字节数
    unsigned short extra_size;        // 额外字段的长度，ZIP格式允许包含一些额外的信息
};

int main(){

    // 输出两种结构体的大小，以观察内存对齐的影响
    printf("sizeof(zip_header_nopacked_t) = %d\n", sizeof(struct zip_header_nopacked_t));
    printf("sizeof(zip_header_t) = %d\n", sizeof(struct zip_header_t));

    // 尝试打开ZIP文件
    FILE* f = fopen("vscode-plugin-demo-master.zip", "rb");
    if(f == NULL){ // C标准中，nullptr应为NULL
        printf("Open failed.\n");
        return -1;
    }

    // 循环读取文件直到文件结束
    while(!feof(f)){

        // 初始化zip头部结构
        struct zip_header_t header = {0};
        fread(&header, 1, sizeof(header), f);

        // 检查文件头标志，确认是否为ZIP文件
        if(header.flag != 0x04034b50){
            printf("Flag is %X\n", header.flag);
            break; // 如果标志不匹配，则退出循环
        }
        
        // 根据头部信息中的文件名长度读取文件名
        char name[512] = {0};
        fread(name, 1, header.name_size, f);

        // 根据压缩大小和额外字段大小跳过文件内容，准备读取下一个文件头
        fseek(f, header.compress_size + header.extra_size, SEEK_CUR);

        // 输出文件信息，包括文件名、压缩大小、原始大小和压缩比
        printf("File: %s, Size: %d byte / %d byte,  Ratio: %.2f %%.\n", 
            name, header.compress_size, header.raw_size, 
            header.raw_size != 0 ? header.compress_size / (float)header.raw_size * 100 : 0
        );
    }

    // 关闭文件
    fclose(f);
    return 0;
}