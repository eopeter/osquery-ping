# osquery-ping
An Osquery extension that adds a virtual table called `ping` to the set of Standard Osquery tables. It returns the number of 
milliseconds measured between an ICMP request and the reply from the specified host. 


## Schema
### ping
| Column    | Type   | Description                                                                                         |
|-----------|--------|-----------------------------------------------------------------------------------------------------|
| latency   | NUMBER | The duration in milliseconds measured between an ICMP request and the reply from the specified host |
| host      | TEXT   | The host that the latency was measured for                                                          |


### Usage
```sql
osquery> select host, latency from ping where host = 'www.apple.com' OR host = '127.0.0.1';
```

The result of the above query will look like:
```
+---------------+---------+
| host          | latency |
+---------------+---------+
| 127.0.0.1     | 0.057   |
| www.apple.com | 28.782  |
+---------------+---------+
```

## Running From Compiled Binaries
* Download the extension in the [releases](https://github.com/eopeter/osquery-ping/releases) page for this repository
* Run `sudo osqueryi --extension /Users/<user>/Downloads/osquery_extension_ping.ext` from terminal
* Once `Osqueryi` starts, you should be able to execute the sample sql query above

## Building & Running From Source (Linux/ macOs)
* Follow the instructions to install, configure and build `osquery` at [readthedocs.io](https://osquery.readthedocs.io/en/latest/development/building/) and stop before the configure step.
* Clone this repository
* Symlink the `osquery-ping` folder into `osquery/external/extension_osquery-extension-ping` using the command
```
cd osquery
ln -s ../../osquery-ping external/extension_osquery-extension-ping
```
* `cd build`
* Resume following the instruction at [readthedocs.io](https://osquery.readthedocs.io/en/latest/development/building/)
* Note that Osquery build step above assumes building into `users/<user>/osquery`
* Run `sudo osqueryi --extension osquery/build/external/extension_ping/osquery_extension_ping.ext`

## Troubleshooting
* Use the `--verbose` flag when troubleshooting to see more details on what is going on
* If you encounter the following error `[Ref #1382] Extension binary has unsafe permissions: osquery/build/external/extension_osquery-extension-ping/osquery-extension-ping.ext`, run as sudo with `--allow-unsafe` flag with the `osqueryi` command.
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

* I had the option to either do real-time pings or find a source that shows the history of pings and filter by the specified host. Showing the history of pings will not be the right solution as it may not match the user's desire to find the current latency for the specified host at the current time.
* It will be possible to allow the user to see how latency has changed overtime by using a sqlite db to keep track of all ping requests. For the current ask, that may be an overkill.
* I also considered querying the `process_events` table in Osquery and using the `cmdline` column to determine if ping ws executed but that in itself does not give me the response at the time of execution.
* I thought about the possibility of preventing sql injection as the user sends the query; I attempted to sanitize the host name that was received from the user but the way the `QueryContext` constraint `getAll` function extracts values from the query will protect against it but the actual text passed to the where statement needs to be sanitized.
* The Shell class to execute the ping command on terminal was created to make it easier to write unit tests without executing the actual ping. A mock of the Shell interface is used to test the functionality
* I need to test the following private methods of the plugin and since private methods cannot be accessed outside of the class except by friends and subclasses of the plugin. I had to add the `FRIEND_TEST` fixture to the test class to allow writing unit tests for the private methods
```c++
table.h
--
virtual std::bool isValidHost(const std::string& input);
virtual std::string pingHost(std::string host);
virtual std::vector<double> parsePingOutputForLatencies(std::string output);
```
* I thought about how to efficiently to parse the ping response from the shell to extract the latency; I had the option to either use regular expression or string manipulation by iterating over the output lines and tokenizing the string. String tokenization could be more efficient than regex matching. I decided to go with regex due to how small our input string is and for code clarity.

## Roadmap
* Will be ideal to run concurrent ping commands if more than one host is provided for example in the case like:
```sql
osquery> select latency from ping where host in ('apple.com', '127.0.0.1', 'localhost')
```
* Add support for Linux and Windows
* Add additional columns such as the time the command was issued and the time the response was received. This could be used to correlate the latency value that was returned
