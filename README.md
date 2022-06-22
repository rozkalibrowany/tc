# Telematics Connector - service for IoT Devices Connectivity
![alt text](https://ibb.co/vcKB5KK)

# Stack
 - C++17 
 - Tools (Python, Ansible, Conan)
 - CMake + GNU/GCC

# Bulding (Linux)
* mkdir build-conan && cd build-conan
* cmake ../conan && make conan-build
* cd ../ && mkdir build && cd build
* cmake ../ && make

# Targets
* tc-telematics-client
* tc-telematics-server
* tc-vehicle-connector-server
