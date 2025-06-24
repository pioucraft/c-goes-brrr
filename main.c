#include <stdio.h> 

int main() {
    double first = 0.000000001;
    double second = 0.000000002;
    double sum = first + second;
    printf("%.50lf\n", second);
    return 0;
}
