# osquery-ping
An Osquery extension that adds a table called ping to the set of Standard Osquery tables. You can returns the latency encountered when to ping 
a host or network device. It displays the latency in milliseconds of the round 
trip ICMP call to the domain or host as shown below:

## Running Release Binaries
* Download the binary in the Release tab for this repository
* Step 2
* 
## Building From Source
* Follow the instructions to install `osquery`
* To run testing ensure `osquery`, `gtest`, `gflags`,`gmock` and `boost` are in your includes path which is assumed to be `/usr/local/include`. If they are in a different path, you can adjust the `CMakeLists.txt` `include_directories` call
* Execute `cmake .` at the root of the repository

## Troubleshooting
* Step 1
* Step 2

## Design Consideration

* TODO: Will be ideal to run concurrent ping commands if more than one host is provided for example in the case like:
```sql
select latency from ping where host in ('apple.com', '127.0.0.1', 'localhost')
```
* The Shell class to execute the ping command on terminal was created to make it easier to write unit tests without executing the actual ping
* Ideally, the following calls should be private to the table but for the same of testing; I made them public
```c++
virtual std::string pingHost(std::string host);
virtual std::vector<double> parsePingOutputForLatencies(std::string output);
```
