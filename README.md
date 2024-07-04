# ![icon](install_script\icon.ico) NyarukoMiniTools

- 存放一些超小的 Windows 程序的仓库。
- 这些工具全都是 C 语言编写。
- 通过这些新增的工具命令，可以让 bat 批处理变得更方便。

- A repository for storing some very small Windows programs.
- These tools are all written in C language.
- These newly added tool commands can make bat batch processing more convenient.

## 应用场景示例

我在平时使用 Windows 制作一些 bat 批处理脚本时，会遇到需要使用某些系统没有提供的命令。因此创建了本仓库，存储一些实用工具。例如：

- bat 中获取时间值会因区域设置而遇到兼容性问题，单独获取某个时间或日期会麻烦，而 `datetime.exe "yyyy-MM-dd HH:mm:ss"` 解决了它。例如获得现在的分钟数只需要 `datetime.exe mm` 。
- 有一个软件中的 exe 组件我不想让其发挥功能，而又不想引发权限之类的错误（例如替换软件中的广告弹窗程序），用 `null.exe` 替换它，它就什么功能都没有了，并且系统视为正常执行。
- 我想生成一个随机密码，但 Windows 上没有 `pwgen` 命令，那么生成 1 个 64 位密码就用 `pwgen.exe 64 1` 吧。它还支持硬件随机数生成器。
- 我想开发一个和控制台程序交互的 GUI 软件，输入什么就回复什么的 `reflex.exe` 是一个好办法。它还支持持续输出时间模式 `reflex.exe /T` 。
- 我有一个路径 `\dir\path\` 想转换为 `/dir/path/` ，`repstr.exe \dir\path\ \ /` 即可搞定。可以用来快速替换各种英文字符串。
- 我想等几秒再继续运行下一条命令，但 Windows 上没有 `sleep` 命令，而 `timeout` 在旧版本上没有并且会输出多余信息，那么就用 `sleep.exe` 吧。它还支持高精度模式（时间同步）和标注时间单位，例如等待一分钟用 `sleep.exe 1m` 即可，你也可以加上 `/D` 显示倒计时。
- 我想一键息屏，双击启动 `scrctl.exe` 即可。配合 `sleep.exe` 命令加个延迟可以防止刚运行后误滑鼠标而中止。
- 我运行一个命令，又想实时看到它的输出，又想实时记录到日志文件，但 Windows 上没有 `tee` 命令，那么就用 `要执行的命令 2>&1 | tee.exe 日志文件.log` 吧。
- 我和我的朋友约好，只要在这个小时或者这天，传输的东西都用 `tspwd.exe` 来生成密码，我们就再也不需要互相告诉对方密码了，它还是个动态密码。只需事先约定好预定义“盐”。或者从公司电脑复制到U盘，回家再移动到电脑的临时传输文件的场合，也可以使用它的动态密码来临时加密。

## Scenario Example

When I usually use Windows to make some bat batch scripts, I will encounter the need to use some commands that are not provided by the system. Therefore, this repository was created to store some practical tools. For example:

- Obtaining time values ​​in bat will encounter compatibility issues due to regional settings. It is troublesome to obtain a certain time or date separately, and `datetime.exe "yyyy-MM-dd HH:mm:ss"` solves it. For example, to get the minute now, just use `datetime.exe mm`.
- There is an exe component in the software that I don't want to function, but I don't want to cause permissions errors (for example, replacing the ad pop-up program in the software). Replace it with `null.exe`, it will have no function, and the system will treat it as normal execution.
- I want to generate a random password, but there is no `pwgen` command on Windows, so use `pwgen.exe 64 1` ​​to generate a 64-bit password. It also supports hardware random number generators.
- I want to develop a GUI software that interacts with console programs. `reflex.exe` is a good way to reply to what you enter. It also supports continuous output time mode `reflex.exe /T` .
- I have a path `\dir\path\` that I want to convert to `/dir/path/`, and `repstr.exe \dir\path\ \ /` can do it. It can be used to quickly replace various English strings.
- I want to wait a few seconds before continuing to run the next command, but there is no `sleep` command on Windows, and `timeout` does not exist on old versions and will output redundant information, so use `sleep.exe`. It also supports high-precision mode (time synchronization) and time units. For example, to wait for one minute, use `sleep.exe 1m`. You can also add `/D` to display the countdown.
- I want to turn off the screen with one click, and double-click to start `scrctl.exe`. Adding a delay with the `sleep.exe` command can prevent the program from being terminated by accidentally sliding the mouse just after it is run.
- I run a command and want to see its output in real time and record it to a log file in real time, but there is no `tee` command on Windows, so use `command 2>&1 | tee.exe output.log`.
- My friend and I agreed that as long as we use `tspwd.exe` to generate passwords for everything we transmit during this hour or this day, we will no longer need to tell each other the password, and it is still a dynamic password. Just agree on a predefined "salt" in advance. Or when you need to temporarily transfer files by copying them from a company computer to a USB drive and then moving them to your computer when you get home, you can also use its dynamic password to temporarily encrypt them.

## 安裝

1. 从 `Release` 中下载最新编译版本。
2. 提取与当前系统对应的架构(x86/x64)版本文件，将提取出的所有 `.exe` 文件放在同一个文件夹中。

如果需要能在 cmd 中随时作为命令调用，可以选择一下其中之一：

- 将本程序集中的所有 `.exe` 拷贝到 `%SystemRoot%\system32\` 文件夹中。
- 将本程序集所在的文件夹添加到 `path` 环境变量。

## Install

1. Download the latest compiled version from `Release`.
2. Extract the architecture (x86/x64) version file corresponding to the current system, and put all the extracted `.exe` files in the same folder.

If you need to call it as a command in cmd at any time, you can choose one of the following:

- Copy all `.exe` in this assembly to the `%SystemRoot%\system32\` folder.
- Add the folder where this assembly is located to the `path` environment variable.

## 卸载 Uninstall

和安装操作反向操作即可。

Just follow the reverse installation steps.

## ![sleep](sleep/icon1.ico) sleep.exe

- 功能类似于 sleep: 暂停一定时间。
  - 补充 Windows 中缺失的批处理中常用的延时命令 `sleep` 。不再需要 ping 等命令作延迟替代了。
  - 支持单位（秒、分、时、天）。
  - 支持多参数相加。
  - 支持显示倒计时。

- sleep-like: Pause for a certain amount of time.
  - Supplement the missing delay command `sleep` commonly used in batch processing in Windows. No longer need to use commands such as ping as a delay substitute.
  - Support units (seconds, minutes, hours, days).
  - Support multiple parameters to add.
  - Support countdown display.

### sleep:使用

- 使用: `SLEEP [选项] 数字[后缀]`...
- 暂停 `数字` 秒。
- 后缀可以是：
  - `s` 是秒钟 (the default),
  - `m` 是分钟,
  - `h` 是小时,
  - `d` 是天。
- `数字` 支持小数。
  - `数字` 的默认值的 1 秒
  - 如果给了 1 个以上的时间参数，暂停的时间是它们加在一起。
- `选项`:
  - `/D` : 每秒输出剩余时间（秒）
  - `/H` : 使用高精度计时器（更消耗 CPU），不建议和 `/D` 同时使用。
  - `/?` : 显示此帮助信息
  - `/V` : 显示版本信息

### sleep:Usage

- Usage: `SLEEP NUMBER[SUFFIX]`...
- or: `SLEEP OPTION`
- Pause for `NUMBER` seconds.
- SUFFIX may be
  - `s` for seconds (the default),
  - `m` for minutes,
  - `h` for hours,
  - `d` for days.
- Here `NUMBER` may be an arbitrary floating point number.
  - DEFAULT: 1s
  - Given two or more arguments, pause for the amount of time specified by the sum of their values.
- `OPTION`:
  - `/D` : display the remaining time (seconds) every second
  - `/H` : Use high-precision timer (more CPU intensive). Not recommended to use with `/D`.
  - `/?` : display this help and exit
  - `/V` : output version information and exit

## ![datetime](datetime/icon1.ico) datetime.exe

更方便地在批处理中获取需要的格式时间

- 输出时间日期
- Get time and date

### datetime:使用

- 使用: `datetime 模式 [时间]`
- 模式:
  - 支持的模式: `yyyy,yy,MM,dd,HH,hh,mm,ss`
    - （4位年，2位年，月，日，24小时时，12小时时，分，秒）
  - 不支持模式: `y,M,d,H,h,m,s,ddd,dddd,MMM,MMMM,gg ...`
- 时间:
  - `gmt` 标准时间 或者 `local` 本地时间 (默认值)
- 演示:
  - `datetime "yyyy-MM-dd HH:mm:ss"`
  - `->  2021-02-01 20:21:11`
- `datetime /?` : 显示此帮助信息
- `datetime /V` : 显示版本信息

### datetime:Usage

- Usage: `datetime MODE [TIME]`
- MODE:
  - Support:   `yyyy,yy,MM,dd,HH,hh,mm,ss`
  - Unsupport: `YYYY,y,M,d,H,h,m,s,ddd,dddd,MMM,MMMM,gg ...`
- TIME:
  - `gmt` or `local` (the default)
- DEMO:
  - `datetime "yyyy-MM-dd HH:mm:ss"`
  - `->  2021-02-01 20:21:11`
- `datetime /?` : display this help and exit
- `datetime /V` : output version information and exit

## ![repstr](repstr/icon1.ico) repstr.exe

- 替换英文字符串
- String substitution

### repstr:使用

- 使用: `repstr [模式] [原字符串] [要替换的字符串] [替换为]`
  - 所有字符串只允许英文和数字
- 模式:
  - `/c` : 忽略大小写
  - `/?` : 显示此帮助信息
  - `/V` : 显示版本信息
- 演示:
  - `repstr info\txt\readme.txt \ /` -> `info/txt/readme.txt`

### repstr:Usage

- Usage: `repstr [MODE] [String] [Find what] [Replace with]`
  - `String`: English only.
- MODE supported:
  - `/c` : ignore case
  - `/?` : display this help and exit
  - `/V` : output version information and exit
- DEMO:
  - `repstr info\txt\readme.txt \ /`

## ![scrctl](scrctl/icon1.ico) scrctl.exe

方便地快速关闭显示器和锁定系统

- 屏幕开关控制
- Monitor Power Control

### scrctl:使用

- 使用: `scrctl [模式]`
- 模式:
  - `off` 或 `/0` 关闭显示器 (default)
  - `on` 或 `/1` 激活显示器
  - `eco` 或 `/2` 节电模式
  - `lock` 或 `/3` 关闭显示器并锁定系统
  - `/?` 显示此帮助信息
  - `/V` 显示版本信息

### scrctl:Usage

- Usage: `scrctl [MODE]`
- MODE:
  - `off` or `/0` Power Off (default)
  - `on` or `/1` Power On
  - `eco` or `/2` Low Power Mode
  - `lock` or `/3` Power Off and Lock
  - `/?` display this help and exit
  - `/V` output version information and exit

## ![pwgen](pwgen/icon1.ico) pwgen.exe

pwgen-like:功能类似于 pwgen: 快捷生成由随机字符组成的密码

- 随机密码生成器
- Password Generator

### pwgen:使用

- 使用:   `pwgen [要包含的字符] [单个密码长度] [要生成的数量]`
  - 或者 `[单个密码长度] [要生成的数量]`
  - 或者 `[单个密码长度]`
- 要包含的字符 包括:
  - `A` : 大写字母
  - `a` : 小写字母
  - `n` : 数字
  - `s` : 标点符号
  - `/?`: 显示此帮助信息
  - `/V`: 显示版本信息
- 命令示例: `pwgen Aan 32 1` (生成1个32位的由大小写字母和数字组成的密码)

### pwgen:Usage

- Usage: `pwgen [MODE] [pw_length] [num_pw]`
  - or  `[pw_length] [num_pw]`
  - or  `[pw_length]`
- MODE supported:
  - `A` : Upper case
  - `a` : Lower case
  - `n` : Number
  - `s` : Symbol
  - `/?`: display this help and exit
  - `/V`: output version information and exit
- Demo: `pwgen Aan 32 1`

## ![null](null/icon1.ico) null.exe

程序直接退出自己，用于将普通 exe 替换来禁用其原有功能。例如替代软件中的广告弹窗 exe 。

- 该程序不做任何事情
- does nothing

## ![reflex](reflex/icon1.ico) reflex.exe

- 输入输出测试工具。这个程序可以用于开发调用其他程序的软件时进行测试，检查输入输出通讯管道是否正常。
- Input and output testing tool. This program can be used to test when developing software that calls other programs to check whether the input and output communication channels are normal.

### reflex:使用

- 使用: `reflex [模式]`
- 模式:
  - (留空,不提供) : 输入一行字符串，返回一行字符串（单个字符串长度不能超过 1000 半角字符）
  - `/T` : 每隔一秒钟，输出一行当前的时间（不再响应其他输入）
  - `/?`: 显示此帮助信息
  - `/V`: 显示版本信息
  - (其他字符串): 将输入的参数直接输出并退出

### reflex:Usage

- Usage: `reflex [MODE]`
- MODE:
  - (null) : Input as is, output as is. (lenth <= 1000)
  - `/T` : output the current time every second
  - `/?`: display this help and exit
  - `/V`: output version information and exit
  - (other): output parameters

## ![reflex](tspwd/icon1.ico) tspwd.exe

- 获取当前时间戳的SHA256。
  - 例如：A 和 B 的拥有同样的时间戳，那么两边的密码是一样的，两边只要知道时间便可以在不需要密码传输的情况下进行加密传送，相当于拥有一个64位的动态密码。需要根据对方的大概解密时间确定时间戳精度。为了防止被反推和猜测时间戳，可以在此基础上加入预共享的固定密码作为盐。
- Get the SHA256 of the current timestamp.
  - For example, if A and B have the same timestamp, then the passwords on both sides are the same. As long as both sides know the time, they can encrypt and transmit without the need for password transmission, which is equivalent to having a 64-bit dynamic password. The timestamp accuracy needs to be determined based on the approximate decryption time of the other party. In order to prevent the timestamp from being reversed and guessed, a pre-shared fixed password can be added as a salt on this basis.

### tspwd:使用

- 使用: `tspwd [时间戳精度] [盐]`
  - 时间戳精度: `y` / `m` / `d` / `h` / `m` / `s`(默认)
  - 盐: 任意英文字母/数字组成的字符串，小于 200 字符 (默认为空)。
- 命令示例: `tspwd m "aabbcc"`
  - 生成当前月份的 SHA256 临时密码，加入固定密码 `aabbcc` 防止反推。

### tspwd:Usage

- Usage: `tspwd [timestamp precision] [salt]`
  - timestamp precision: `y` / `m` / `d` / `h` / `m` / `s`(default)
  - salt: any string of English letters/numbers, less than 200 characters (default is empty).
- Demo: `tspwd m "aabbcc"`
  - Generate a SHA256 temporary password for the current month, and add a fixed password `aabbcc` to prevent reverse engineering.

## ![tee](tee/icon1.ico) tee.exe

- tee-like:功能类似于 tee
  - 将标准输入复制到每个文件，并复制到标准输出。
  - Copy standard input to each FILE, and also to standard output.

### tee:使用

- 使用: `Usage: tee [模式] [要写入的日志文件]`
- 模式:
  - `/A` : 追加到给定的文件，而不是覆盖
  - `/?`: 显示此帮助信息
  - `/V`: 显示版本信息

### tee:Usage

- Usage: `Usage: tee [MODE] [logfile]`
- MODE supported:
  - `/A` : append to the given FILEs, do not overwrite
  - `/?`: display this help and exit
  - `/V`: output version information and exit

## ![path](install_script\path.ico) path.exe

- 通过命令添加或删除 PATH 变量中的路径。
- Add or remove paths from the PATH variable through commands.

请前往 Please go to: [kagurazakayashi/PathEd](https://github.com/kagurazakayashi/PathEd/blob/master/README.md)

## 其他说明

- 支持 Win32 x86, x64, ARM, ARM64, ARM64EC
  - 程序制作环境： Windows 11 / VS 2022
- 由于 C 语言共通性，部分经少许修改或无需修改可以在 Linux / macOS 中运行。

## Other instructions

- Supports Win32 x86, x64, ARM, ARM64, ARM64EC
  - Programming environment: Windows 11 / VS 2022
- Due to the commonality of C language, some parts can run on Linux / macOS with little or no modification.

## 许可协议 License

```txt
NyarukoMiniTools is licensed under Mulan PSL v2 (木兰宽松许可证，第2版).
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
```
