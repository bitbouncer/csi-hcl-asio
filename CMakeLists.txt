cmake_minimum_required(VERSION 2.8.11)

SET(BOOST_PATH ${CMAKE_SOURCE_DIR}/../boost)
SET(Boost_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/../boost)
SET(Boost_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/../boost/stage/lib)

SET(CSI_INCLUDE_PATH 
${CMAKE_SOURCE_DIR}/../boost
${CMAKE_SOURCE_DIR}
)

if(WIN32)

SET(CSI_INCLUDE_PATH ${CSI_INCLUDE_PATH}
	${CMAKE_SOURCE_DIR}/../curl/include
	${CMAKE_SOURCE_DIR}/../openssl/inc32
  )
SET(CSI_LIBRARY_PATH        
	${CMAKE_SOURCE_DIR}/../lib/$(Platform)
  ${CMAKE_SOURCE_DIR}/../boost/stage/lib/$(Platform)/lib
	${CMAKE_SOURCE_DIR}/../openssl/out32
)

SET(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/bin/$(Platform))
else()

#SET(CSI_LIBRARY_PATH        ${CMAKE_SOURCE_DIR}/boost/stage/lib
#set(LIBRARY_OUTPUT_PATH     ${CMAKE_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/bin)

endif() 

#COMMON DEFINITIONS
add_definitions(-D_FILE_OFFSET_BITS=64 -D_REENTRANT -DEXTERNAL_LOCKS -DMULTITHREAD)
add_definitions(-DBOOST_ASIO_HAS_STD_CHRONO)
add_definitions(-DBOOST_ALL_STATIC_LINK)

if(WIN32)
add_definitions(-DMARCH_x86_64) 
add_definitions(-D_WIN32_WINNT=0x0602) # win8.1 to get nthll 64 bit
add_definitions(-DWINVER=0x0601)
add_definitions(-DCF_WINDOWS)
add_definitions(/MP)
add_definitions(/bigobj)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-DSECURITY_WIN32)
add_definitions(/wd4996) #'strdup': The POSIX name for this item is deprecated
add_definitions(/wd4197) # 'cf_atomic32' : top-level volatile in cast is ignored
add_definitions(/wd4200) #nonstandard extension used : zero-sized array in struct/union 
add_definitions(-DCURL_STATICLIB)
add_definitions(-DCARES_STATICLIB)
add_definitions(-DNGHTTP2_STATICLIB)
endif()

if(__LINUX__)
add_definitions(-D__LINUX__)
add_definitions(-fPIC)
#if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8.0")
add_definitions(-std=c++11)
#endif()
add_definitions(-DBOOST_LOG_USE_NATIVE_SYSLOG)
add_definitions(-Wno-deprecated)
endif()

if(__DARWIN__)
add_definitions(-DCURL_STATICLIB)
add_definitions(-D__DARWIN__)
add_definitions(-fPIC)
add_definitions(-std=c++11)
include_directories(/opt/X11/include)
endif()

if(WIN32)
SET(CSI_BOOST_LIBS Ws2_32)
SET(CSI_HTTP_CLIENT_LIBS libcurl nghttp2 ssleay32 libeay32 ${CSI_BOOST_LIBS})
endif()

if(__LINUX__)
set(CSI_BOOST_LIBS ${BOOST_PATH}/stage/lib/libboost_log_setup.a ${BOOST_PATH}/stage/lib/libboost_log.a boost_date_time boost_timer boost_thread boost_system boost_program_options boost_filesystem boost_regex boost_chrono boost_iostreams pthread rt c)
SET(CSI_HTTP_CLIENT_LIBS curl crypto ssl ${CSI_BOOST_LIBS})
endif()

#most likely wrong since I don't have a mac anymore...
if(__DARWIN__)
set(CSI_BOOST_LIBS boost_log_setup boost_log boost_date_time boost_timer boost_thread boost_system boost_program_options boost_filesystem boost_regex boost_chrono boost_iostreams pthread stdc++ c)
endif()

include_directories(${CSI_INCLUDE_PATH} ${CMAKE_SOURCE_DIR})
link_directories(${CSI_LIBRARY_PATH})
add_subdirectory(samples)
