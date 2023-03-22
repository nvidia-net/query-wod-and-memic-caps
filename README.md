# Query-WOD-MEMIC-Cap
A library that queries device HW capabilities.
It's main purpose it to show if MEMIC & WOD are enabled on the device.

As a sanity check, the repo provides a test that calls the library and prints on the CLI the capabilities.

# Build with unit-test
```
cd <root-dir>
mkdir build
cd build
cmake -DBUILD_TEST=ON ..
make
```

# Build without unit-test
```
cd <root-dir>
mkdir build
cd build
cmake -DBUILD_TEST=OFF ..
make
```

# Run (you need to build with unit-tests)
```
./query_wod_memic_cap_test
```
