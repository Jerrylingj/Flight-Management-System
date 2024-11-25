cmake_minimum_required(VERSION 3.16)

project(FlightManagementSystem VERSION 0.1 LANGUAGES CXX)

# 添加前端和后端两个子目录
add_subdirectory(backend)
add_subdirectory(frontend)
