# 设置CMake的最低版本要求为3.16
cmake_minimum_required(VERSION 3.16)

# 定义一个变量QT_PROJECT_NAME，表示项目的名称
set(QT_PROJECT_NAME "PROJECT_NAME")

# 定义一个变量EXECUTABLE_NAME，表示生成的可执行文件的名称
set(EXECUTABLE_NAME "HelloWorld")

# 设置C语言的标准为C11
set(CMAKE_C_STANDARD 11)

# 设置C++语言的标准为C++17
set(CMAKE_CXX_STANDARD 17)

# 定义一个变量QT_COMPONENTS，表示需要链接的Qt组件，包括Core、Gui和Widgets
set(QT_COMPONENTS Core Gui Widgets)

# 定义一个变量OTHER_LIBS，表示其他需要链接的库（当前为空）
set(OTHER_LIBS)

# 定义一个变量QT_PROJECT_LOCALES，表示项目支持的 locales，包括中文和英文
set(QT_PROJECT_LOCALES "zh_CN" "en_US")

# 定义一个选项ENABLE_TEST，表示是否启用单元测试，默认关闭
option(ENABLE_TEST "Enable Unit Test" OFF)

# 使用之前定义的QT_PROJECT_NAME变量来初始化项目
project("${QT_PROJECT_NAME}")

# 递归地查找所有在CMake目录下的.cmake文件，并将它们的路径存储在变量MODULES中
file(GLOB_RECURSE MODULES "CMake/*.cmake")

# 遍历MODULES变量中的每一个.cmake文件，并包含它们
foreach(item IN LISTS MODULES)
    include(${item})
endforeach()

# 添加一个子目录Dependencies，用于处理项目的依赖项
add_subdirectory(Dependencies)

# 添加一个子目录Src，用于处理项目的源代码
add_subdirectory(Src)
