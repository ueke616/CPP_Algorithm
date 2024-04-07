#include <iostream>
#include <cstdio>
using namespace std;

int main() {
    int yourNum = 6;
    int* yourNumPtr;

    yourNumPtr = &yourNum;
    printf("youNum [直接取值] is %d\n", yourNum);
    printf("yourNumPtr %p\n", yourNumPtr);
    printf("yourNum [通过地址去取值] is %d\n", *yourNumPtr);
    printf("the addr of yourNumPtr is %p\n", &yourNumPtr);

    return 0;
}
