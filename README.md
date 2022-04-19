# OpenGL_Init_MultyThread

【OpenGL】OpenGL——初始化，mesh渲染，以及多线程调用相关。

实验中使用OpenGL进行初始化，Mesh加载和显示，使用VCG作为Mesh加载的库。

初步实现Mesh的多线程加载和显示，但没有进行多线程之间的安全监测。

单线程使用CMake作为编译工具，多线程使用QT；
