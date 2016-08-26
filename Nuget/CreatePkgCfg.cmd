echo off

echo ^<packages^> > packages.config

for %%i in (%1\*.nupkg) do (
echo ^<package id="%%~ni"/^> >> packages.config
echo
)
echo ^</packages^> >> packages.config