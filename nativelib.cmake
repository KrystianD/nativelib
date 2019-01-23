get_filename_component(NATIVELIB_CUR_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

include_directories("${NATIVELIB_CUR_DIR}/include")
include_directories("${NATIVELIB_CUR_DIR}/port/${NATIVELIB_PORT}")

# defines: NATIVELIB_NO_STL

set(COMMON_SOURCES ${COMMON_SOURCES}
        "${NATIVELIB_CUR_DIR}/port/${NATIVELIB_PORT}/Inet.cpp"
        "${NATIVELIB_CUR_DIR}/port/${NATIVELIB_PORT}/UdpSocket.cpp"
        "${NATIVELIB_CUR_DIR}/port/${NATIVELIB_PORT}/TcpSocket.cpp"
        )
