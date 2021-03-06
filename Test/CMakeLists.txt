set(Sources
	bulk_memory_ops.wast
	exceptions.wast
	misc.wast
	reference_types.wast
	simd.wast
	threads.wast
	trunc_sat.wast
	wavm_atomic.wast)
add_custom_target(WAVMTests SOURCES ${Sources})
set_target_properties(WAVMTests PROPERTIES FOLDER Testing)

set(TEST_BIN ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIGURATION}/RunTestScript)

if(ENABLE_RUNTIME)
	add_test(bulk_memory_ops ${TEST_BIN} ${CMAKE_CURRENT_LIST_DIR}/bulk_memory_ops.wast)
	add_test(exceptions ${TEST_BIN} ${CMAKE_CURRENT_LIST_DIR}/exceptions.wast)
	add_test(misc ${TEST_BIN} ${CMAKE_CURRENT_LIST_DIR}/misc.wast)
	add_test(reference_types ${TEST_BIN} ${CMAKE_CURRENT_LIST_DIR}/reference_types.wast --enable-reference-types)
	add_test(simd ${TEST_BIN} ${CMAKE_CURRENT_LIST_DIR}/simd.wast)
	add_test(threads ${TEST_BIN} ${CMAKE_CURRENT_LIST_DIR}/threads.wast)
	add_test(trunc_sat ${TEST_BIN} ${CMAKE_CURRENT_LIST_DIR}/trunc_sat.wast)
	add_test(wavm_atomic ${TEST_BIN} ${CMAKE_CURRENT_LIST_DIR}/wavm_atomic.wast)
endif()

add_subdirectory(Containers)
add_subdirectory(DumpTestModules)
add_subdirectory(fuzz)
add_subdirectory(RunTestScript)
add_subdirectory(spec)
