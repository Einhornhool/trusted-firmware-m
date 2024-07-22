
set WORK_DIR=%CD%
set BUILD_DIR=%WORK_DIR%\iar
set BUILD_TFM=build_s

mkdir %BUILD_DIR%
cd %BUILD_DIR%
del /S/Q %BUILD_TFM%

set WORK_DIR=%WORK_DIR:\=/%


set TFM_SOURCE=%WORK_DIR%/trusted-firmware-m

set TOOL_CHAIN=-DTFM_TOOLCHAIN_FILE=%TFM_SOURCE%/toolchain_IARARM.cmake
::platform
set TARGET=-DTFM_PLATFORM=stm/b_u585i_iot02a
::set TARGET=-DTFM_PLATFORM=stm/stm32h573i_dk
::set TARGET=-DTFM_PLATFORM=stm/stm32l562e_dk

::profile
set PROFILE=-DTFM_PROFILE=profile_medium 

::library
set MCUBOOT_SRC=-DMCUBOOT_PATH=%WORK_DIR%/mcuboot-src
set MBEDCRYPTO_SRC=-DMBEDCRYPTO_PATH=%WORK_DIR%/mbedtls
set TFMTEST_SRC=%WORK_DIR%/tf-m-tests/tests_reg/spe
set QCBOR=-DQCBOR_PATH=%WORK_DIR%/QCBOR

set BUILD_TYPE=RelWithDebInfo
::set BUILD_TYPE=Debug
set MBED_BUILD_TYPE=RelWithDebInfo
::set MBED_BUILD_TYPE=Debug
set BUILD_NS_TESTS=-DTEST_NS=ON
set BUILD_S_TESTS=-DTEST_S=ON


cmake -S %TFMTEST_SRC% -B %BUILD_TFM% -GNinja %TARGET% %TOOL_CHAIN% -DCONFIG_TFM_SOURCE_PATH=%TFM_SOURCE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% %PROFILE% -DTFM_PSA_API=OFF -DTFM_ISOLATION_LEVEL=2 -DTEST_S=OFF -DTEST_NS=OFF -DTEST_S_CRYPTO=OFF -DTEST_NS_CRYPTO=OFF
ninja -C %BUILD_TFM% -j12 install
pause