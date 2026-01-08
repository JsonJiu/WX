cmake_minimum_required(VERSION 3.16)

# ===== TOOLCHAIN CONFIGURATION =====
# 设置目标系统
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 跳过编译器测试 (解决交叉编译问题)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 设置编译器
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_NM arm-none-eabi-nm)

# 处理器特定设置 (FM33LG0xx 使用 Cortex-M0+ 核心)
set(CPU_FLAGS "-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft")

# 基础编译器标志
#set(CMAKE_C_FLAGS_INIT "${CPU_FLAGS} -ffunction-sections -fdata-sections")
#set(CMAKE_CXX_FLAGS_INIT "${CPU_FLAGS} -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions")
#set(CMAKE_ASM_FLAGS_INIT "${CPU_FLAGS}")
set(CMAKE_C_FLAGS "${CPU_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CPU_FLAGS} -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")

# 链接器标志
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CPU_FLAGS} -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections")
#set(CMAKE_EXE_LINKER_FLAGS_INIT "${CPU_FLAGS} -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections -lgcc -lc -lnosys")
#set(CMAKE_EXE_LINKER_FLAGS_INIT "${CPU_FLAGS} -specs=nano.specs -specs=rdimon.specs -Wl,--gc-sections")

# 搜索路径配置
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# ===== PROJECT DEFINITION =====
project(Sanchuan_NB_ValveCtrl
    VERSION 1.0.0
    DESCRIPTION "Sanchuan NB-IoT Valve Control Test Program for FM33LG0xx"
    LANGUAGES C CXX ASM
)

# Set executable suffix
set(CMAKE_EXECUTABLE_SUFFIX .elf)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Compiler feature requirements
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== PROJECT DIRECTORIES =====
set(DRIVERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Drivers)
set(CMSIS_DIR ${DRIVERS_DIR}/CMSIS/Device)
set(FM33_DIR ${DRIVERS_DIR}/FM33LG0xx_FL_Driver)
set(CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MF-config)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Inc)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src)

# Linker script path
set(LINKER_SCRIPT ${CMSIS_DIR}/FM/FM33xx/Source/Templates/gcc/linker/fm33lg04x_flash.ld)
if(NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "Linker script not found: ${LINKER_SCRIPT}")
endif()

# ===== EXECUTABLE CREATION =====
add_executable(${PROJECT_NAME})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMSIS_DIR}
    ${CMSIS_DIR}/FM/FM33xx/Include
    ${FM33_DIR}/Inc
    ${INC_DIR}
    ${CONFIG_DIR}/Inc
)

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    DEBUG
    USE_HAL_DRIVER
    FM33LG0XX
    FM33LG04X
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:Release>:NDEBUG=1>
)

# Compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
    # Common options for project code
    -Wall
    -Wextra
    -Wpedantic

    # 🔇 屏蔽特定的第三方代码警告
    -Wno-unused-parameter          # 屏蔽接口函数中的未使用参数
    -Wno-format-truncation         # 屏蔽格式化字符串截断警告 (INA219调试工具)
    -Wno-sign-compare              # 屏蔽snprintf返回值的符号比较警告
    -Wno-enum-conversion           # 屏蔽枚举类型转换警告
    -Wno-unused-but-set-variable   # 屏蔽厂商库中的 volatile 变量警告

    # 🚫 将厂商库目录设为系统头文件，彻底屏蔽警告
    "-isystem${CMAKE_CURRENT_SOURCE_DIR}/Drivers/FM33LG0xx_FL_Driver/Inc"
    "-isystem${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS"

    # Debug/Release specific
    $<$<CONFIG:Debug>:-Og -g3>
    $<$<CONFIG:Release>:-O2 -DNDEBUG>

    # C specific options
    $<$<COMPILE_LANGUAGE:C>:-std=gnu11>

    # C++ specific options
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++17>
)

# Linker options
target_link_options(${PROJECT_NAME} PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,--print-memory-usage
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--cref
    -Wl,--check-sections
    -Wl,--unresolved-symbols=report-all
    -Wl,--warn-common
)

# ===== SOURCE FILES =====
# CMSIS and System sources
set(CMSIS_SOURCES
    ${CMSIS_DIR}/FM/FM33xx/Source/Templates/gcc/startup_fm33lg0xx.s
    ${CMSIS_DIR}/FM/FM33xx/Source/system_fm33lg0xx.c
)

# FM33LG0xx Driver sources
file(GLOB FM33_DRIVER_SOURCES
    ${FM33_DIR}/Src/fm33lg0xx_fl*.c
)

# Application source files (automatically find all .c files)
file(GLOB_RECURSE APP_SOURCES_AUTO
    ${SRC_DIR}/*.c
    ${CONFIG_DIR}/Src/*.c
)

# Exclude certain files if needed (e.g., test files or disabled modules)
set(EXCLUDED_SOURCES
    # Add any files you want to exclude from compilation here
    # Example: ${SRC_DIR}/test_file.c
    # ${SRC_DIR}/uart4.c  # Temporarily exclude uart4.c due to compilation issues - FIXED and re-enabled
)

# Remove excluded files from the list
list(REMOVE_ITEM APP_SOURCES_AUTO ${EXCLUDED_SOURCES})

# Filter out non-existent files
set(APP_SOURCES)
foreach(source ${APP_SOURCES_AUTO})
    if(EXISTS ${source})
        list(APPEND APP_SOURCES ${source})
    endif()
endforeach()

# Print found source files for debugging
message(STATUS "Found application source files:")
foreach(source ${APP_SOURCES})
    get_filename_component(filename ${source} NAME)
    message(STATUS "  - ${filename}")
endforeach()

# Verify all source files exist
foreach(source ${APP_SOURCES})
    if(NOT EXISTS ${source})
        message(WARNING "Source file not found: ${source}")
    endif()
endforeach()

# Add all sources to target
target_sources(${PROJECT_NAME} PRIVATE
    ${CMSIS_SOURCES}
    ${FM33_DRIVER_SOURCES}
    ${APP_SOURCES}
)

# ===== DRIVER LIBRARY (OPTIONAL) =====
# Create static library for drivers
add_library(fm33_drivers STATIC ${FM33_DRIVER_SOURCES})
target_include_directories(fm33_drivers PUBLIC
    ${CMSIS_DIR}
    ${CMSIS_DIR}/FM/FM33xx/Include
    ${FM33_DIR}/Inc
)
target_compile_definitions(fm33_drivers PUBLIC USE_HAL_DRIVER FM33LG0XX FM33LG04X)

# 🚫 厂商库警告屏蔽配置 - v1.9.1优化
target_compile_options(fm33_drivers PRIVATE
    # 基础编译选项
    -ffunction-sections -fdata-sections
    $<$<CONFIG:Debug>:-Og -g3>
    $<$<CONFIG:Release>:-O2>

    # 🔇 屏蔽厂商驱动库的常见警告
    -Wno-unused-parameter          # 屏蔽未使用参数警告
    -Wno-unused-but-set-variable   # 屏蔽设置但未使用变量警告
    -Wno-unused-variable           # 屏蔽未使用变量警告
    -Wno-unused-function           # 屏蔽未使用函数警告
    -Wno-sign-compare              # 屏蔽符号比较警告
    -Wno-format-truncation         # 屏蔽格式化字符串截断警告
    -Wno-stringop-truncation       # 屏蔽字符串操作截断警告
    -Wno-missing-field-initializers # 屏蔽缺失字段初始化器警告
    -Wno-implicit-fallthrough      # 屏蔽隐式switch fallthrough警告
    -Wno-pedantic                  # 禁用严格的ISO C标准检查
)

# Link the driver library
target_link_libraries(${PROJECT_NAME} PRIVATE fm33_drivers)

# 🔇 为特定第三方文件设置警告屏蔽 (当前项目暂不需要)
# function(suppress_warnings_for_files file_list)
#     foreach(file ${file_list})
#         if(EXISTS ${file})
#             set_source_files_properties(${file} PROPERTIES
#                 COMPILE_FLAGS "-Wno-all -Wno-extra -Wno-format-truncation -Wno-unused-function -Wno-pedantic"
#             )
#             message(STATUS "Applied warning suppression to: ${file}")
#         endif()
#     endforeach()
# endfunction()

# 应用警告屏蔽到特定的第三方工具文件 (当前项目暂不需要)
# set(THIRD_PARTY_FILES
#     # Add third-party files here if needed
# )
# suppress_warnings_for_files("${THIRD_PARTY_FILES}")

# ===== POST-BUILD COMMANDS =====
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Generating HEX and BIN files, printing size information"
    VERBATIM
)

# ===== CUSTOM TARGETS =====
# Flash target
add_custom_target(flash
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/flash.sh ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex
    DEPENDS ${PROJECT_NAME}
    COMMENT "Flashing ${PROJECT_NAME} to target device"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Memory usage analysis
add_custom_target(memory_usage
    COMMAND ${CMAKE_NM} --print-size --size-sort --radix=d $<TARGET_FILE:${PROJECT_NAME}> > ${PROJECT_NAME}_memory.txt
    COMMAND echo "Memory usage analysis saved to ${PROJECT_NAME}_memory.txt"
    DEPENDS ${PROJECT_NAME}
    COMMENT "Analyzing memory usage"
)

# Clean all generated files
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Cleaning all build files"
)

# ===== INSTALLATION RULES =====
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map
    DESTINATION bin
    OPTIONAL
)

# ===== CONFIGURATION SUMMARY =====
message(STATUS "=== Build Configuration ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Linker script: ${LINKER_SCRIPT}")
message(STATUS "Source files found: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Build directory: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "===============================")
