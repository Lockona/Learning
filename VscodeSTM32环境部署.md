# 一、软件

​	 [VS Code](https://code.visualstudio.com/);

​	 [LLVM](http://releases.llvm.org/download.html);

​	 [Git for Windows](https://gitforwindows.org/);

​	 [arm-none-eabi-gcc](https://launchpad.net/gcc-arm-embedded/+download);

​	[OpenOCD for Windows](http://gnutoolchains.com/arm-eabi/openocd/);

​	[STM32CubeMX](http://www.stm32cube.com/);

# 二、VSCode插件

![image-20210201141534534](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201141534534.png)

![image-20210201141555979](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201141555979.png)

# 三、配置Git为VS Code内置终端

![image-20210201141903902](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201141903902.png)

![image-20210201142258583](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201142258583.png)

```json
/* 终端在Windows上使用的shell的路径 */
"terminal.integrated.shell.windows": "C:\\Program Files\\Git\\bin\\bash.exe",
"terminal.external.windowsExec": "C:\\Program Files\\Git\\bin\\bash.exe"

```
# 四、配置智能补全、智能感知插件LLVM

<!--在当前目录的.vscode文件夹下创建c_cpp_properties.json配置文件，用来告诉VS Code我们定义的宏与文件的路径。-->



```json
{
    "configurations": [
        {
            "name": "Win32",
            "browse": {
                "path": [
                    "${workspaceFolder}/",
                    "${workspaceFolder}/Drivers/CMSIS/Device/ST/STM32F4xx/Include",
                    "${workspaceFolder}/Drivers/CMSIS/Include",
                    "${workspaceFolder}/Drivers/STM32F4xx_HAL_Driver/Inc",
                    "${workspaceFolder}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy",
                    "${workspaceFolder}/Inc"
                ],
                "limitSymbolsToIncludedHeaders": true
            },
            "includePath": [
                "${workspaceFolder}/",
                "${workspaceFolder}/Drivers/CMSIS/Device/ST/STM32F4xx/Include",
                "${workspaceFolder}/Drivers/CMSIS/Include",
                "${workspaceFolder}/Drivers/STM32F4xx_HAL_Driver/Inc",
                "${workspaceFolder}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy",
                "${workspaceFolder}/Inc"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE",
                "__CC_ARM",
                "USE_STDPERIPH_DRIVER",
                "STM32F40_41xxx"
            ],
            "compilerPath": "C:\\Program Files\\LLVM\\bin\\clang.exe",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "intelliSenseMode": "clang-x64"
        }
    ],
    "version": 4
}

```

<!--`注意：如果提示variable “uint32_t” is not a type name不是一个type类型。-->
<!--则需要 添加宏定义__CC_ARM，如果没有该宏定义，则uint32_t类型会报错。并且结构体中使用了 uint32_t定义的成员，也会补全不了。-->

出现未定义字符的报错

![image-20210201160006594](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201160006594.png)

# 五、用STM32Cube创建工程

![image-20210201160419223](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201160419223.png)

# 六、编译程序

##### 在Terminal下输入make指令，即会开始build程序

<!--需要添加make.exe这个软件，可以用Qt下自带的mingw32-make.exe重命名为make.exe,之后需要将其所在路径添加到系统环境变量中，由于环境变量里有Git的路径，所以我将其复制到git的cmd目录里-->

在系统环境变量里要有一下路径

![image-20210201161922490](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201161922490.png)



编译成功的结果

![image-20210201162304524](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201162304524.png)

# 七、下载调试

需要在环境变量中添加

![image-20210201164031918](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201164031918.png)

在当前工程下创建一个名为openocd.cfg的文件

\#选择st-link

source [find interface/stlink-v2.cfg]

```
#选择SWD接口

\# transport select swd



\#选择目标芯片

source [find target/stm32f4x.cfg]
```



在终端输入openocd

![image-20210201170706478](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201170706478.png)

新开一个终端输入

```
arm-none-eabi-gdb build/STM32F4_PWM_HAL.elf 
```



![image-20210201171031423](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201171031423.png)

输入

```
	target remote localhost:3333 
```

连接上openocd(openocd留给gdb的TCP/IP端口是3333)

![image-20210201172428550](C:\Users\Kero\AppData\Roaming\Typora\typora-user-images\image-20210201172428550.png)

连接上openocd返回的信息里有？？之类的也是正常的

# 八、配置openocd任务与build任务

在工作区下创建build.py

```python
import subprocess

import os


if __name__= '__main__':

  subprocess.run(["echo","building···"])

  subprocess.run(["make","clean"])

  subprocess.run(["make"])
```



按 F1，选择"配置任务" .vscode目录下就会创建一个tasks.json配置文件,修改为如下代码

```json
{

  "version": "2.0.0",

  "tasks": [

    {

      "label": "build",

      "type": "shell",

      "command": "python",

      "args": [

        "build.py"

      ]

    },

   {

     "label": "openocd",

     "type": "shell",

     "command": "openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg",

    }

  ]

}
```

# 九、配置VsCode的调试功能

在VS Code内选择debug（就是左边那个虫子图标），选择“添加配置”，类型为GDB。就会在.vscode文件夹下生成launch.json配置文件。
按照下面来配置：

```json
{

  // 使用 IntelliSense 了解相关属性。 

  // 悬停以查看现有属性的描述。

  // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387

  "version": "0.2.0",

  "configurations": [



​    {

​      "name": "ARM Debug",

​      "type": "cppdbg",

​      "request": "launch",

​      "program": "${workspaceRoot}/build/${workspaceFolderBasename}.elf",

​      "args": [],

​      "stopAtEntry": false,

​      "cwd": "${workspaceFolder}",

​      "environment": [],

​      "externalConsole": false,

​      "MIMode": "gdb",

​      "miDebuggerPath": "C:\\Program Files (x86)\\GNU Tools ARM Embedded\\5.4 2016q3\\bin\\arm-none-eabi-gdb.exe",//调试工具原始路径

​      "targetArchitecture": "arm", //目标架构，此参数必须要有

​      "setupCommands": [// 进入GDB以后，自动执行的一些配置 

​        {

​          "description": "选择调试文件(.elf)到gdb",

​          "text": "file D:/STM32/Learing/STM32F4_PWM_HAL/build/STM32F4_PWM_HAL.elf",

​          "ignoreFailures": false

​        },

​        {

​          "description": "连接GDB Server",

​          "text": "target remote localhost:3333",                 

​          "ignoreFailures": false

​        },

​        {

​          "description": "Reset MCU",

​          "text": "monitor reset",                       

​          "ignoreFailures": false

​        },

​        {

​          "description": "Halt",

​          "text": "monitor halt",                         

​          "ignoreFailures": false

​        },

​        {

​          "description":"下载代码到MCU",

​          "text": "load" ,       

​          "ignoreFailures": false                   

​        }

​      ],

​      "preLaunchTask": "build", // 在调试前预先执行的任务，此处是tasks.json中的

​    }

  ]

}
```



<!--在进行调试的时候，需要先 按 F1 -> 任务：运行任务里选择 openocd 任务运行。-->