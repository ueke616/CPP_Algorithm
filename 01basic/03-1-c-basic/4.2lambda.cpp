#include <cstdio>
using namespace std;

int main() {
    int c = 2;
    // [capture](parameters)->return-type{body}
    auto add = [=](int x, int y){return x + y + c;};

    int sum = add(1, 8);
    printf("sum = %d\n", sum);
    return 0;
}
