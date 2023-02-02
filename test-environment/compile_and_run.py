import sys
from os import system, path
from subprocess import Popen
from threading import Thread

paths = ['driver', 'tester']

for PATH in paths:
    sys.path.append(path.join(path.dirname(__file__), PATH))


def exec_process(cpp_files, exe_file, sh, dir):

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


if __name__ == "__main__":

    device = input("Select device to simulate: gpio, spi, i2c, ethernet, usart, uart, can\n")

    file = open("command", "w", encoding="utf-8")
    file.write(device)
    file.close()

    cpp_files = ["main.cpp", "DriverLibrary.cpp", "DriverLibrary.h", "AuxiliaryTools.cpp", "AuxiliaryTools.h"]
    exe_file = "driver.out"

    print("driver is being built ...")

    program = Thread(target=exec_process, args=(cpp_files, exe_file, True, "driver"))
    program.start()

    file = open("command", "r", encoding="utf-8")

    while file.readline() != "start":
        file.seek(0, 0)

    file.close()
    file = open("command", "w", encoding="utf-8")
    file.write("&")
    file.close()

    cpp_files = ["main.cpp"]
    exe_file = "tester.out"

    print("\ntester is being built ...")

    tester = Thread(target=exec_process, args=(cpp_files, exe_file, True, "tester"))
    tester.start()

