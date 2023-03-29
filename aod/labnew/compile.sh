clang -c -Weverything -Wno-vla -ggdb3 ./mygraph.c -o mygraph.o
ar rcs libmyfunctions.a mygraph.o
clang -c -Weverything -Wno-vla -ggdb3 ./fifo.c -o fifo.o
ar rcs libfifo.a fifo.o
clang -c -Weverything -Wno-vla -ggdb3 ./mystack.c -o mystack.o
ar rcs libmystack.a mystack.o
clang -c -Weverything -Wno-vla -ggdb3 ./quick.c -o quick.o
ar rcs libsort.a quick.o
clang -Weverything -Wno-vla -ggdb3 ./dfs.c -L. -lmyfunctions -lmystack -lfifo -lsort -o mydfs.out
