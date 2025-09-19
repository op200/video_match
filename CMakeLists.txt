cmake_minimum_required(VERSION 4.1)

# 强制 64 位构建检查
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "32-bit builds are not supported. Please use 64-bit toolchain.")
endif()

# 设置静态链接目标
set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "强制使用静态库版本")

# Vcpkg 工具链自动配置
if(NOT CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(vcpkg_toolchain "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        if(EXISTS "${vcpkg_toolchain}")
            set(CMAKE_TOOLCHAIN_FILE "${vcpkg_toolchain}"
                CACHE STRING "Vcpkg toolchain file")
            message(STATUS "Using vcpkg from: $ENV{VCPKG_ROOT}")
        else()
            message(WARNING "Vcpkg toolchain file not found at: ${vcpkg_toolchain}")
        endif()
    else()
        message(STATUS "VCPKG_ROOT not set, skipping vcpkg toolchain setup")
    endif()
else()
    message(STATUS "Using custom toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# 优先使用 Ninja 生成器
find_program(NINJA_EXECUTABLE NAMES ninja)
if(NINJA_EXECUTABLE)
    set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "Use Ninja if available")
endif()

project(video_match
    VERSION 1.0
    LANGUAGES CXX
    DESCRIPTION "Video matching application"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 全局静态链接配置
set(BUILD_SHARED_LIBS OFF)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 查找 FFmpeg 组件（静态链接版本）
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFmpeg REQUIRED IMPORTED_TARGET
    libavcodec libavformat libavutil libswscale libavfilter)

# 使用更安全的文件收集方式
set(SOURCES_DIR src)
file(GLOB SOURCES CONFIGURE_DEPENDS "${SOURCES_DIR}/*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "${SOURCES_DIR}/*.h")

# 添加可执行目标
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 包含头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE
    "${SOURCES_DIR}"
    "${FFmpeg_INCLUDE_DIRS}"
)

# 链接 FFmpeg 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    PkgConfig::FFmpeg
)

# 添加 Windows 系统依赖
if(MSVC)
    # 修改链接器选项，添加必要的静态库
    target_link_options(${PROJECT_NAME} PRIVATE
        /SUBSYSTEM:CONSOLE
        /NODEFAULTLIB:MSVCRT
        /NODEFAULTLIB:LIBCMT
        /NODEFAULTLIB:LIBCMTD
    )
    
    # 添加静态运行时库
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries(${PROJECT_NAME} PRIVATE
            libcmtd.lib
            libvcruntimed.lib
            libucrtd.lib
        )
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE
            libcmt.lib
            libvcruntime.lib
            libucrt.lib
        )
    endif()
endif()

if(MSVC)
    # 修复重复符号问题
    target_link_options(${PROJECT_NAME} PRIVATE
        /NODEFAULTLIB:MSVCRT
        /SUBSYSTEM:CONSOLE
        # /DYNAMICBASE:NO  # 禁用ASLR以便完全静态
        # /WHOLEARCHIVE  # 确保所有静态库内容被包含
        # /FORCE:MULTIPLE  # 允许重复符号
    )
    
    # 添加静态链接器标志
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
endif()

# 添加静态链接的编译器定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    FFMPEG_STATIC
    STATIC_LINKING
)

# 禁用自动复制DLL（vcpkg特性）
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
endif()

# 添加安装目标
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)