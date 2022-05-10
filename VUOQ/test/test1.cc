// Depending on:
// 1. trip count
// 2. loop depth
// 3. data types of the array operations
// ... empirically (runtime) determine the optimal Vectorization and Unroll Factors and compare with what LLVM does

#include <iostream>


#define N 1e2


using namespace std;


// template <typename T>
// void foo(T *A, T *B, T *C, int n) {
//   #pragma clang loop vectorize_width(2) interleave_count(2)
//   for (T i = 0; i != N; ++i)
//     C[i] = i;
// }

void bar(int *A, int *B, int K, int start, int end) {
  for (int i = start; i < end; ++i)
    A[i] *= B[i] + K;
}


// #define WITH_MAIN
#ifdef WITH_MAIN
int main() {
  int *A = new int[N];
  int *B = new int[N];
  bar(A, B, N, 0, N);

//   uint16_t *A_uint16_t = new uint16_t[N];
//   uint16_t *B_uint16_t = new uint16_t[N];
//   uint16_t *C_uint16_t = new uint16_t[N];
//   foo<uint16_t>(A_uint16_t, B_uint16_t, C_uint16_t, N);
}
#endif
