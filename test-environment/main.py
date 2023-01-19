import sys

from os import system, path, chdir, getcwd
from subprocess import Popen
from threading import Thread

paths = ['GPIO-Driver', 'tester']

for PATH in paths:
    sys.path.append(path.join(path.dirname(__file__), PATH))


def exec_process(cpp_files, exe_file, sh, dir):

    #path.abspath(path.join(getcwd(), '../bbx15-driver/{}').format(dir))
    # #print(getcwd())
    #
    # chdir(dir)
    # #chdir("..")
    # #chdir("..")
    # print(getcwd())
    # print(dir)

    cpp_file = str()
    for i in cpp_files:
        #print(cpp_file)
        cpp_file += "../" + dir + "/" + i + " "
    cpp_file.rstrip()
    #print(cpp_file)
    system('g++ ' + cpp_file + ' -o ' + exe_file)

    process = Popen("./{}".format(exe_file), shell=sh)

    # out, err = process.communicate()
    # errcode = process.returncode

    process.kill()
    process.terminate()


if __name__ == "__main__":

    cpp_files = ["main.cpp", "gpiolib.cpp", "gpiolib.h"]
    exe_file = "program.out"

    print("program is being built ...")

    program = Thread(target=exec_process, args=(cpp_files, exe_file, True, "GPIO-Driver"))
    program.start()

    #print(getcwd())
    #chdir('test-environment')
    #print(getcwd())

    file = open("command", "r", encoding="utf-8")

    while file.readline() != "start":
        file.seek(0, 0)

    file.close()
    file = open("command", "w", encoding="utf-8")
    file.write("&")
    file.close()

    cpp_files = ["main.cpp"]
    exe_file = "tester.out"

    tester = Thread(target=exec_process, args=(cpp_files, exe_file, True, "tester"))
    tester.start()
