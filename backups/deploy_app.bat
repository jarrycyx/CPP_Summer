del ..\deploytest\*
copy ..\Release\release\*.exe ..\deploytest
C:\Qt\Qt5.13.0\5.13.0\mingw73_64\bin\windeployqt -sql ..\deploytest/H1.exe --qmldir C:\Qt\Qt5.13.0\5.13.0\mingw73_64\qml --plugindir ../deploytest/plugins
copy Deploy_addons\*.dll ..\deploytest