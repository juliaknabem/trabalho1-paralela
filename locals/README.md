Como compilar:


ARQUIVOS COM OPENMP:
gcc -fopenmp parallel_bucket_sort_openmp.c randomlist.c -o openmp 


ARQUIVOS COM MULTITHREAD:
gcc -pthread parallel_bucket_sort_posix.c randomlist.c -o openmp 

ARQUIVOS SEQUENCIAIS:
gcc bubble_sort.c randomlist.c -o bubble 
