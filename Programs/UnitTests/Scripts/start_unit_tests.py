#!/usr/bin/env python

# start application on device or PC(Mac) 
# then start server inside this script to listen unit test log output
# example:
# win32: 
# > cd to dava.framework/Projects/UnitTests/Reports
# > python start_unit_tests.py
# mac os x: 
# make DerivedData relative to project go to Xcode->Preferences->Location->DerivedData select relative
# build project
# > cd to dava.framework/Projects/UnitTests/Reports
# > python start_unit_tests.py
# android:
# deploy application to device first
# > python start_unit_tests.py android
# iOS:
# deploy application on device first
# "Enable UI Automation" must be turned on in the "Developer" section of the "Settings" app on device
# > python start_unit_tests.py ios
# universal windows platform:
# deploy application to device first
# > python start_unit_tests.py uwp

import subprocess
import sys
import os
import signal

def get_postfix(platform):
    if platform == 'win32':
        return '.exe'
    elif platform == 'darwin':
        return '.app'
    else:
        return ''

PRJ_NAME_BASE = "UnitTests"
PRJ_POSTFIX = get_postfix(sys.platform)

start_on_android = False
start_on_ios = False
start_on_uwp = False

run_at_teamcity = False

if len(sys.argv) > 1:
    if sys.argv[1] == "android":
        start_on_android = True
    elif sys.argv[1] == "ios":
        start_on_ios = True
    elif sys.argv[1] == "uwp":
        start_on_uwp = True

# define if tests were started on teamcity
params_count = len(sys.argv)
for index in range(1, params_count):
    if sys.argv[index] == "--teamcity":
        run_at_teamcity = True
        break

sub_process = None

def start_unittests_on_android_device():
    global sub_process
    # if screen turned off
    device_state = subprocess.check_output(['adb', 'shell', 'dumpsys', 'power'])
    if device_state.find("mScreenOn=false") != -1:
        # turn screen on
        subprocess.check_call(['adb', 'shell', 'input', 'keyevent', '26'])
    # unlock device screen
    subprocess.check_call(['adb', 'shell', 'input', 'keyevent', '82'])
    # clear log before start tests
    subprocess.check_call(["adb", "logcat", "-c"])
    # start adb logcat and gather output DO NOT filter by TeamcityOutput tag
    # because we need interrupt gather log when unittests process finished
    sub_process = subprocess.Popen(
        ["adb", "logcat", "-s", "TeamcityOutput", "AndroidRuntime:E", "ActivityManager:W"],
        stdout=subprocess.PIPE)
    # start unittests on device
    commandLine = ["adb", "shell", "am", "start", "-n", "com.dava.unittests/com.dava.engine.DavaActivity"]
    if run_at_teamcity == True:
        commandLine.extend(["-es", "-teamcity"]) 

    subprocess.Popen(commandLine)
    return sub_process

def start_unittests_on_uwp_device():
    global sub_process
    name = []

    for root, dirs, files in os.walk("../Release"):
        for file in files:
            if file.endswith(".appxbundle") and PRJ_NAME_BASE in file:
                 name.append(os.path.join(root, file))

    package_name = name[0]
    arch = sys.argv[2]

    commandLine = ["../../../Bin/UWPRunner.exe", 
                                    '--package', package_name, 
                                    '--arch', arch,
                                    '--tc_test', '--dava_app']
    if run_at_teamcity == True:
        commandLine.extend(['--cmd_line', '-teamcity']) 

    sub_process = subprocess.Popen(commandLine, stdout=subprocess.PIPE, stderr=subprocess.PIPE)    
    return sub_process

if start_on_ios:

    commandLine = ["./ios-deploy", "-d", "--noninteractive", "-b", "../build/" + PRJ_NAME_BASE + PRJ_POSTFIX]
    if run_at_teamcity == True:
        commandLine.extend(["-a", "-teamcity"]) 

    # ../build/ios-deploy -d --noninteractive -b ../build/UnitTests.app
    sub_process = subprocess.Popen(commandLine, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print("copy " + PRJ_NAME_BASE + PRJ_POSTFIX + " on device and run")
elif start_on_android:
    sub_process = start_unittests_on_android_device()
elif sys.platform == 'win32' and start_on_uwp == False:
    commandLine = []

    if os.path.isfile("..\\Release\\app\\" + PRJ_NAME_BASE + PRJ_POSTFIX):  # run on build server (TeamCity)
        commandLine = ["..\\Release\\app\\" + PRJ_NAME_BASE + PRJ_POSTFIX]
    else:
        commandLine = ["..\\Release\\" + PRJ_NAME_BASE + PRJ_POSTFIX]

    if run_at_teamcity == True:
        commandLine.extend(["-teamcity"])

    sub_process = subprocess.Popen(commandLine, cwd="./..", stdout=subprocess.PIPE, stderr=subprocess.PIPE)


elif sys.platform == 'win32' and start_on_uwp == True:
    # run appx to Win10 device
    sub_process = start_unittests_on_uwp_device()

elif sys.platform == "darwin":
    if os.path.exists("./" + PRJ_NAME_BASE + PRJ_POSTFIX):
        # if run on teamcity current dir is: Projects/UnitTests/DerivedData/TemplateProjectMacOS/Build/Products/Release
        app_path = "./" + PRJ_NAME_BASE + PRJ_POSTFIX + "/Contents/MacOS/" + PRJ_NAME_BASE
    else:
        # run on local machine from dir: UnitTests/Report
        # Warning! To make DerivedData relative to project go to
        # Xcode->Preferences->Location->DerivedData select relative
        app_path = "../DerivedData/TemplateProjectMacOS/Build/Products/Release/UnitTests.app/Contents/MacOS/" \
                   + PRJ_NAME_BASE

    commandLine = [app_path]
    if run_at_teamcity == True:
        commandLine.extend(["-teamcity"])

    sub_process = subprocess.Popen(commandLine, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

elif sys.platform == 'linux2':
    app_path = "../build/app/" + PRJ_NAME_BASE
    commandLine = [app_path]
    if run_at_teamcity == True:
        commandLine.extend(["-teamcity"])
    os.environ['ASAN_OPTIONS'] = 'check_malloc_usable_size=0:detect_leaks=0'
    sub_process = subprocess.Popen(commandLine, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

app_exit_code = 0

continue_process_stdout = True

while continue_process_stdout:
    try:
        line = sub_process.stdout.readline()
        if line != '':
            teamcity_line_index = line.find("##teamcity")
            if teamcity_line_index != -1:
                teamcity_line = line[teamcity_line_index:]
                sys.stdout.write(teamcity_line)
                sys.stdout.flush()
            if line.find("Finish all tests.") != -1:    # this text marker helps to detect good \
                                                        #  finish tests on ios device (run with lldb)
                if start_on_android:
                    # we want to exit from logcat process because sub_process.stdout.readline() will block
                    # current thread
                    if sys.platform == "win32":
                        sub_process.send_signal(signal.CTRL_C_EVENT)
                    else:
                        sub_process.send_signal(signal.SIGINT)
                    continue_process_stdout = False
                if sys.platform == 'linux2':
                    continue_process_stdout = False
                    while True:
                        errline = sub_process.stderr.readline()
                        if errline == '':
                            break
                        sys.stderr.write(errline)
                        sys.stderr.flush()
                        app_exit_code = 1
            if line.find("E/AndroidRuntime") != -1:
                sys.stdout.write(line)
                sys.stdout.flush()
            if line.find("Force finishing activity com.dava.unittests") != -1: #or line.find("end=assert=msg") != -1: TODO: asserts should stop execution
                app_exit_code = 1
                sys.stdout.write(line)
                sys.stdout.flush()
                
                if start_on_android:
                    # we want to exit from logcat process because sub_process.stdout.readline() will block
                    # current thread
                    if sys.platform == "win32":
                        sub_process.send_signal(signal.CTRL_C_EVENT)
                    else:
                        sub_process.send_signal(signal.SIGINT)
                    continue_process_stdout = False
        else:
            sub_process.wait()
            continue_process_stdout = False
    except IOError as err:
        sys.stdout.write(err.message)
        sys.stdout.flush()


# if sys.platform == "darwin" and start_on_ios == False and start_on_android == False:
#     pathScriptDir        = os.path.dirname(os.path.realpath(__file__))
#     pathCoverageDir      = os.path.realpath( os.path.join(pathScriptDir, '../../../RepoTools/coverage') )
#     pathHtmlReportScript = os.path.join( pathCoverageDir, 'coverage_report.py' )   
#     pathBuild            = os.path.realpath( os.path.join( os.getcwd(), '../' ) )
#     pathExecut           = os.path.realpath( os.path.join( os.getcwd(), '{0}.app'.format( PRJ_NAME_BASE ) ) )
#     pathReportOut        = os.path.join( pathBuild, 'Coverage')    

#     params = [  'python', pathHtmlReportScript,
#                 '--pathBuild', pathBuild,  
#                 '--pathExecut', pathExecut,                   
#                 '--pathReportOut', pathReportOut,
#                 '--buildConfig', 'Release',
#                 '--notExecute' , 'true',
#                 '--teamcityMode' , 'true', 
#                 '--buildMode', 'true'                
#                  ]

#     subprocess.call(params)

if app_exit_code == 1:
    sys.exit(app_exit_code)
else:
    sys.exit(sub_process.returncode)
