#include <stdio.h>

#include <vector>

#include "avl.h"

using vi = std::vector<int>;

int main() {
  // clang-format off
  // 3          5        5         5            5              5
  //  5    =>  3 7   => 3 7  =>  3   7   =>  3     7    =>   3    7
  //   7                   9      4   9       4   6 9      2  4  6 9
  // clang-format on
  Avl::NodeVisitor visitor = [](const AvlNode *p) {
    printf("%d ", p->value);
  };
  vi v = {3, 5, 7, 9, 4, 6, 2, 8, 10, 11, 12, 13};
  Avl avl;
  printf("INSERTING...\n");
  for (int a : v) {
    avl.Insert(a);
    printf("INSERT(%d) => ", a);
    avl.Preorder(visitor);
    printf("\n");
  }
  printf("INSERT DONE.\n");
  vi t = {8, 6, 5, 3, 2, 9, 4, 1, 7};
  printf("SEARCHING...\n");
  for (int a : t) printf("Search(%d) => %d\n", a, avl.Search(a));
  printf("SEARCH DONE.\n");
  printf("DELETING...\n");
  for (int a : t) {
    avl.Delete(a);
    printf("DELETE(%d) => ", a);
    avl.Preorder(visitor);
    printf("\n");
  }
  printf("DELETE DONE.\n");
  avl.Preorder(visitor);
  printf("\n");
  return 0;
}