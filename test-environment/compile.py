import sys
from os import system, path
from threading import Thread

paths = ['driver', 'tester']

for PATH in paths:
    sys.path.append(path.join(path.dirname(__file__), PATH))


def exec_process(cpp_files, exe_file, dir):

    cpp_file = str()
    
    for i in cpp_files:
        cpp_file += "../" + dir + "/" + i + " "
        
    cpp_file.rstrip()

    system('g++ ' + cpp_file + ' -o ' + exe_file)
    print(dir + " build finished.")


if __name__ == "__main__":

    cpp_files = ["main.cpp"]
    exe_file = "tester.out"

    print("\ntester is being built ...")

    tester = Thread(target=exec_process, args=(cpp_files, exe_file, "tester"))
    tester.start()
    
    ############
    
    cpp_files = ["main.cpp", "DriverLibrary.cpp", "DriverLibrary.h", "AuxiliaryTools.cpp", "AuxiliaryTools.h"]
    exe_file = "driver.out"

    print("driver is being built ...")

    program = Thread(target=exec_process, args=(cpp_files, exe_file, "driver"))
    program.start()


    

