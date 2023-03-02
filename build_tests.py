import sys
from os import chdir

def build_utests():
    sys.path.append('driver/driver-unit-test')
    import build_utests
    chdir("driver/driver-unit-test")
    build_utests.main()
    chdir("../../..")

def build_itests():
    sys.path.append('runtime-environment/integration-test')
    import build_itests
    chdir("runtime-environment/integration-test")
    build_itests.main()

def argument_error():
    print("***************************")
    print("Example:")
    print("python3 build_tests allunittests")
    print("python3 build_tests allintegrationtests")
    print("python3 build_tests alltests")
    print("***************************")
    exit()

def main():
    if len(sys.argv) <= 1:
        print("Argument must be passed")
        argument_error()
    elif sys.argv[1] == "allunittests":
        build_utests()
    elif sys.argv[1] == "allintegrationtests":
        build_itests()
    elif sys.argv[1] == "alltests":
        build_utests()
        build_itests()
    else:
        print("Argument is invalid")
        argument_error()

if __name__ == "__main__":
    main()