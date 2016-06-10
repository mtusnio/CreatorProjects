## Temperature report

### Requirements

Requires Python 2.7 to be built with OpenWRT (Languages->Python->python).

Thermo3 click to be plugged into Mikrobus 1

### Usage

python server.py

Then run python server.py and access the page at local_ip:8080/temperature. Every refresh the page will report temperature from the click. Additionally, the script acts as an HTTP server serving any pages present in the directory.
