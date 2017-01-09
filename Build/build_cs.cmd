@echo off
setlocal

set trunk=%~dp0..\

if "%1"=="" goto usage
if "%1"=="-?" goto usage
if "%1"=="/?" goto usage
if /I "%1"=="help" goto usage

if "%~2" NEQ "" (set config=%~2) else (set config=Release)

set platform=%~3
if "%platform%" NEQ "" set "buildopt=/p:Platform=%platform%"

if "%~4" NEQ "" set "buildopt=%buildopt% %~4"

if "%platform%"=="" (set "logname=%~dp0%~nx1.%config: =%") else (set "logname=%~dp0%~nx1.%config: =%.%platform: =%")

set buildopt=%buildopt% /t:Rebuild /p:TargetFrameworkVersion=v4.0 /p:Configuration=%config%
set buildopt=%buildopt% /p:DebugType=pdbonly /p:Optimize=true
set buildopt=%buildopt% /p:ReferencePath="%trunk%bin;%LIB%"

@echo on
%systemroot%\Microsoft.NET\Framework\v4.0.30319\msbuild %DF_MSBUILD_BUILD_STATS_OPTS% %1 %buildopt% > %logname%.log
@echo off

findstr /I /C:": fatal error" /C:": error"  %logname%.log > %logname%.err
findstr /I /C:": warning" %logname%.log > %logname%.wrn

REM clean the report files that are zero size - i.e. no errors/warnings in them
call %~dp0clean_report.cmd %logname%.err
call %~dp0clean_report.cmd %logname%.wrn

goto :eof

:usage
echo This script builds all CSharp modules in the specified solution.
echo Usage: "build_cs.cmd <solution> [<configuration>] [<platform>] [<buildoptions>]"
echo By specifying the module name you can build only a particular module ( and all if its dependencies ).
echo The default behaviour is to rebuild every time the release configuration.
echo Examples:
echo 	build_cs.cmd server.sln Release AnyCPU - will rebuild the entire release solution for the AnyCPU platform.
echo 	build_cs.cmd tiff32.csproj Debug x86 - will rebuild the tiff32 debug project for the x86 platform.
echo 	build_cs.cmd tiff32.csproj - will rebuild the tiff32 release project.