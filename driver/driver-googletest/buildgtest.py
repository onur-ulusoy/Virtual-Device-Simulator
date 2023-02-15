from os import system, chdir

def build():
    ## Run cmake command
    cmake_command = 'cmake -B build -G "MinGW Makefiles"'
    system(cmake_command)

    ## Change directory to build/
    chdir('./build')

    ## Run make command
    make_command = 'make'
    system(make_command)

def main():
    build()


if __name__ == "__main__":
    main()
