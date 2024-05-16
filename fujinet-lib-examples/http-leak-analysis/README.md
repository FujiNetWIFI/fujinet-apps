# http-leak-analysis

Because the application sends requests to TCP and TNFS servers that need to be running, you will have to specify those on the make command line if running those tests.

## Creating test data file

The TNFS test requires a file of 512 bytes, and a path to it in TNFS_FILE_PATH.

To create the test file of 512 bytes, you can use `dd` like this:

```shell
dd if=/dev/urandom of=/path/to/tnfs-share/your-file.dat bs=512 count=1
```

## Building

Now you can build the app with:

```shell
# using default values (which is Thom's machine!)
make clean all disk

# specifying the values as you build. Note: yes you need 3x backslash per quote.
# the default port is 6502 if you use that, you don't need to set TCP_SERVER_PORT
make clean all disk TCP_SERVER_ADDRESS=\\\"192.168.1.100\\\" TCP_SERVER_PORT=\\\"16384\\\" TNFS_FILE_PATH=\\\"192.168.1.100/your-file.dat\\\"
```

The `disk` task will create the file `dist/leak.po` which you can deploy and run on your FN as normal.

## Starting a netcat listener for the test

```
netcat -l localhost 6502
```

## Running the application via Altirra

Setup your altirra with bridge to fujinet-pc, details should be in the `fujinet-emulator-bridge` project.

Run the application (which will start Altirra) with:

```shell
make TARGETS=atari clean all test TCP_SERVER_ADDRESS=\\\"192.168.1.100\\\" TCP_SERVER_PORT=\\\"16384\\\" TNFS_FILE_PATH=\\\"192.168.1.100/your-file.dat\\\"
```
