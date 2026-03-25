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
project(NB_IOT_Gas_Meter_Board_18
    VERSION 1.1.0
    DESCRIPTION "NB_GAS_METER_TESTER for FM33LG0xx"
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

# ===== BOOTLOADER SUPPORT =====
#
# 配置方式（二选一）：
#   1. 修改 Inc/app_config.h 中的 #define USE_BOOTLOADER（推荐）
#   2. CMake 命令行: cmake -DUSE_BOOTLOADER=ON ...（命令行优先级更高）
#
# 从 app_config.h 读取配置
file(READ ${INC_DIR}/app_config.h APP_CONFIG_CONTENT)
string(FIND "${APP_CONFIG_CONTENT}" "#define USE_BOOTLOADER" USE_BOOTLOADER_POS)
# 检查是否被注释（查找 "// #define USE_BOOTLOADER"）
string(FIND "${APP_CONFIG_CONTENT}" "// #define USE_BOOTLOADER" USE_BOOTLOADER_COMMENTED_POS)

# 如果命令行没有指定，则从 app_config.h 读取
if(NOT DEFINED USE_BOOTLOADER OR USE_BOOTLOADER STREQUAL "")
    if(USE_BOOTLOADER_POS GREATER -1 AND USE_BOOTLOADER_COMMENTED_POS EQUAL -1)
        set(USE_BOOTLOADER ON)
        message(STATUS "USE_BOOTLOADER detected in app_config.h")
    else()
        set(USE_BOOTLOADER OFF)
    endif()
endif()

# Linker script path - 根据 USE_BOOTLOADER 选项选择
if(USE_BOOTLOADER)
    set(LINKER_SCRIPT ${CMSIS_DIR}/FM/FM33xx/Source/Templates/gcc/linker/fm33lg04x_app_with_bootloader.ld)
    message(STATUS "")
    message(STATUS "╔════════════════════════════════════════════════════════╗")
    message(STATUS "║  🚀 BOOTLOADER MODE: APP starts at 0x4000 (240KB)      ║")
    message(STATUS "║     配置位置: Inc/app_config.h                         ║")
    message(STATUS "╚════════════════════════════════════════════════════════╝")
    message(STATUS "")
else()
    set(LINKER_SCRIPT ${CMSIS_DIR}/FM/FM33xx/Source/Templates/gcc/linker/fm33lg04x_flash.ld)
    message(STATUS "")
    message(STATUS "╔════════════════════════════════════════════════════════╗")
    message(STATUS "║  📦 STANDALONE MODE: APP starts at 0x0000 (256KB)      ║")
    message(STATUS "║     配置位置: Inc/app_config.h                         ║")
    message(STATUS "╚════════════════════════════════════════════════════════╝")
    message(STATUS "")
endif()

if(NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "Linker script not found: ${LINKER_SCRIPT}")
endif()

# ===== FORCE REBUILD MAIN.C FOR TIMESTAMP UPDATE =====
# 强制每次构建时重新编译 main.c，确保 __DATE__ 和 __TIME__ 宏自动更新
# 方法: 在构建前 touch main.c 使其时间戳更新
add_custom_target(touch_main_for_timestamp ALL
    COMMAND ${CMAKE_COMMAND} -E touch_nocreate ${SRC_DIR}/main.c
    COMMENT "Touching main.c to update build timestamp (__DATE__ / __TIME__)"
)

# ===== EXECUTABLE CREATION =====
add_executable(${PROJECT_NAME})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMSIS_DIR}
    ${CMSIS_DIR}/FM/FM33xx/Include
    ${FM33_DIR}/Inc
    ${INC_DIR}
    ${INC_DIR}/Peripheral/uart
    ${CONFIG_DIR}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/ValveCtrl
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/TimeManager
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/LedIndicator
    # Protocol framework includes - 使用Components作为根目录,支持 #include "Protocol/xxx.h"
    ${CMAKE_CURRENT_SOURCE_DIR}/Components
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/PC
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/Device
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/Device/Diomestic/DiaphragmGasMeters
    # Utility includes
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Utility
    # EasyLogger includes
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/EasyLogger
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/EasyLogger/easylogger/inc
    # FlashDB includes
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/port/fal/inc
    # Test framework includes - 支持 #include "Test/xxx.h"
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Test
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Test/NB_18_DiaphragmGas_Test
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Test/Domestic_water_meter_Test
)

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    DEBUG
    USE_HAL_DRIVER
    FM33LG0XX
    # Bootloader 模式宏定义，可在代码中使用 #ifdef USE_BOOTLOADER 判断
    $<$<BOOL:${USE_BOOTLOADER}>:USE_BOOTLOADER=1>
    $<$<BOOL:${USE_BOOTLOADER}>:APP_START_ADDRESS=0x4000>
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:Release>:NDEBUG=1>
)

# Compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
    # Common options
    -Wall
    -Wextra
    # -Wpedantic                    # 禁用：与 GCC 扩展（如 __FUNCTION__）冲突
    -Wno-unused-parameter           # 屏蔽"unused parameter"警告
    -Wno-unused-but-set-variable    # 屏蔽"unused but set variable"警告
    -Wno-unused-variable            # 屏蔽"unused variable"警告

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

# Application sources - Auto-scan all .c files
# Application source files (automatically find all .c files)
# 注意: 只包含需要的目录，不使用递归搜索 Components 根目录
file(GLOB APP_SOURCES_AUTO
    ${SRC_DIR}/*.c
    ${SRC_DIR}/Peripheral/uart/*.c
    ${SRC_DIR}/Test/*.c
    ${SRC_DIR}/Test/NB_18_DiaphragmGas_Test/*.c
    ${SRC_DIR}/Test/Domestic_water_meter_Test/*.c
    ${CONFIG_DIR}/Src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/ValveCtrl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/TimeManager/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/LedIndicator/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/PC/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/PC/Domestic/WaterMeters/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/PC/Domestic/DiaphragmGasMeters/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/Device/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/Device/Diomestic/DiaphragmGasMeters/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/Device/Diomestic/WaterMeters/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Utility/*.c
)

# EasyLogger sources (only core files, port file is in Src/)
set(EASYLOGGER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/EasyLogger/easylogger/src/elog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/EasyLogger/easylogger/src/elog_utils.c
)

# FlashDB sources
set(FLASHDB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/src/fdb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/src/fdb_kvdb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/src/fdb_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/port/fal/src/fal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/port/fal/src/fal_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/port/fal/src/fal_partition.c
    # FM33LG04x Flash port
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/fal_flash_fm33lg04_port.c
    # Flash diagnostics and test stats
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/flash_diag.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/FlashDB/test_stats.c
)

# Exclude certain files if needed (e.g., test files or disabled modules)
set(EXCLUDED_SOURCES
    # PC_xieyi_Ctrl.c 已移至 Src/backup/ (被 Protocol 模块替代)
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/backup/PC_xieyi_Ctrl.c
    # Legacy适配层不再需要
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Protocol/PC/pc_protocol_legacy.c
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
    ${EASYLOGGER_SOURCES}
    ${FLASHDB_SOURCES}
)

# ===== DRIVER LIBRARY (OPTIONAL) =====
# Create static library for drivers
add_library(fm33_drivers STATIC ${FM33_DRIVER_SOURCES})
target_include_directories(fm33_drivers PUBLIC
    ${CMSIS_DIR}
    ${CMSIS_DIR}/FM/FM33xx/Include
    ${FM33_DIR}/Inc
)
target_compile_definitions(fm33_drivers PUBLIC USE_HAL_DRIVER)
target_compile_options(fm33_drivers PRIVATE
    -Wall -Wextra -ffunction-sections -fdata-sections
    # 屏蔽官方固件库的常见警告
    -Wno-unused-parameter           # 屏蔽"unused parameter"警告
    -Wno-unused-but-set-variable    # 屏蔽"unused but set variable"警告
    -Wno-unused-variable            # 屏蔽"unused variable"警告
    -Wno-unused-function            # 屏蔽"unused function"警告
    -Wno-missing-prototypes         # 屏蔽"missing prototypes"警告
    -Wno-missing-declarations       # 屏蔽"missing declarations"警告
    -Wno-sign-compare               # 屏蔽"sign compare"警告
    -Wno-type-limits                # 屏蔽"type limits"警告
    -Wno-cast-function-type         # 屏蔽"cast function type"警告
    -Wno-implicit-fallthrough       # 屏蔽"implicit fallthrough"警告
    -Wno-strict-aliasing            # 屏蔽"strict aliasing"警告
    -Wno-maybe-uninitialized       # 屏蔽"maybe uninitialized"警告
    -Wno-array-bounds               # 屏蔽"array bounds"警告
    -Wno-format                     # 屏蔽"format"警告
    -Wno-format-truncation          # 屏蔽"format truncation"警告
    -Wno-format-overflow            # 屏蔽"format overflow"警告
    -Wno-stringop-truncation        # 屏蔽"stringop truncation"警告
    -Wno-stringop-overflow          # 屏蔽"stringop overflow"警告
    -Wno-switch-default             # 屏蔽"switch default"警告
    -Wno-switch-enum                # 屏蔽"switch enum"警告
    -Wno-uninitialized              # 屏蔽"uninitialized"警告
    -Wno-aggressive-loop-optimizations  # 屏蔽"aggressive loop optimizations"警告
    $<$<CONFIG:Debug>:-Og -g3>
    $<$<CONFIG:Release>:-O2>
)

# Link the driver library
target_link_libraries(${PROJECT_NAME} PRIVATE fm33_drivers)

# Ensure main.c is touched before compiling (for timestamp update)
add_dependencies(${PROJECT_NAME} touch_main_for_timestamp)

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
