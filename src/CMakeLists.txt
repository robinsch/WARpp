if(WIN32)
  set(sources_windows
    ${CMAKE_SOURCE_DIR}/src/common/Debugging/WheatyExceptionReport.cpp
    ${CMAKE_SOURCE_DIR}/src/common/Debugging/WheatyExceptionReport.h
  )
endif(WIN32)

add_subdirectory(genrev)
add_subdirectory(common)

if(SERVERS)
  add_subdirectory(server)
endif(SERVERS)
