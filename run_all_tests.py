import sys
from os import chdir

sys.path.append('driver/driver-unit-test')
sys.path.append('runtime-environment/integrity-test')

import run_utests, run_itests

chdir("driver/driver-unit-test")
run_utests.main()

chdir("../../")

chdir("runtime-environment/integrity-test")
run_itests.main()