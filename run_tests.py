import sys
from os import chdir

def run_utests():
    sys.path.append('driver/driver-unit-test')
    import run_utests
    chdir("driver/driver-unit-test")
    run_utests.main()
    chdir("../../")

def run_itests():
    sys.path.append('runtime-environment/integration-test')
    import run_itests
    chdir("runtime-environment/integration-test")
    run_itests.main()

def argument_error():
    print("***************************")
    print("Example:")
    print("python3 run_tests allunittests")
    print("python3 run_tests allintegrationtests")
    print("python3 run_tests alltests")
    print("***************************")
    exit()

def main():
    if len(sys.argv) <= 1:
        print("Argument must be passed")
        argument_error()
    elif sys.argv[1] == "allunittests":
        run_utests()
    elif sys.argv[1] == "allintegrationtests":
        run_itests()
    elif sys.argv[1] == "alltests":
        run_utests()
        run_itests()
    else:
        print("Argument is invalid")
        argument_error()

if __name__ == "__main__":
    main()