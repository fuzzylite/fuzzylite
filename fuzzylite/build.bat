@echo off
setlocal EnableDelayedExpansion
set argc=0
set valid="no"

for %%a in (%*) do (
	set /A argc+=1
	if /I "%%a"=="help" (
		call:usage
		goto:eof
	)
	if /I "%%a"=="all" set valid="yes"
	if /I "%%a"=="release" set valid="yes"
	if /I "%%a"=="debug" set valid="yes"
	if /I "%%a"=="clean" set valid="yes"
	if /I "%%a"=="documentation" set valid="yes"

	if !valid!=="no" (
		echo Invalid option: %%a
		call:usage
		goto:eof
	)
)

if %argc%==0 echo Building schedule: all
if not %argc%==0 echo Building schedule: %*
echo Starting in 3 seconds...
ping 1.1.1.1 -n 1 -w 3000 > nul
rem sleep 3 ::This function makes command line DOS-esque C:\Archiv~1

if %argc%==0 (call:all)

for %%a in (%*) do (call:%%a)

goto:eof

:debug
	echo.
	echo.
	echo ****************************************
	echo STARTING: debug

	if not exist debug mkdir debug
	cd debug
	cmake .. -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DFL_BACKTRACE=ON -DFL_USE_FLOAT=OFF -DFL_CPP98=OFF -DFL_BUILD_TESTS=ON
	nmake
	cd ..

	echo.
	echo FINISHED: debug
	echo ****************************************
	goto:eof

:release
	echo.
	echo.
	echo ****************************************
	echo STARTING: release

	if not exist release mkdir release
	cd release
	cmake .. -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DFL_BACKTRACE=OFF -DFL_USE_FLOAT=OFF -DFL_CPP98=OFF -DFL_BUILD_TESTS=ON
	nmake
	cd ..

	echo.
	echo FINISHED: release
	echo ****************************************
	goto:eof

:all
	echo.
	echo.
	echo ****************************************
	echo STARTING: all
	call:release
	call:debug
	echo.
	echo FINISHED: all
	echo ****************************************
	goto:eof

:documentation
	echo.
	echo.
	echo ****************************************
	echo STARTING: documentation

	cd ..
	doxygen
	rem TODO: cd back to previous directory. Maybe use: cd /D %~dp0
	echo.
	echo FINISHED: documentation
	echo ****************************************
	goto:eof
:clean
	echo.
	echo.
	echo ****************************************
	echo STARTING: clean
	@echo on
	if exist debug rmdir /S /Q debug
	if exist release rmdir /S /Q release
	if exist CMakeFiles rmdir /S /Q CMakeFiles
	if exist CMakeCache.txt del CMakeCache.txt
	@echo off
	echo.
	echo FINISHED: clean
	echo ****************************************
	goto:eof

:usage
    echo Usage:	build.bat [options]
    echo where	[options] can be any of the following:
    echo ^	all		builds fuzzylite in debug and release mode (default)
    echo ^	debug		builds fuzzylite in debug mode
    echo ^	release		builds fuzzylite in release mode
    echo ^	clean		erases previous builds
    echo ^	help		shows this information
	echo.

ENDLOCAL
