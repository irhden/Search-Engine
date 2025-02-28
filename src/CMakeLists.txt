cmake_minimum_required(VERSION 3.8)

set(SEARCH_ENGINE_LIB ${CMAKE_SOURCE_DIR}/include)

# Создаём библиотеку search_engine_lib
add_library(search_engine_lib STATIC
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)

# Добавляем include-директории
target_include_directories(search_engine_lib PUBLIC
    ${SEARCH_ENGINE_LIB}
)

# Линкуем с JSON
target_link_libraries(search_engine_lib PRIVATE nlohmann_json::nlohmann_json)
