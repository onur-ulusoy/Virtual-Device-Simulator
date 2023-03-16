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
    chdir("../../")

def run_drv_itests():
    sys.path.append('driver/driver-int-tests')
    import run_drv_itests
    chdir("driver/driver-int-tests")
    run_drv_itests.main()
    chdir("../../")

def argument_error():
    print("***************************")
    print("Example:")
    print("python3 run_tests allunittests")
    print("python3 run_tests sysintegrationtests")
    print("python3 run_tests drvintegrationtests")
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
    elif sys.argv[1] == "drvintegrationtests":
        run_drv_itests()
    elif sys.argv[1] == "sysintegrationtests":
        run_itests()
    elif sys.argv[1] == "allintegrationtests":
        run_drv_itests()
        run_itests()
    elif sys.argv[1] == "alltests":
        run_drv_itests()
        run_utests()
        run_itests()
        
    else:
        print("Argument is invalid")
        argument_error()

if __name__ == "__main__":
    main()