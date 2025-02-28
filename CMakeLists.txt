cmake_minimum_required(VERSION 3.8)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Устанавливаем отладочные флаги
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")

# Включение горячей перезагрузки для MSVC
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Подключение nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)

# Подключение Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# Добавляем путь к заголовочным файлам
set(SEARCH_ENGINE_LIB ${CMAKE_SOURCE_DIR}/include)

# Подключаем поддиректории src и tests
add_subdirectory(src)
add_subdirectory(tests)

# Создаём исполняемый файл
add_executable(search_engine main.cpp)

# Линкуем с библиотеками
target_link_libraries(search_engine PRIVATE search_engine_lib nlohmann_json::nlohmann_json gtest_main)

# Добавляем тесты Google Test
include(GoogleTest)
gtest_discover_tests(search_engine)

# Устанавливаем C++20 для новых версий CMake
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET search_engine PROPERTY CXX_STANDARD 20)
endif()
