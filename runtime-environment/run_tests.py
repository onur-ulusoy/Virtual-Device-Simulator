"""
@file run_tests.py
@brief Script for running Python scripts via Robot Framework.

This script defines the RunScript class, which Robot Framework can use to run 
specific Python scripts. 

This class is integrated into the Robot Framework by using the `@keyword` decorator. 
This makes the methods of the RunScript class available as keywords within test 
cases written in Robot Framework.

Robot Framework utilizes the `run_test1` method, marked as a keyword, 
to run the Python script 'test1.py'. It runs this script in a separate process 
and captures the script's standard output, which is then returned as a string.

Example of use in Robot Framework:
| ${output}= | Run Test1 |
| Log To Console | ${output} |

@author: Onur Ulusoy
@date: 22/04/2023
@license: MIT
"""

import subprocess
from robot.api.deco import keyword

class RunScript:

    @keyword
    def run_test1(self):
        result = subprocess.run(['python3', 'test1.py'], stdout=subprocess.PIPE)
        return result.stdout.decode('utf-8')
