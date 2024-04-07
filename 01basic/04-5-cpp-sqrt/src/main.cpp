#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

// 定义一个recoder类，用于记录（写入）浮点数到文件中
class recoder {
public:
    // 构造函数，接受一个文件路径和一个浮点数x作为参数
    // file_: 成员变量，用于存储文件路径
    // fh_: 成员变量，用于存储文件句柄
    recoder(const char* file, float x): file_(file) {
        fh_ = fopen(file, "wb"); // 以写入二进制模式打开文件，并初始化文件句柄fh_
        push(x); // 调用push函数，将传入的浮点数x写入文件
    }

    // 成员函数push，用于将一个浮点数x写入文件
    void push(float x) {
        fprintf(fh_, "%f\n", x); // 使用fprintf将浮点数x格式化后写入文件，每个数值后跟一个换行符
    }

    // 析构函数，用于关闭打开的文件，释放资源
    virtual ~recoder() {
        fclose(fh_); // 调用fclose关闭文件句柄fh_，避免资源泄露
    }

private:
    FILE* fh_ = nullptr; // 文件句柄，初始化为nullptr，用于操作文件
    const char* file_ = nullptr; // 文件路径，初始化为nullptr
};


// 梯度下降法, 函数式: f(t) = (t^2 - C)^2  平方根
float sqrt_gradient_descent(float x, bool print = true) {
    float t = x / 2.0f; // 初始化t为x的一半，作为平方根的初始猜测值
    float L = std::pow(t*t - x, 2.0f); // 计算损失函数L，表示当前猜测值的平方与实际值x的平方差的平方
    float eps = 1e-5; // 设置收敛阈值epsilon，用于判断算法是否收敛
    float alpha = 0.0001f; // 设置学习率alpha，控制每一步的迭代步长
    int iter_count = 0; // 迭代计数器，记录迭代次数
    recoder r("sqrt_gradient_descent", t); // 创建recoder对象r，用于记录每次迭代的t值

    // 循环直到损失函数L小于epsilon，表示找到了足够接近的平方根值
    while(L > eps){
        float derivative = 2 * (t*t - x) * 2 * t; // 计算损失函数L关于t的导数
        t = t - derivative * alpha; // 更新t值，根据梯度下降算法的公式
        L = std::pow(t*t - x, 2.0f); // 重新计算损失函数L

        // 如果print标志为true，则打印当前迭代的详细信息
        if(print){
            iter_count++; // 迭代计数器加1
            // 打印当前损失函数值L，当前猜测值t，本次迭代的步长，以及迭代次数
            printf("L = %f, t = %f, dt = %f, iter_count = %d\n", L, t, derivative * alpha, iter_count);
            r.push(t); // 调用recoder对象r的push方法，记录当前的t值
        }
    }
    return t; // 返回计算得到的平方根值
}

// 牛顿法, 求解 f'(t) = 0, 函数式: f(t) = (t^2 - C)^2
float sqrt_newton_method1(float x, bool print = true){
    float t = x / 2.0f;
    float L = std::pow(t*t-x, 2.0f);
    float eps = 1e-5f;
    int iter_count = 0;
    recoder r("sqrt_newton_method1", t);

    while(L > eps){
        float first_derivative = 2 * (t*t - x) * 2 * t;
        float second_derivative = 4 * t * 2 * t + 4 * (t*t - x);

        t = t - first_derivative / second_derivative;
        L = std::pow(t*t - x, 2.0f);

        if(print){
            iter_count++;
            printf("L=%f, t=%f, dt1=%f, dt2=%f, iter_count=%d\n", L, t, first_derivative, second_derivative, iter_count);
            r.push(t);
        }
    }
    return t;
}

// 牛顿法, 求解f(t) = 0, 函数式: f(t) = (t^2 - C)^2
float sqrt_newton_method2(float x, bool print=false){
    float t = x / 2.0f;
    float L = std::pow(t*t-x, 2.0f);
    float eps = 1e-5f;
    int iter_count = 0;
    recoder r("sqrt_newton_method2", t);

    while(L > eps) {
        float fx    = L;
        float first_derivative = 2 * (t*t - x)*2 * t;

        t = t - fx/first_derivative;
        L = std::pow(t*t-x, 2.0f);

        if(print){
            iter_count++;
            printf("L=%f, t=%f, fx=%f, dt=%f, iter_count=%d\n", L, t, fx, first_derivative, iter_count);
            r.push(t);
        }
    }
    return t;
}


// 牛顿法, 求解 f(t)=0, 函数式: f(t) = t^2 - C
float sqrt_newton_method3(float x, bool print=false){
    float t = x / 2.0f;
    float L = t*t - x;
    float eps = 1e-5f;
    int iter_count = 0;
    recoder r("sqrt_newton_method3", t);

    while(abs(L) > eps){
        float fx    = L;
        float first_derivative = 2*t;

        t = t - fx/first_derivative;
        L = t*t - x;

        if(print){
            iter_count++;
            printf("L=%f, t=%f, fx=%f, dt=%f, iter_count=%d\n", L, t, fx, first_derivative, iter_count);
            r.push(t);
        }
    }
    return t;
}


int main(){
    float x = 60;
    cout << "sqrt_gradient_descent(x): " << sqrt_gradient_descent(x) << endl;
    cout << "sqrt_newton_method1(x): " << sqrt_newton_method1(x) << endl;
    cout << "sqrt_newton_method2(x): " << sqrt_newton_method2(x) << endl;
    cout << "sqrt_newton_method3(x): " << sqrt_newton_method3(x) << endl;
}
