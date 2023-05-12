import subprocess
from robot.api.deco import keyword

class RunScript:

    @keyword
    def run_test1(self):
        result = subprocess.run(['python3', 'test1.py'], stdout=subprocess.PIPE)
        return result.stdout.decode('utf-8')
