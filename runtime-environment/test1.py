"""
@file test1.py

Example test scenerio script utilizes test_suite module.
Visit README in root directory for further information.

@author: Onur Ulusoy
@date: 18/05/2023
@license: MIT
"""

from test_suite import *

remote_repository = local_directory + "/../program_tested_mock/env"

run_process(remote_repository, "ProgramToBeTestedMock")

num_iterations = 3

for i in range(num_iterations):
    print("start")
    # Prepare data a
    prepare_data(spi_write_file="SPI_A.txt",
                 remote_directory=remote_repository,
                 time_out=10,
                 sleep_time=1)

    # Prepare expected data b
    prepare_data_b(spi_write_file, spi_read_file)

    run_assembly()
    time.sleep(0.5)

    # Continuously send data and expect read data
    while True:
        # Check if SPI_A.txt is empty
        local_file_path = os.path.join(local_directory, spi_write_file)
        with open(local_file_path, 'r') as file:
            if not file.read(1):
                os.remove(local_file_path)  # Remove the empty file
                break
        # Send data when asked
        send_data_when_asked(spi_write_file, signal_listener, data_supplier, local_directory)
        # Expect and compare received data with the data in spi_read_file
        expect(spi_read_file, data_listener, local_directory)
        print("\n" + "*************************")

    time.sleep(0.1)
    copy_spi_log_to_destination(remote_repository)

    # Check if the loop is about to exit before calling continue_process()
    if i != num_iterations - 1:
        continue_process(remote_repository)
        print("end")

    time.sleep(0.5)
    kill_assembly()
    time.sleep(0.5)
        

stop_process(remote_repository)
# Close the subscriber and publisher sockets
signal_listener.close()
data_listener.close()
data_supplier.close()

print("Test1 completed successfully!")
