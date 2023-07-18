改为使用64bit带宽进行I/O后，每次经过处理可以得到连续的八个uint8_t类型。
使用#pragma HLS unroll factor=4 展开循环后，一个循环处理32个像素。
ii=12,平均每个像素点需要3/8个ii，是8bit下平均ii的1/8，运行效率提升八倍。
或与8bit下latancy比较，也得到运行效率提升8倍的结论。
