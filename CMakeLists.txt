cmake_minimum_required(VERSION 3.10)
project(ExtractFrames)

# 查找OpenCV
find_package(OpenCV REQUIRED)

# 添加可执行文件
add_executable(extract_frames extract_frames.cpp)

# 链接OpenCV库
target_link_libraries(extract_frames ${OpenCV_LIBS})

# 设置C++17标准
set_target_properties(extract_frames PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)
