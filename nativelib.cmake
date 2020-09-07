add_library(nativelib STATIC
        "${CMAKE_CURRENT_LIST_DIR}/port/${NATIVELIB_PORT}/Inet.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/port/${NATIVELIB_PORT}/UdpSocket.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/port/${NATIVELIB_PORT}/TcpSocket.cpp")

target_include_directories(nativelib PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/")
target_include_directories(nativelib PUBLIC "${CMAKE_CURRENT_LIST_DIR}/port/${NATIVELIB_PORT}/")

target_link_libraries(nativelib crosslib)