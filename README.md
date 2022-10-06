# Telematics Connector - service for IoT Devices Connectivity
![alt text](https://i.ibb.co/hgtM5tt/Screenshot-from-2022-06-22-20-52-34.png)

# Integrated hardware API
 - Teltonika Codec 8/12

# Stack
 - C++17
 - Tools (CMake, Ansible, Conan)
 - Cppserver 3rd party library based on ASIO
 - MongoDB to backup telemetry (possible to use only RAM Cache)

# Service config files
tc-telematics-server:
```
[server]
address=127.0.0.1
port=8881

[session]
cache=1000

[db]
enabled=true
name=cluster0
collection_packets=Packets
collection_devices=Devices
uri=mongodb+srv://login:pass@cluster.12345.mongodb.net
```

tc-vehicle-connector-server:
```
[tcp]
address=127.0.0.1
port=8881
interval=10000

[http]
port=8443
```

# Ansible env setup
```
ws-ansible wsr dev
ws-ansible wsr tc-telematics-server
ws-ansible wsr tc-vehicle-connector-server
```
# Bulding (Linux)
GCC/G++ 10 required
```
mkdir build-conan && cd build-conan
cmake ../conan && make conan-build
cd ../ && mkdir build && cd build
cmake ../ && make
```

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
* POST (device)
curl -X POST --data '{}' https://127.0.0.1:8443/device/<IMEI/ID>/unlock
curl -X POST --data '{}' https://127.0.0.1:8443/device/<IMEI/ID>/lock
curl -X POST --data '{}' https://127.0.0.1:8443/device/<IMEI/ID>/led_off
curl -X POST --data '{}' https://127.0.0.1:8443/device/<IMEI/ID>/led_on
curl -X POST --data '{}' https://127.0.0.1:8443/device/<IMEI/ID>/engine_off
curl -X POST --data '{}' https://127.0.0.1:8443/device/<IMEI/ID>/engine_on

* POST (settings)
curl -X POST --data '{}' https://127.0.0.1:8443/device/<IMEI>/set?id=123456
```

```bash
* DELETE
curl -X DELETE --cacert <cert> https://127.0.0.1:8443/device/<IMEI/ID>
```
