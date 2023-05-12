*** Settings ***
Library    run_tests.RunScript

*** Test Cases ***
Run Test1 Script
    ${output}=    Run Test1
    Log    ${output}
