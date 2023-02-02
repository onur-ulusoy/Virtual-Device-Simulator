import sys
from os import path
from subprocess import Popen
from threading import Thread

paths = ['driver', 'tester']

for PATH in paths:
    sys.path.append(path.join(path.dirname(__file__), PATH))


def exec_process(exe_file, sh):

    process = Popen("./{}".format(exe_file), shell=sh)

    out, err = process.communicate()
    errcode = process.returncode

    process.kill()
    process.terminate()


if __name__ == "__main__":

    files = ['driver.out', 'tester.out']

    for file in files:
        if not path.exists(file):
            print("Out files are missing.")
            sys.exit()

    device = input("Select device to simulate: gpio, spi, i2c, ethernet, usart, uart, can\n")

    file = open("command", "w", encoding="utf-8")
    file.write(device)
    file.close()

    exe_file = "driver.out"

    program = Thread(target=exec_process, args=(exe_file, True))
    program.start()

    file = open("command", "r", encoding="utf-8")

    while file.readline() != "start":
        file.seek(0, 0)

    file.close()
    file = open("command", "w", encoding="utf-8")
    file.write("&")
    file.close()

    exe_file = "tester.out"

    tester = Thread(target=exec_process, args=(exe_file, True))
    tester.start()

