@if exist ..\..\e.exe goto j_check
@echo 请把本文件放置于\e\tools\drv\中
@pause
@exit

:j_check
@if not exist krnln_static_raw.lib goto j_start
@echo 请勿重复运行
@pause
@exit


:j_start
copy ..\..\static_lib\krnln_static.lib krnln_static_raw.lib
copy ..\..\static_lib\krnln_static.res krnln_static_raw.res
copy ..\link.ini link_win32.ini

@echo.
@echo 使用switch_win32/switch_drv为切换 win桌面程序/win驱动程序 的编译
@echo 请先修改link_drv.ini中的libpath到ddk中(让E能编译obj通过)
@echo 编译时请复制所需要的lib到.e目录中，或者将ddk目录设置在%%path%%中(让手工link时能识别)
@echo.

@pause