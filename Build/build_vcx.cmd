@echo off
setlocal

set trunk=%~dp0..\

if "%1"=="" goto usage
if "%1"=="-?" goto usage
if "%1"=="/?" goto usage
if /I "%1"=="help" goto usage

if "%~2" NEQ "" (set "config=%~2") else (set config=Release)

if "%~3" NEQ "" (set "platform=%~3") else (set platform=win32)

if "%~4" NEQ "" (set "buildopt=%~4")

if "%~5" NEQ "" (set "%platformtoolset%=%~5") else (set platformtoolset=v80)

if /I %platform%==win32 set vsvarplatform=x86
if /I %platform%==x64 set vsvarplatform=x86_amd64

set logname=%~dp0%~nx1.%config: =%.%platform: =%

set "vscomntools=%vs80comntools%"
if /I %platformtoolset%==v80 set "vscomntools=%vs80comntools%"
if /I %platformtoolset%==v100 set "vscomntools=%vs100comntools%"
if /I %platformtoolset%==v140 set "vscomntools=%vs140comntools%"

call "%vscomntools%..\..\vc\vcvarsall.bat" %vsvarplatform%

set buildopt=%buildopt% /t:Rebuild /p:VCTargetsPath=%~dp0msbuildcpp\ /p:ForceImportBeforeCppTargets=%~dp0cppdefaults.props
set buildopt=%buildopt% /p:Configuration=%config% /p:Platform=%platform% /p:PlatformToolset=%platformtoolset%

@echo on
%systemroot%\Microsoft.NET\Framework\v4.0.30319\msbuild %DF_MSBUILD_BUILD_STATS_OPTS% %1 %buildopt% > %logname%.log
@echo off

findstr /I /C:": fatal error" /C:": error" /C:": command line error" %logname%.log > %logname%.err
findstr /I /C:": warning" %logname%.log > %logname%.wrn

REM clean the report files that are zero size - i.e. no errors/warnings in them
call %~dp0clean_report.cmd %logname%.err
call %~dp0clean_report.cmd %logname%.wrn

goto :eof

:usage
echo This script builds all native modules using msbuild in the specified solution.
echo Usage: "build_vcx.cmd <solution> [<configuration>] [<platform>] [<buildoptions>] [<PlatformToolset>]"
echo By specifying the module name you can build only a particular module ( and all if its dependencies ).
echo The default behaviour is to rebuild every time the release configuration for the x86 platform using v80 platformtoolset.
echo Examples:
echo 	build_vcx.cmd server.sln Release AnyCPU - will rebuild the entire release solution for the AnyCPU platform.
echo 	build_vcx.cmd tiff32.vcproj Debug x86 - will rebuild the tiff32 debug project for the x86 platform.
echo 	build_vcx.cmd tiff32.vcproj - will rebuild the tiff32 release project.