# redblacktree-cpp
A implementation of the RedBlackTree from Cormen et. al. in C++

# building the project
This project uses `CMake` to generate the `Makefile` used to actually build the project.
To generate the makefile use
```
cmake .
```
Within the project root (look for the `CMakeLists.txt` file). This will generate a makefile `Makefile`
in the same directory.

## targets
The generated makefile has several build targets that can be run:
- `clean`: Cleans the project directory of building artifacts
- `tests`(default): Builds the testing executable in the `bin` folder
- `format`: Runs the `astyle` code formatter on the source files
