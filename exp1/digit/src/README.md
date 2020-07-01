# README.md

digit_astar.cpp是A\*搜索的程序

digit_idastar.cpp是IDA\*搜索的程序

文件的读取写入地址通过参数进行定义，只需修改其中值即可选择不同的输入输出文件。

```c
#define INPUT_PATH "../input/2.txt"
#define OUTPUT_PATH "../output/2.txt"
```

输入输出同时会输出移位过程，可通过修改get_result()函数使其不在终端输出.