# osquery-ping
An Osquery extension that adds a table called ping to the set of Standard Osquery tables. You can returns the latency encountered when to ping 
a host or network device. It displays the latency in milliseconds of the round 
trip ICMP call to the domain or host as shown below:

## Schema
### ping
| Column    | Type   | Description                                                                                         |
|-----------|--------|-----------------------------------------------------------------------------------------------------|
| latency   | NUMBER | The duration in milliseconds measured between an ICMP request and the reply from the specified host |
| host      | TEXT   | The host that the latency was measured for                                                          |


### Sample Query
```sql
SELECT latency FROM ping WHERE host = '127.0.0.1' OR host = 'www.apple.com'
```
## Running From Compiled Binaries
* Download the binary in the [releases](https://github.com/eopeter/osquery-ping/releases) page for this repository
* Run `sudo osqueryi --extension osquery/build/external/extension_ping/osquery_extension_ping.ext --nodisable_extensions`
* If you do not have sudo, you can run with the `--allow_unsafe` flag

## Building From Source (Linux/ macOS)
* Follow the instructions to install, configure and build `osquery` at [readthedocs.io](https://osquery.readthedocs.io/en/latest/development/building/) and stop before the configure step.
* Clone this repository
* Symlink the `osquery-ping` folder into `osquery/external/extension_ping` using the command
```
cd osquery
ln -s ../../osquery-ping external/extension_ping
```
* `cd build`
* Resume following the instruction at [readthedocs.io](https://osquery.readthedocs.io/en/latest/development/building/)
* Note that Osquery build step above assumes building into `users/<user>/osquery`
* To make changes, build from an IDE or tests, ensure `osquery`, `gtest`, `gflags`,`gmock` and `boost` are in your includes path which is assumed to be `/usr/local/include`. If they are in a different path, you can adjust the `CMakeLists.txt` `include_directories` call
* Run `sudo osqueryi --extension osquery/build/external/extension_ping/osquery_extension_ping.ext --nodisable_extensions`
* If you do not have sudo, you can run with the `--allow_unsafe` flag

## Troubleshooting
* Use the `--verbose` flag when troubleshooting to see more details on what is going on
* If you encounter the following error, run as sudo or use the `--allow-unsafe` flag with the `osqueryi` command. `Extension binary has unsafe permissions:1`
* If your query does not work against the table, make sure you have the correct socket for `osqueryi` by running the following command `select path from osquery_extensions` once your start `osqueryi`. The path in the result should be passed to the `--socket` flag when attaching your extention
```sql
osquery> select path from osquery_extensions;
+-------------------------------------------------------------------------+
| path                                                                    |
+-------------------------------------------------------------------------+
| /var/folders/w5/pvflyg8942d4041xjq5vxdk4001slv/T/osquery-25846/shell.em |
+-------------------------------------------------------------------------+
```

## Design Consideration

* The Shell class to execute the ping command on terminal was created to make it easier to write unit tests without executing the actual ping
* Ideally, the following calls should be private to the table but for the same of testing; I made them public
```c++
virtual std::string pingHost(std::string host);
virtual std::vector<double> parsePingOutputForLatencies(std::string output);
```

## Roadmap
* Will be ideal to run concurrent ping commands if more than one host is provided for example in the case like:
```sql
select latency from ping where host in ('apple.com', '127.0.0.1', 'localhost')
```
* Add support for Linux and Windows
* Add additional columns such as the time the command was issued and the time the response was received. This could be used to correlate the latency value that was returned
