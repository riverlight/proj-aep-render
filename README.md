# proj-aep-render
opengl 的一些简单封装，目标是用一个简单的类封装实现各种滤镜、特效、转场、位置变换等处理

v0.07		20200212
1. 现在支持 alpha 通道的融合了


v0.06a		20200204
1. 优化了合成类代码

v0.06		20200123
1. 完成了合成类的开发，现在支持旋转、平移和缩放了
2. 把特效类和合成类做了个融合的demo

v0.05		20200122
1. 新增 layer 和 合成类，为位置变换做准备

v0.04		20200119
1. 特效类现在支持转场shader 了

v0.03		20200119
1. 现在支持颜色表滤波了

v0.02b		20200116
1. 修复了几个bug，另新增灵魂出窍 shader

v0.02a		20200114
1. 把 vertex shader 的 in 变量属性改成了 attribute

v0.02		20200114
1. 增加了一个时间控制参数

v0.01		20200113
1. 初始版本，仅完成了 CAEPFilter 特效类
