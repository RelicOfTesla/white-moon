
@if "%1" == "" goto :_err_args
@if "%2" == "" goto :_err_args
@if "%3" == "" goto :_err_args

@set entry=Raw_DriverEntry
@set syscmd=/nologo /subsystem:native /driver /base:0x10000
@set libpath=/nodefaultlib ntoskrnl.lib ntdll.lib krnln_static.lib /libpath:"%2" /libpath:"%3"
link %1.obj %syscmd% %libpath% /entry:%entry% /out:%1.sys /pdb:%1.pdb
@if exist %1.exp del %1.exp
@if exist %1.lib del %1.lib
@if exist %1.res del %1.res
@if exist %1.dll del %1.dll
@if exist %1.obj del %1.obj

@exit

:_err_args
@echo no set %%1(BaseName),%%2(libdir1),%%3(libdir2)
@pause
@exit
