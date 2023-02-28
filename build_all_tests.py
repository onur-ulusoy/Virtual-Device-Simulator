import sys
from os import chdir

sys.path.append('driver/driver-unit-test')
sys.path.append('runtime-environment/integrity-test')

import build_utests, build_itests

chdir("driver/driver-unit-test")
build_utests.main()

chdir("../../..")
chdir("runtime-environment/integrity-test")

build_itests.main()