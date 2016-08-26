for /D %%i in (%1\*) do (
nuget spec %%~nxi
move %%~nxi.nuspec %%i
)
