import sys
from os import system
from subprocess import Popen
from threading import Thread
from time import sleep


def exec_process(cpp_file, exe_file, sh):
    system('g++ ' + cpp_file + ' -o ' + exe_file)

    process = Popen("./{}".format(exe_file), shell=sh)

    out, err = process.communicate()
    errcode = process.returncode
    print(errcode)

    process.kill()
    process.terminate()

"""
def evaluate_command():
    while True:
        file = open("command", "r", encoding="utf-8")
        command = file.readline()
        if command.find("show gpio") != -1:
            file = open("command", "w", encoding="utf-8")

            inputs = command.split("-")
            funct = inputs[1]
            gpiox = inputs[2]

            print(funct + " " + gpiox)

            file.write(funct + " " + gpiox)

            file.close()

        sleep(0.2)
        file.close()
"""


cpp_file = "/home/onur/CLionProjects/bbx15/GPIO-Driver/main.cpp /home/onur/CLionProjects/bbx15/GPIO-Driver/gpiolib.cpp /home/onur/CLionProjects/bbx15/GPIO-Driver/gpiolib.h"
exe_file = "program.out"

print("program is being built ...")

program = Thread(target=exec_process, args=(cpp_file, exe_file, True))
program.start()

file = open("command", "r", encoding="utf-8")

while file.readline() != "start":
    file.seek(0, 0)

file.close()
file = open("command", "w", encoding="utf-8")
file.write("&")
file.close()

cpp_file = "/home/onur/CLionProjects/tester/main.cpp"
exe_file = "tester.out"

tester = Thread(target=exec_process, args=(cpp_file, exe_file, True))
tester.start()


"""
communicator = Thread(target=evaluate_command)
communicator.start()
"""




#exec_process(cpp_file, exe_file, False)


