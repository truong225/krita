SET(PREFIX_ext_quazip "${EXTPREFIX}" )
if (ANDROID)
    ExternalProject_Add( ext_quazip
        DOWNLOAD_DIR ${EXTERNALS_DOWNLOAD_DIR}
        URL https://github.com/stachenov/quazip/archive/v0.8.1.tar.gz
        URL_MD5 0161c679b1d32c289dab2f96c81354aa
        PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/find_quazip.diff
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/liblocation.diff
        INSTALL_DIR ${PREFIX_ext_quazip}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PREFIX_ext_quazip} -DCMAKE_BUILD_TYPE=${GLOBAL_BUILD_TYPE} ${GLOBAL_PROFILE} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DANDROID_PLATFORM=${ANDROID_PLATFORM} -DQt5Core_DIR=$ENV{QT_ANDROID}/lib/cmake/Qt5Core/ -DANDROID_ABI=${ANDROID_ABI}

        UPDATE_COMMAND ""
        DEPENDS ext_zlib
)
else()
    ExternalProject_Add( ext_quazip
        DOWNLOAD_DIR ${EXTERNALS_DOWNLOAD_DIR}
        URL https://github.com/stachenov/quazip/archive/v0.8.1.tar.gz
        URL_MD5 0161c679b1d32c289dab2f96c81354aa
        PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/find_quazip.diff
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/liblocation.diff
        INSTALL_DIR ${PREFIX_ext_quazip}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PREFIX_ext_quazip} -DCMAKE_BUILD_TYPE=${GLOBAL_BUILD_TYPE} ${GLOBAL_PROFILE}

        UPDATE_COMMAND ""
        DEPENDS ext_zlib
    )
endif()

