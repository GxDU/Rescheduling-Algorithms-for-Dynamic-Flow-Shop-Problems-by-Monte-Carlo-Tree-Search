for /r "C:\Users\Ahmed\Pictures\Calibration ADA\Cal_Large _ADA" %%i in (*.m) do for /f "delims=" %%j in ('type "%%i"') do echo %%j>> resultat.txt