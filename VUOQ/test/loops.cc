void foo(float *a, float *b, float *c, int n) {
  for (int i = 0; i != n; ++i)
    c[i] = a[i] * b[i];
}

// int foo(float *a, float *b, float *c, int n) {
//     // // Level 1
//     // for (int i = 0; i < n; ++i) {
//     //     a[i] = b[i] + c[i];
//     // }
//     // // Level 2
//     // for (int i = 0; i < n; ++i) {
//     //     for (int j = 0; j < n; ++j) {
//     //         a[i] = b[j] + c[i];
//     //     }
//     // }
//     // // Level 3
//     // for (int i = 0; i < n; ++i) {
//     //     for (int j = 0; j < n; ++j) {
//     //         for (int k = 0; k < n; ++k) {
//     //             a[i] = b[j] + c[k];
//     //         }
//     //     }
//     // }
//     // // Perfect nest, not normalized, iteration space is not a rectangular polyhedron
//     // for (int i = 0; i < n; ++i) {
//     //     for (int j = i; j < n; ++j) {
//     //         for (int k = i; k < n; ++k) {
//     //             a[i] = b[j] + c[k];
//     //         }
//     //     }
//     // }
//     // // Not normalized, iteration space is a rectangular polyhedron
//     // for (int i = 1; i < n; ++i) {
//     //     a[i] = b[i] + c[i];
//     // }
//     // // Perfect nest, normalized, iteration space is a rectangular polyhedron
//     // for (int i = 0; i < n; ++i) {
//     //     for (int j = 0; j < n; ++j) {
//     //         a[i] = b[j] + c[i];
//     //     }
//     // }
//     // Not a perfect nest, normalized, iteration space is a rectangular polyhedron
//     for (int i = 0; i < n; ++i) {
//         if (a[i] < 7) {
//             break;
//         }
//         for (int j = 0; j < n; ++j) {
//             a[j] = b[j] + c[j];
//         }
//     }
//     return a[n - 1];
// }
