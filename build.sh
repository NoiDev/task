rm -rf build
mkdir build
clang task.c -o build/task -O0 -g
chmod +x build/task
