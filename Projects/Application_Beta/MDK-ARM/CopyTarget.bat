REM Create folder in Binary\, then copy Targets
@echo off
echo "--------------------------------------------------"
REM Date Time
set sDate=%date:~0,4%%date:~5,2%%date:~8,2%
set sTime=%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%
REM Project root
cd ..
REM Get Project Name
set mProjectDir=%cd%
for /f "delims=" %%i in ("%mProjectDir%") do set mProjectName=%%~ni
echo %mProjectName%
REM Create Binary folder
if not exist Binary\Target_%sDate% md Binary\Target_%sDate%
REM Copy files
copy "MDK-ARM\vehicle\vehicle.bin" Binary\Target_%sDate%\%mProjectName%_%sDate%%sTime%.bin
copy "MDK-ARM\vehicle\vehicle.hex" Binary\Target_%sDate%\%mProjectName%_%sDate%%sTime%.hex
type null >> Binary\Target_%sDate%\%mProjectName%_%sDate%%sTime%_EN.bin
type null >> Binary\Target_%sDate%\%mProjectName%_%sDate%%sTime%_EN.dfu

echo "--------------------------------------------------"
REM pause
exit