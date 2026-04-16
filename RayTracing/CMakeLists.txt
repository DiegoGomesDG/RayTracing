add_executable(RayTracing)

file(GLOB_RECURSE RT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(RayTracing PRIVATE ${RT_SOURCES})

target_include_directories(RayTracing
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(RayTracing
        PRIVATE Core imgui
)