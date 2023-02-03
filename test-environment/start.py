import sys
from os import system, path
from subprocess import Popen
from threading import Thread

paths = ['driver', 'tester']

driver_cpp_files = [ "drivermain.cpp",
                     "libdriver.cpp", 
                     "libdriver.h", 
                     "libdriverutility.cpp", 
                     "libdriverutility.h", 
                     "drivercommapi.cpp", 
                     "drivercommapi.h" ]


driver_exe_file = "driver.out"

tester_cpp_files = cpp_files = [ "testermain.cpp",
                                 "libtesterutility.cpp", 
                                 "libtesterutility.h", 
                                 "testercommapi.cpp", 
                                 "testercommapi.h" ]

tester_exe_file = "tester.out"

for PATH in paths:
    sys.path.append(path.join(path.dirname(__file__), PATH))


def compile_and_run(cpp_files, exe_file, sh, dir):

    cpp_file = str()
    
    for i in cpp_files:
        cpp_file += "../" + dir + "/" + i + " "
        
    cpp_file.rstrip()

    system('g++ ' + cpp_file + ' -o ' + exe_file)
    print("Build finished.")
    process = Popen("./{}".format(exe_file), shell=sh)

    out, err = process.communicate()
    errcode = process.returncode

    process.kill()
    process.terminate()

def compile(cpp_files, exe_file, dir):

    cpp_file = str()
    
    for i in cpp_files:
        cpp_file += "../" + dir + "/" + i + " "
        
    cpp_file.rstrip()

    system('g++ ' + cpp_file + ' -o ' + exe_file)
    print(dir + " build finished.")

def run(exe_file, sh):

    process = Popen("./{}".format(exe_file), shell=sh)

    out, err = process.communicate()
    errcode = process.returncode

    process.kill()
    process.terminate()

if __name__ == "__main__":
    try:
        request = sys.argv[1]
    except IndexError:
        request = input("Enter one of them:\ncompile\nrun\ncompile_run\n")

    if request == "compile":
        print("\ntester is being built ...")

        tester = Thread(target=compile, args=(tester_cpp_files, tester_exe_file, "tester"))
        tester.start()

        print("driver is being built ...")

        program = Thread(target=compile, args=(driver_cpp_files, driver_exe_file, "driver"))
        program.start()

    elif request == "run":
        files = ['driver.out', 'tester.out']

        for file in files:
            if not path.exists(file):
                print("Out files are missing.")
                sys.exit()

        device = input("Select device to simulate: gpio, spi, i2c, ethernet, usart, uart, can\n")

        file = open("command", "w", encoding="utf-8")
        file.write(device)
        file.close()

        program = Thread(target=run, args=(driver_exe_file, True))
        program.start()

        file = open("command", "r", encoding="utf-8")

        while file.readline() != "start":
            file.seek(0, 0)

        file.close()
        file = open("command", "w", encoding="utf-8")
        file.write("&")
        file.close()

        program = Thread(target=run, args=(tester_exe_file, True))
        program.start()

    else:
        device = input("Select device to simulate: gpio, spi, i2c, ethernet, usart, uart, can\n")

        file = open("command", "w", encoding="utf-8")
        file.write(device)
        file.close()

        print("driver is being built ...")

        program = Thread(target=compile_and_run, args=(driver_cpp_files, driver_exe_file, True, "driver"))
        program.start()

        file = open("command", "r", encoding="utf-8")

        while file.readline() != "start":
            file.seek(0, 0)

        file.close()
        file = open("command", "w", encoding="utf-8")
        file.write("&")
        file.close()

        print("\ntester is being built ...")

        program = Thread(target=compile_and_run, args=(tester_cpp_files, tester_exe_file, True, "tester"))
        program.start()




    

    

    

