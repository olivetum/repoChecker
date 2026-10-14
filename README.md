# repoChecker
<a href="https://www.aihonestybadge.com" target="_blank" rel="noopener"><img src="https://www.aihonestybadge.com/badges/no-ai-small.svg" alt="No AI" height="20"></a>*

*AI was only used to generate this README file

---

Lists a GitHub user's repositories. Requires a C++20 compiler and CMake 4.3 or
newer. The first CMake configure downloads CPR, curl, and nlohmann/json and their
dependencies, so it needs internet access.

## Build and run with CMake

From the project directory:

```sh
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug --target repoChecker --parallel
./cmake-build-debug/repoChecker olivetum
```

In CLion, select the `repoChecker` CMake target and put your GitHub username in
the run configuration's program arguments.

## Compile directly with Apple Clang

First build the dependencies with the CMake commands above. Then:

```sh
clang++ --config=./clang.cfg main.cpp -o repoChecker
./repoChecker olivetum
```

`clang.cfg` supplies C++20, dependency include paths, linker flags, and runtime
library paths for the existing `cmake-build-debug` directory. Pass it explicitly;
`clang++ main.cpp` does not automatically load this project's configuration.
The executable still needs the libraries in that build directory at runtime.
Use CMake for other compilers or build directories.

## Check startup and request failures

```sh
python3 tests/test_startup.py cmake-build-debug/repoChecker
```

These checks need no internet connection. Normal repository queries require
access to `api.github.com`.
