### svg编辑器介绍
这个简单的编辑器主要是用来熟悉Qt的用法，实现较为简陋。
实现功能：
1. 直线，自由线，圆形，矩形，文字的编辑。
2. 支持通用的svg文件的读（但是不支持编辑）。
3. 支持保存为svg文件。
4. 支持读取保存的svg文件。

### 开发工具介绍
cmake 3.0
Qt 5.9.3
vs 2019

### 构建步骤
1. 在github上clone代码。[github传送门](https://github.com/b1ywww/svgEditor)

2. clone下来的代码目录如下![image.png](/upload/2021/12/image-b8390b29ae19462699909f02f0f6e66e.png)

在当前目录新建一个debug文件夹
进去执行命令
```cmake
cmake ../
```
即可构建成功。
注意 Qt需要配置环境变量。

在使用vs打开项目之后还需要如下设置
"链接器-系统-子系统"中选择Windows  
"链接器-高级-入口点"设置为mainCRTStartup
