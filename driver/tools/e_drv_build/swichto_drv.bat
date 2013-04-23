if not exist link_win32.ini exit

copy /y link_drv.ini ..\link.ini
del /q ..\..\static_lib\krnln_static.res
copy /y krnln_static_by.lib ..\..\static_lib\krnln_static.lib

pause
