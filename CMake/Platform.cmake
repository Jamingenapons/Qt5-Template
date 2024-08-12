# 在你的 CMakeLists.txt 文件中添加以下代码
function(copy_qt_platform_plugins target_dir)  # 定义复制Qt平台插件的函数
    # 获取 Qt 安装目录
    get_filename_component(QT_PLUGINS_DIR "${Qt5_DIR}/../../../plugins" ABSOLUTE)  # 获取Qt插件目录

    # 指定要复制的平台插件目录
    set(QT_PLATFORMS_DIR "${QT_PLUGINS_DIR}/platforms")  # 设置平台插件目录

    # 确保目标目录存在
    file(MAKE_DIRECTORY ${target_dir})  # 创建目标目录

    # 复制平台插件
    file(COPY ${QT_PLATFORMS_DIR} DESTINATION ${target_dir})  # 复制平台插件

    # 添加自定义目标以在构建时触发插件复制
    add_custom_target(copy-plugins  # 添加自定义目标
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${QT_PLATFORMS_DIR} ${target_dir}  # 复制命令
        DEPENDS ${target_dir}  # 依赖目标目录
    )

    # 使主目标依赖于自定义目标，确保在构建之前插件已经被复制
    add_dependencies(${EXECUTABLE_NAME} copy-plugins)  # 添加依赖关系
endfunction()