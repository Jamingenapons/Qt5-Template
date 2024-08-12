# 使用之前定义的EXECUTABLE_NAME变量来初始化项目
project("${EXECUTABLE_NAME}")

# 启用CMake的自动MOC、RCC和UIC功能
set(CMAKE_AUTOMOC ON)  # 启用自动MOC（Meta-Object Compiler）处理
set(CMAKE_AUTORCC ON)  # 启用自动RCC（Resource Compiler）处理
set(CMAKE_AUTOUIC ON)  # 启用自动UIC（User Interface Compiler）处理

# 定义项目目录结构
set(MAIN_DIR "${PROJECT_SOURCE_DIR}")  # 设置主目录为项目源目录
set(TEST_DIR "${PROJECT_SOURCE_DIR}/Test")  # 设置测试目录
set(RESOURCES_DIR "${PROJECT_SOURCE_DIR}/../Resources")  # 设置资源目录
set(QRC_DIR "${RESOURCES_DIR}/Files")  # 设置资源文件目录
set(TRANSLATIONS_DIR "${RESOURCES_DIR}/Translations")  # 设置翻译文件目录
set(UTILS_DIR "${PROJECT_SOURCE_DIR}/Utils")  # 设置工具目录

# 查找并配置Qt5组件
find_package(Qt5 COMPONENTS REQUIRED ${QT_COMPONENTS} LinguistTools)  # 查找并配置所需的Qt5组件
if (Qt5Core_FOUND)  # 检查Qt5Core是否找到
    message(STATUS "Qt5Core_INCLUDE_DIRS: ${Qt5Core_INCLUDE_DIRS}")  # 输出Qt5Core的包含目录
    message(STATUS "Qt5Core_LIBRARIES: ${Qt5Core_LIBRARIES}")  # 输出Qt5Core的库文件
else()
    message(FATAL_ERROR "Qt5Core was not found.")  # 如果Qt5Core未找到，则输出致命错误
endif()
list_add_prefix(QT_LIBS "Qt5::" ${QT_COMPONENTS})  # 为Qt库添加前缀

# 扫描本地化翻译文件
set(LOCALE_PREFIX "${PROJECT_NAME}")  # 设置本地化前缀
list(LENGTH QT_PROJECT_LOCALES localeNums)  # 获取本地化列表的长度
if(localeNums LESS_EQUAL 0)  # 如果本地化列表为空
    list(APPEND QT_PROJECT_LOCALES "en_US")  # 添加默认的`en_US`
endif()
foreach(locale IN LISTS QT_PROJECT_LOCALES)  # 遍历本地化列表
    list(APPEND LOCALIZATION_FILES "${TRANSLATIONS_DIR}/${LOCALE_PREFIX}_${locale}.ts")  # 添加本地化文件
    list(APPEND quoted_locale_names "\"${locale}\"")  # 添加带引号的本地化名
endforeach()
# 把所有 Locale 名转成 C 数组
list(JOIN quoted_locale_names ", " LOCALES_NAME_ARRAY)  # 将本地化名列表转换为C数组

# 自定义 Target: 本地化
qt5_create_translation(QM_FILES ${PROJECT_SOURCE_DIR} ${LOCALIZATION_FILES})  # 创建本地化文件
add_custom_target(UpdateTranslation ALL DEPENDS ${QM_FILES})  # 添加自定义目标以更新本地化文件

# 解析 *.qrc 资源文件
file(GLOB_RECURSE QRC_FILES "${QRC_DIR}/*.qrc")  # 递归查找所有资源文件
foreach(item IN LISTS QRC_FILES)  # 遍历资源文件列表
    get_filename_component(ext_name ${item} EXT)  # 获取文件扩展名
    if(ext_name MATCHES ".*\\.big\\.qrc$")  # 如果是大资源文件
        list(APPEND BIG_QRC_FILES ${item})  # 添加到大资源文件列表
    else()
        list(APPEND SMALL_QRC_FILES ${item})  # 添加到小资源文件列表
    endif()
endforeach()
qt5_add_resources(RESOURCES_FILES ${SMALL_QRC_FILES})  # 添加小资源文件
# qt5_add_big_resources(BIG_RESOURCES_FILE ${BIG_QRC_FILES})  # 添加大资源文件（注释掉）

# Target: 主程序
list_add_prefix(SRC_EXPRESSIONS "${MAIN_DIR}/" "*.ui" "*.c" "*.cpp" "*.hpp" "*.cc")  # 添加源文件表达式
file(GLOB_RECURSE MAIN_SOURCES LIST_DIRECTORIES true ${SRC_EXPRESSIONS})  # 递归查找所有源文件
list(APPEND MAIN_SOURCES "${MAIN_DIR}")  # 添加主目录
foreach (item IN LISTS MAIN_SOURCES)  # 遍历源文件列表
    if (IS_DIRECTORY ${item})  # 如果是目录
        list(APPEND MAIN_INCLUDE_DIR ${item})  # 添加到包含目录列表
        continue()
    endif()
    list(APPEND MAIN_SRC ${item})  # 添加到源文件列表
endforeach()

add_library(MainLibrary STATIC ${MAIN_SRC} ${LOCALIZATION_FILES})  # 添加静态库
target_include_directories(MainLibrary PUBLIC ${MAIN_INCLUDE_DIR})  # 设置包含目录
target_link_libraries(MainLibrary PUBLIC ${QT_LIBS} ${OTHER_LIBS})  # 链接库
target_compile_definitions(MainLibrary PUBLIC -DLOCALE_PREFIX="${LOCALE_PREFIX}")  # 设置编译定义
target_compile_definitions(MainLibrary PUBLIC -DLOCALES_NAME_ARRAY=${LOCALES_NAME_ARRAY})  # 设置编译定义

# Target: 主程序入口
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")  # 如果是Windows且构建类型为Release
    add_executable(${PROJECT_NAME} WIN32 EntryPoint.cpp ${RESOURCES_FILES} ${BIG_RESOURCES_FILE})  # 添加可执行文件
else()
    add_executable(${PROJECT_NAME} EntryPoint.cpp ${RESOURCES_FILES} ${BIG_RESOURCES_FILE})  # 添加可执行文件
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC MainLibrary)  # 链接库

# TODO 自动化测试
if(ENABLE_TEST)  # 如果启用测试
    add_subdirectory(${TEST_DIR})  # 添加测试目录
endif()

#add_subdirectory(core)  # 添加核心目录（注释掉）
#add_subdirectory(gui)  # 添加GUI目录（注释掉）
#add_subdirectory(services)  # 添加服务目录（注释掉）
add_subdirectory(utils)  # 添加工具目录

# 确保程序定义后添加依赖关系
copy_qt_platform_plugins("${CMAKE_BINARY_DIR}/Src")  # 复制Qt平台插件
