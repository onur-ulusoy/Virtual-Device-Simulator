# Example Usages of Process Communication Interface with Publisher and Subscriber Classes

## Python
This example demonstrates how to use the `Publisher` and `Subscriber` classes from the `comm_interface` module in a Python script.

```python
from comm_interface import Publisher, Subscriber
import time
```
### Subscriber
```python
# Initialize the subscriber
subscriber = Subscriber("tcp://localhost:5555")
counter = 0

# Receive 5 messages from the publisher
while counter < 5:
    subscriber.receive()
    counter += 1
```
### Publisher

```python
# Initialize the publisher
local_address = "tcp://127.0.0.1:5555"
publisher = Publisher(local_address, "Python Script")
counter = 0

# Publish 5 messages
while counter < 5:
    time.sleep(2)
    message = "Python publisher"
    publisher.publish(message)
    counter += 1
```

## C++
This example demonstrates how to use the Publisher and Subscriber classes from the `CommInterface.hpp` in a C++ program.

```C++
#include "CommInterface.hpp"
#include <string>
#include <unistd.h>
```
### Subscriber
```C++
int main() {
    // Initialize the subscriber
    std::string localAddress = "tcp://127.0.0.1:5555";
    Subscriber subscriber(localAddress);
    counter = 0;

    // Receive 5 messages from the publisher
    while (counter < 5) {
        subscriber.receive();
        ++counter;
    }

    return 0;
}
```

### Publisher
```C++
int main() {
    // Initialize the publisher
    Publisher publisher("tcp://127.0.0.1:5555");
    int counter = 0;

    // Publish 5 messages
    while (counter < 5) {
        sleep(2);
        publisher.publish("C++ publisher from driver " + std::to_string(counter));
        ++counter;
    }
}
```




