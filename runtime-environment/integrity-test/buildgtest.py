from os import system, chdir, remove, path

def build():
    ## Run cmake command
    cmake_command = 'cmake -B build -G "Unix Makefiles"'
    system(cmake_command)

    ## Change directory to build/
    chdir('./build')

    ## Run make command
    make_command = 'make'
    system(make_command)

def remove_out_files():
    # List of file names to remove
    file_list = ['driver.out', 'tester.out']
    # Loop through the list and remove each file
    for file in file_list:
        if path.exists(file):
            remove(file)

def main():
    build()
    chdir('..')
    remove_out_files()
    

if __name__ == "__main__":
    main()
