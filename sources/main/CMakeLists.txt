add_subdirectory(src)
add_subdirectory(ut)
add_subdirectory(mt)

  ADD_CUSTOM_TARGET(Coverage

    # Run tests
    DEPENDS UnitTest_coverage
    DEPENDS ModuleTest_coverage

    # Capturing lcov counters and generating report
    COMMAND ${LCOV_PATH} -d ${PNS_MT_BINARY_DIR} -d ${PNS_UT_BINARY_DIR} -d ${PhoneticNumberSystem_BINARY_DIR} --capture --output-file coverage.info
    COMMAND ${LCOV_PATH} --remove coverage.info 'tests/*' '/usr/*' 'build/*' 'ut/*' 'mt/*' --output-file coverage.info.cleaned
    COMMAND ${GENHTML_PATH} -o ${CMAKE_BINARY_DIR}/coverage coverage.info.cleaned
    COMMAND ${CMAKE_COMMAND} -E remove coverage.info coverage.info.cleaned

    COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report for whole project."
  )

