# test-fnlib

Tests apple2 with fujinet-lib new functions.

This requires locally compiled fujinet-lib, as it uses non-release version to test.

```bash
mkdir fujinet-lib
cp ../../fujinet-lib/dist/fujinet-lib-apple2-2.2.2.zip fujinet-lib/ && cd fujinet-lib && rm -rf 2.2.* && unzip -o fujinet-lib-apple2-2.2.2.zip -d 2.2.2 && cd ..
make clean all po gendebug

# and copy the test file to tnfs for loading into real A2 or AppleWin
cp dist/acx.po /path/to/your/tnfs
```
