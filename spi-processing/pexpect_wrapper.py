import pexpect

def send_input_to_spi_finder(input_file):
    child = pexpect.spawn("python spi_finder.py")
    child.expect(".*")
    child.sendline(input_file)
    child.expect("------")
    return child.before.decode("utf-8")
