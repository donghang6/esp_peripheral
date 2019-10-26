# How to compile C/C++ program in Virsual Studio Code Editor

---

## There are three steps must be done

- tasks.json

  - **`CTRL+SHIFT+P -> Tasks: Configure Task -> create tasks.json file from template -> Others`** to crete tasks.json

  - then we should write the code below in it

    ``` ymal
    "command": "make",
    "group": {
            "kind": "build",
            "isDefault": true
        },
    ```

    we use make tool to compile a program
- launch.json
  - **`Debug -> Add Configuration -> C++(GDB/LLDB)`** to create launch.json
  - the marked line must be modified according to your compure

    ``` ymal
    {
        "version": "0.2.0",
        "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
        *   "program": "${workspaceFolder}/build/hello.exe",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "gdb",
        *   "miDebuggerPath": "C:\\mingw64\\bin\\gdb.exe",
        *   "preLaunchTask": "echo",
            "setupCommands": [
            {
                "description": "Enable pretty-printing for gdb",
                "text": "-enable-pretty-printing",
                "ignoreFailures": true
            }
            ]
        }
        ]
    }
    ```

    the key-value pair of `"preLaunchTask": "echo"` should be put here, for the program must be compiled before debugging. The value echo if the label value in tasks.json.
- c_cpp_properties.json
  **`CTRL+SHIFT+P -> C/C++ Edit Configurations(JSON)`** to create **`c_cpp_properties.json`** file. In this file we must give the right **`includePath`** and **`compilerPath`**

---

## Now you can code with higher efficiency in your Visual Studio Code.
