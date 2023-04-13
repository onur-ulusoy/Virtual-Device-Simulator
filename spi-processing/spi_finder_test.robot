*** Settings ***
Library           Process
Library           OperatingSystem
Library           pexpect_wrapper.py
Suite Setup       Terminate All Processes    python3
Suite Teardown    Terminate All Processes    python3

*** Test Cases ***
Run Spi Script With Input Periodically
    Start Process    python3    spi_finder.py    alias=spi_finder_script    stdout=STDOUT
    FOR    ${i}    IN RANGE    10
        Sleep    2 seconds
        ${output}=    Send Input To Spi Finder    input_file.txt
        Log    ${output}
    END
    Terminate All Processes    python3
