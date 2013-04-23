if not exist link_win32.ini exit

copy /y link_win32.ini ..\link.ini
copy /y krnln_static_raw.lib ..\..\static_lib\krnln_static.lib
copy /y krnln_static_raw.res ..\..\static_lib\krnln_static.res

pause