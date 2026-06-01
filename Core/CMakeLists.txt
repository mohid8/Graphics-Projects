add_library(Core STATIC
    src/GMath.cpp
    include/GMath.hpp
)

target_include_directories(Core PUBLIC include)

add_library(GMath::core ALIAS Core)