# golden_fliter
###### 在pynq平台上，经由FPGA加速任意视频的怀旧色滤镜添加

在FPGA系列实验的第二实验中，我们学习了如何通过FPGA加速灰度图下sobel卷积运算。

在本次实验中，我们尝试使用卷积核对图像上各像素点在颜色空间上实现卷积，并通过FPGA进行加速。 具体公式如下：

$R = 0.393r+0.769g+0.189b (2-1)$

$G = 0.349r+0.686g+0.168b (2-2)$

$B = 0.272r+0.534g+0.131b (2-3)$

##### 原图
![image](https://github.com/xyc1719/golden_fliter/blob/main/img/raw.png)
##### 效果图
![image](https://github.com/xyc1719/golden_fliter/blob/main/img/after.png)

------

### Main Problem
在项目进行过程中，我们小组将FPGA的加速程序和python自带的卷积函数对比，发现加速效果并不明显。

在C synthesis的报告文件中，我们最终发现是由于视频文件的I/O次数过多，导致程序I/O时间过长，进一步导致在综合时产生的RTL程序并行程度不高。

### Solution
新的程序中，我们扩大了axis的带宽，从8bit拓宽到64bit，速度也相较于python函数有了明显的优势。

（由于cv2中卷积库实际调用的是C/Cpp函数，所以可以认为解决了I/O开销问题之后的程序相较于C程序有了明显的优势）

同时我们也获悉了FPGA和pynq上的一些参数和特性：
```
1.pynq-z2上不存在图形界面，所以调用某些图形界面上的函数会导致内核挂掉。
2.pynq-z2上内存是采用小端法存储的。
3.DMA上axis提供的最大数据流带宽是1024 bit，但是由于机子的限制，实际最高只能达到64bit。
```
