This is flash, a super fast BSE solver.

# Install:
To build **Flash** with debug flags, release flags and the documentation type in the root directory
```bash
make 
```
You can seperately build **Flash** with debug flags, release flags or the documentation seperately by typing, respectively
```bash
make debug
make release
make documentation
```
If you wish to make build the documentation, make sure that doxygen is installed.

## Run unit tests:
Run all tests:
```
make test
```
Run debug or release build tests
```bash
make test-debug
make test-release
```

## Lint and auto-format the code:
To run first **clang-tidy** and then **clang-format** for all files, type
```bash
make linter

```
To run **clang-tidy** or then**clang-format**, type, respectively
```bash
make clang-tidy
make clang-format
```

## Requirements:  
  For building:
    - CMake > 
    - Eigen
    - HDF5
    - cpp-yaml
    - catch2
    - boost
  For the documentation
    - doxygen
  For linting and auto formatting:
    - clang-tidy
    - clang-format
  

    