# VS code  clangd and lldb setup

https://clangd.llvm.org/

**clangd understands your C++ code and adds smart features to your editor: code completion, compile errors, go-to-definition and more.**

https://lldb.llvm.org/

**LLDB is a next generation, high-performance debugger. **



## Install llvm packages

https://apt.llvm.org/

```bash
# auto install the lastst stable version  (now is version 10)
sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
# install the clangd 
sudo apt install  clang-tools-10
```

#### version will change over time!!!

## VScode setup

### 1.install the following plugin:

**clangd** : C and C++ completion, navigation, and insights

**CodeLLDB**: Native debugger based on LLDB.

#### setup clangd in settings

search `clangd` in settings, find `clangd:path` option

set it to your current clang version, `clangd-10` 

### 2.add compile flag

```bash
# for cmake project
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 
# for ros project
catkin_make  -DCMAKE_EXPORT_COMPILE_COMMANDS=1
# always add flag when you add new files
```

if your folder structure opened in vscode  is like this  

../

​	build

​	src

the ` compole_commands.json `  maybe in the build folder

### 3.link the` compole_commands.json ` file

open terminal in the workspace 

```bash
ln -s build/compile_commands.json
# if your build fold is in other place, change it to yours
```

Reopen the workspace and clangd will get to work

### 4.LLDB Debug setting

set clang as the default compiler and open debug  option 

```cmake
set(CMAKE_CXX_COMPILER "clang++-10") # change version if needed
set(CMAKE_C_COMPILER "clang-10")
set(CMAKE_BUILD_TYPE "Debug") # remove it will build release version 
```

click the `gear` icon on the debug panel and open the `lanch.json` file, or click `Add configuration` directly.

Required information:

**name** : your debug section name

**program** : where your program is 

**terminal** : use integrated or external terminal

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "lldb",
            "request": "launch",
            "name": "Debug_node1",
            "program": "${workspaceFolder}/devel/lib/example/example_node",
            "args": [],
            "cwd": "${workspaceFolder}",
            "terminal": "integrated"
        },
        {
            "type": "lldb",
            "request": "launch",
            "name": "timeTest1",
            "program": "${workspaceFolder}/devel/lib/example/example_clockTest1",
            "args": [],
            "cwd": "${workspaceFolder}",
            "terminal": "integrated"
        }
    ]
}
```

usually you can find the program location in the compiler output `Linking CXX executable` 

```bash
[ 39%] Built target example_clockTest1
[ 58%] Built target ubt_core_msgs_generate_messages_eus
[ 90%] Built target ubt_core_msgs_generate_messages_nodejs
[ 94%] Built target ubt_core_msgs_generate_messages_cpp
Scanning dependencies of target example_urdfTest
[ 94%] Built target ubt_core_msgs_generate_messages
[ 96%] Built target example_node
[ 96%] Building CXX object example/CMakeFiles/example_urdfTest.dir/src/urdfTest.cpp.o
[ 96%] Linking CXX executable /home/lin/code/webots_project/ubt/WalkerSimulationFor2020WAIC/walker_WAIC_18.04_v1.2_20200616/ubt_sim_ws/devel/lib/example/example_urdfTest
[100%] Built target example_urdfTest
```

