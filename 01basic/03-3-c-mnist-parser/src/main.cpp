#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

// 使用packed属性确保结构体没有任何填充，直接对应MNIST数据格式
struct __attribute__((packed)) mnist_labels_header_t {
    unsigned int magic_number;       // 文件的魔数（用于校验文件类型）
    unsigned int number_of_items;    // 文件中标签的数量
};

struct __attribute__((packed)) mnist_images_header_t {
    unsigned int magic_number;       // 文件的魔数
    unsigned int number_of_images;   // 图像数量
    unsigned int number_of_rows;     // 每个图像的行数
    unsigned int number_of_columns;  // 每个图像的列数
};

// 用于转换MNIST文件中使用的大端字节序为当前系统使用的字节序
unsigned int inverse_byte(unsigned int v) {
    unsigned char* p = (unsigned char*)&v;
    std::swap(p[0], p[3]);  // 交换字节0和字节3
    std::swap(p[1], p[2]);  // 交换字节1和字节2
    return v;
}

int main() {
    // 尝试打开标签文件
    FILE* f = fopen("../mnist/train-labels-idx1-ubyte", "rb");
    if (f == NULL) {
        perror("Error opening file: train-labels.idx1-ubyte");
        return -1; // 文件打开失败，输出错误信息并退出程序
    }

    // 读取并输出标签文件头信息
    mnist_labels_header_t labels_header{0};
    fread(&labels_header, 1, sizeof(labels_header), f);
    printf("labels_header.magic_number = %X, number_of_items = %d\n", 
        inverse_byte(labels_header.magic_number), inverse_byte(labels_header.number_of_items));

    // 读取第一个标签并输出
    unsigned char label = 0;
    fread(&label, 1, sizeof(label), f);
    printf("First label is: %d\n", label);
    fclose(f);  // 关闭标签文件

    // 尝试打开图像文件
    f = fopen("../mnist/train-images-idx3-ubyte", "rb");
    if (f == NULL) {
        perror("Error opening file: train-images.idx3-ubyte");
        return -1; // 文件打开失败，输出错误信息并退出程序
    }

    // 读取并输出图像文件头信息
    mnist_images_header_t images_header{0};
    fread(&images_header, 1, sizeof(images_header), f);
    printf("images_header.magic_number = %X, number_of_images = %d, number_of_rows = %d, number_of_columns = %d\n",
        inverse_byte(images_header.magic_number),
        inverse_byte(images_header.number_of_images),
        inverse_byte(images_header.number_of_rows),
        inverse_byte(images_header.number_of_columns)
    );

    // 读取第一张图像数据并输出
    std::vector<unsigned char> image(inverse_byte(images_header.number_of_rows) * inverse_byte(images_header.number_of_columns));
    fread(image.data(), 1, image.size(), f);
    for(int i = 0; i < image.size(); ++i){
        // 以文本形式打印图像，未标记的像素打印为"----"
        if(image[i] == 0)
            printf("----");
        else
            printf("%03d ", image[i]);

        // 每行结束时换行
        if((i + 1) % inverse_byte(images_header.number_of_columns) == 0)
            printf("\n");
    }
    fclose(f);  // 关闭图像文件
    return 0;
}
