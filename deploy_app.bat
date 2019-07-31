del ..\deploytest\*
copy ..\build-WeTranslate-Desktop_Qt_5_13_0_MinGW_64_bit-Release\release\*.exe ..\deploytest
C:\Qt\Qt5.13.0\5.13.0\mingw73_64\bin\windeployqt -sql ..\deploytest/WeTranslate.exe --qmldir C:\Qt\Qt5.13.0\5.13.0\mingw73_64\qml --plugindir ../deploytest/plugins
copy Deploy_addons\*.dll ..\deploytest