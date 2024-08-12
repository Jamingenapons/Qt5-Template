function(list_add_prefix OUTPUT_LIST PREFIX)  # 定义添加前缀的函数
    foreach (item IN ITEMS ${ARGN})  # 遍历列表
        list(APPEND RET "${PREFIX}${item}")  # 添加前缀
    endforeach()
    set(${OUTPUT_LIST} ${RET} PARENT_SCOPE)  # 设置输出列表
endfunction()