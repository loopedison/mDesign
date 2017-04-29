REM Create folder in Binary\, then copy Targets into
@echo off
set ToDay=%date:~0,4%%date:~5,2%%date:~8,2%
set TT=%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%
if not exist ..\Binary\Target_%ToDay% (
    md ..\Binary\Target_%ToDay%
)
copy ".\vehicle\vehicle.bin" ..\Binary\Target_%ToDay%\%ToDay%%TT%_vehicle.bin
copy ".\vehicle\vehicle.hex" ..\Binary\Target_%ToDay%\%ToDay%%TT%_vehicle.hex
type null >> ..\Binary\Target_%ToDay%\%ToDay%%TT%_vehicle_en.bin
type null >> ..\Binary\Target_%ToDay%\%ToDay%%TT%_vehicle_en.dfu
REM pause
exit