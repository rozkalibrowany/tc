# Telematics Connector - service for IoT Devices Connectivity
![alt text](https://i.ibb.co/hgtM5tt/Screenshot-from-2022-06-22-20-52-34.png)

# Stack
 - C++17 
 - Tools (CMake, Ansible, Conan)
 - Cppserver 3rd party library based on ASIO

# Bulding (Linux)
* mkdir build-conan && cd build-conan
* cmake ../conan && make conan-build
* cd ../ && mkdir build && cd build
* cmake ../ && make

# Targets
* tc-telematics-client
* tc-telematics-server
* tc-vehicle-connector-server

# Example requests
Configure tc-vehicle-connector-server as localhost
```bask
* GET
curl -X GET -kvH "Accept: application/json" https://127.0.0.1:8443/devices
curl -X GET -kvH "Accept: application/json" https://127.0.0.1:8443/device/<IMEI/ID>
```

```bash
* POST
curl -X POST --cacert <cert> https://127.0.0.1:8443/device/<IMEI/ID>/unlock
curl -X POST --cacert <cert> https://127.0.0.1:8443/device/<IMEI/ID>/lock
curl -X POST --cacert <cert> https://127.0.0.1:8443/device/<IMEI/ID>/led_off
curl -X POST --cacert <cert> https://127.0.0.1:8443/device/<IMEI/ID>/led_on
curl -X POST --cacert <cert> https://127.0.0.1:8443/device/<IMEI/ID>/engine_off
curl -X POST --cacert <cert> https://127.0.0.1:8443/device/<IMEI/ID>/engine_on
```

```bash
* DELETE
curl -X DELETE --cacert <cert> https://127.0.0.1:8443/device/<IMEI/ID>
```
