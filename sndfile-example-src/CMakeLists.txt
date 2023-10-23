cmake_minimum_required(VERSION 3.16)
project(sndfile-example)

SET (CMAKE_BUILD_TYPE "Release")
#SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(LIBSNDFILE_PKGCONF sndfile)
        pkg_check_modules(FFTW3_PKGCONF fftw3)
    endif(PKG_CONFIG_FOUND)

    include_directories(${LIBSNDFILE_PKGCONF_INCLUDE_DIRS})
    link_directories(${LIBSNDFILE_PKGCONF_LIBRARY_DIRS})
    include_directories(${FFTW3_PKGCONF_INCLUDE_DIRS})
    link_directories(${FFTW3_PKGCONF_LIBRARY_DIRS})
endif()

SET (BASE_DIR ${CMAKE_SOURCE_DIR} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../sndfile-example-bin)

add_executable (wav_quant wav_quant.cpp)
target_link_libraries (wav_quant sndfile wav_dct2)

add_executable (wav_cp wav_cp.cpp)
target_link_libraries (wav_cp sndfile)

add_executable (wav_hist wav_hist.cpp)
target_link_libraries (wav_hist sndfile)

add_executable (wav_dct wav_dct.cpp)
target_link_libraries (wav_dct sndfile fftw3)

add_executable (wav_cmp wav_cmp.cpp)
target_link_libraries (wav_cmp sndfile)

add_executable (wav_effects wav_effects.cpp)
target_link_libraries (wav_effects sndfile)

add_executable(test_BitStream test_BitStream.cpp)

add_executable (encoder encoder.cpp)

add_executable (decoder decoder.cpp)

add_executable (wav_dct2 wav_dct2.cpp)
target_link_libraries (wav_dct2 sndfile fftw3)
