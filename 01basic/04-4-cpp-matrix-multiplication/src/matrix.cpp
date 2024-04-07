#include <vector>
#include <iostream>
#include <iomanip>
#include "openblas/cblas.h"
#include "openblas/lapacke.h"
#include "matrix.hpp"

Matrix::Matrix() {}  //  默认构造函数

// 构造函数, 使用初始化列表来初始化矩阵
Matrix::Matrix(int rows, int cols, const std::initializer_list<float>& pdata) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->data_ = data;

    // 若提供的数据少于矩阵大小,则扩展data_数组填充额外的空间
    if(this->data_.size() < rows * cols)
        this->data_.resize(rows * cols);
}

// 实现矩阵乘法(GEMM: Gerneral Matrix Multiply) 操作
Matrix Matrix::gemm(const Matrix& other, bool at, bool bt, float alpha, float beta){
    return ::gemm(*this, at, other, bt, alpha, beta);
}

// 创建当前矩阵的新试图，具有不同的行和列数但共享相同的数据
Matrix Matrix::view(int rows, int cols) const {
    // 确保新的维度与原始数据大小匹配
    if(rows * cols != this->rows_ * this->cols_){
        printf("Invalid view to %d x %d\n", rows, cols);
        return Matrix();
    }
    Matrix newmat = *this;
    newmat.rows_ = rows;  // 更新行数
    newmat.cols_ = cols;  // 更新列数
    return newmat;
}

// 重载减法运算符，实现矩阵之间的逐元素减法
Matrix Matrix::operator-(const Matrix& other) const{
    Matrix output = *this;
    auto p0 = output.ptr(); // 指向当前矩阵数据的指针
    auto p1 = other.ptr(); // 指向另一个矩阵数据的指针
    for(int i = 0; i < output.data_.size(); ++i)
        *p0++ -= *p1++; // 逐元素减法
    return output;
}

// 对矩阵的每个元素应用幂运算
Matrix Matrix::power(float y) const{
    Matrix output = *this;
    auto p0 = output.ptr(); // 指向当前矩阵数据的指针
    for(int i = 0; i < output.data_.size(); ++i, ++p0)
        *p0 = std::pow(*p0, y); // 对每个元素应用幂运算
    return output;
}

// 计算矩阵元素的累加和
float Matrix::reduce_sum() const{
    auto p0 = this->ptr(); // 指向当前矩阵数据的指针
    float output = 0;
    for(int i = 0; i < this->data_.size(); ++i)
        output += *p0++; // 累加元素值
    return output;
}

// 计算矩阵的逆
Matrix Matrix::inv(){
    return ::inverse(*this);
}

// 重载乘法运算符，实现矩阵与标量的乘法
Matrix Matrix::operator*(float value){
    Matrix m = *this;
    for(int i = 0; i < data_.size(); ++i)
        m.data_[i] *= value; // 每个元素乘以标量
    return m;
}

// 重载输出运算符，以便于打印矩阵
std::ostream& operator << (std::ostream& out, const Matrix& m){
    for(int i = 0; i < m.rows(); ++i){
        for(int j = 0; j < m.cols(); ++j){
            out << m(i, j) << "\t"; // 打印每个元素
        }
        out << "\n";
    }
    return out;
}

// 实现矩阵乘法，可选是否对操作数进行转置
Matrix gemm(const Matrix& a, bool ta, const Matrix& b, bool tb, float alpha, float beta){
    // 根据是否转置调整维度
    int m = ta ? a.cols() : a.rows();
    int n = tb ? b.rows() : b.cols();
    int k = ta ? a.rows() : a.cols();
    // 调用OpenBLAS库函数进行实际的GEMM操作
    Matrix c(m, n); // 结果矩阵
    cblas_sgemm(
        CblasRowMajor, ta ? CblasTrans : CblasNoTrans, tb ? CblasTrans : CblasNoTrans,
        m, n, k, alpha, a.ptr(), a.cols(), b.ptr(), b.cols(), beta, c.ptr(), c.cols()
    );
    return c;
}

// 计算矩阵的逆，使用LAPACK库函数
Matrix inverse(const Matrix& a){
    if(a.rows() != a.cols()){
        printf("Invalid to compute inverse matrix by %d x %d\n", a.rows(), a.cols());
        return Matrix();
    }

    Matrix output = a;
    int n = a.rows();
    int *ipiv = new int[n]; // 索引数组，用于LU分解

    // 执行LU分解
    int code = LAPACKE_sgetrf(LAPACK_COL_MAJOR, n, n, output.ptr(), n, ipiv);
    if(code == 0){
        // 使用LU分解结果计算逆矩阵
        code = LAPACKE_sgetri(LAPACK_COL_MAJOR, n, output.ptr(), n, ipiv);
    }

    if(code != 0){
        printf("LAPACKE inverse matrix failed, code = %d\n", code);
        return Matrix();
    }

    delete[] ipiv; // 释放资源
    return output;
}
