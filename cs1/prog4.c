#include<stdio.h>

int main() {
  printf("The type of cpu is:\n==========================================\n");
  system("cat /proc/cpuinfo");
  printf("\nThe model is:\n============================================\n");
  system("cat /proc/version");
  printf("\nThe memory info is:\n==========================================\n");
  system("cat /proc/meminfo");
  return 0;
}
