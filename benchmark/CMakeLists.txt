set(BENCHMARK_COMMON_INCLUDES
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

set(BENCHMARK_COMMON_COMPILE_OPTIONS
    -O3
    -march=native
    $<$<CXX_COMPILER_ID:GNU>:-fno-omit-frame-pointer>
)

set(BENCHMARK_COMMON_LIBS
    benchmark::benchmark
    benchmark::benchmark_main
)

set(BENCHMARK_SOURCES
    smart_ptr_bench.cpp
)

foreach(bench_source ${BENCHMARK_SOURCES})
    get_filename_component(bench_name ${bench_source} NAME_WE)
    
    add_executable(${bench_name} ${bench_source})
    
    target_include_directories(${bench_name} PRIVATE ${BENCHMARK_COMMON_INCLUDES})
    target_link_libraries(${bench_name} PRIVATE ${BENCHMARK_COMMON_LIBS})
    target_compile_options(${bench_name} PRIVATE ${BENCHMARK_COMMON_COMPILE_OPTIONS})
    
    set_target_properties(${bench_name} PROPERTIES
        FOLDER "Benchmarks"
    )
endforeach()

add_custom_target(run_all_benchmarks
    COMMENT "Running all benchmarks..."
)

foreach(bench_source ${BENCHMARK_SOURCES})
    get_filename_component(bench_name ${bench_source} NAME_WE)
    add_custom_command(
        TARGET run_all_benchmarks
        POST_BUILD
        COMMAND ${bench_name}
        COMMENT "Running ${bench_name}"
    )
endforeach()