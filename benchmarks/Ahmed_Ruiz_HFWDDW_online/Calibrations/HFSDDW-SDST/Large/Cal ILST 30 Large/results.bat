for /r "C:\Users\Ahmed\Pictures\SDST Instances\Calibration folder\Cal ILST" %%i in (*.m) do for /f "delims=" %%j in ('type "%%i"') do echo %%j>> resultat.txt