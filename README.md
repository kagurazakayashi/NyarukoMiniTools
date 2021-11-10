# NyarukoMiniTools
存放一些超小的程序的仓库。

- 在平时使用 Windows 制作一些 bat 批处理脚本时，会遇到需要某些命令来操作的问题。因此创建了本仓库，存储一些实用工具。
- 这些工具全都是 C 语言编写，体积小，可以直接拷贝到 System32 来作为命令调用。
- `copy *.exe C:\Windows\System32\`

# ![sleep](sleep/icon1.ico) sleep.exe
为了补充 Windows 中缺失的批处理中常用的延时命令 sleep

- 暂停 n 秒
- Pause for N seconds

## 中文
- 使用: `SLEEP 数字[后缀]`...
- 或者: `SLEEP 选项`
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
    - `/?` : 显示此帮助信息
    - `/V` : 显示版本信息

## English
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
    - `/?` : display this help and exit
    - `/V` : output version information and exit

# ![datetime](datetime/icon1.ico) datetime.exe
更方便地在批处理中获取需要的格式时间

- 输出时间日期
- Get time and date

## 中文
- 使用: `datetime 模式 [时间]`
- 模式:
    - 支持的模式: `yyyy,yy,MM,dd,HH,hh,mm,ss`
        - （4位年，2位年，月，日，24小时时，12小时时，分，秒）
    - 不支持模式: `y,M,d,H,h,m,s,ddd,dddd,MMM,MMMM,gg ...`
- 时间:
    - `gmt` 标准时间 或者 `local` 本地时间 (默认值)
- 演示:
    - `datetime "yyyy-MM-dd HH:mm:ss"`
    - `      ->  2021-02-01 20:21:11`
- `datetime /?` : 显示此帮助信息
- `datetime /V` : 显示版本信息

## English
- Usage: `datetime MODE [TIME]`
- MODE:
    - Support:   `yyyy,yy,MM,dd,HH,hh,mm,ss`
    - Unsupport: `YYYY,y,M,d,H,h,m,s,ddd,dddd,MMM,MMMM,gg ...`
- TIME:
    - `gmt` or `local` (the default)
- DEMO:
    - `datetime "yyyy-MM-dd HH:mm:ss"`
    - `      ->  2021-02-01 20:21:11`
- `datetime /?` : display this help and exit
- `datetime /V` : output version information and exit

# ![repstr](repstr/icon1.ico) repstr.exe
替换英文字符串
String substitution

## 中文
- 使用: `repstr [模式] [原字符串] [要替换的字符串] [替换为]`
    - 所有字符串只允许英文和数字
- 模式:
   - `/c` : 忽略大小写
   - `/?` : 显示此帮助信息
   - `/V` : 显示版本信息
- 演示:
    - `repstr info\txt\readme.txt \ /`

## English
- Usage: `repstr [MODE] [String] [Find what] [Replace with]`
    - `String`: English only.
- MODE supported:
   - `/c` : ignore case
   - `/?` : display this help and exit
   - `/V` : output version information and exit
- DEMO:
    - `repstr info\txt\readme.txt \ /`

# ![scrctl](scrctl/icon1.ico) scrctl.exe
方便地快速关闭显示器和锁定系统

- 屏幕开关控制
- Screen Power Mode

## 中文
- 使用: `scrctl [模式]`
- 模式:
    - `off ` 或 `/0` 关闭显示器 (default)
    - `on  ` 或 `/1` 激活显示器
    - `eco ` 或 `/2` 节电模式
    - `lock` 或 `/3` 关闭显示器并锁定系统
    - `/?  ` 显示此帮助信息
    - `/V  ` 显示版本信息

## English
- Usage: `scrctl [MODE]`
- MODE:
    - `off ` or `/0` Power Off (default)
    - `on  ` or `/1` Power On
    - `eco ` or `/2` Low Power Mode
    - `lock` or `/3` Power Off and Lock
    - `/?  ` display this help and exit
    - `/V  ` output version information and exit

# ![genpwd](genpwd/icon1.ico) genpwd.exe
快捷生成由随机字符组成的密码

- 随机密码生成器
- Password Generator

## 中文
- 使用:   `genpwd [要包含的字符] [单个密码长度] [要生成的数量]`
   - 或者 `[单个密码长度] [要生成的数量]`
   - 或者 `[单个密码长度]`
- 要包含的字符 包括:
   - `A` : 大写字母
   - `a` : 小写字母
   - `n` : 数字
   - `s` : 标点符号
   - `/?`: 显示此帮助信息
   - `/V`: 显示版本信息
- 命令示例: `genpwd Aan 32 1` (生成1个32位的由大小写字母和数字组成的密码)

## English
- Usage: `genpwd [MODE] [pw_length] [num_pw]`
   - or  `[pw_length] [num_pw]`
   - or  `[pw_length]`
- MODE supported:
   - `A` : Upper case
   - `a` : Lower case
   - `n` : Number
   - `s` : Symbol
   - `/?`: display this help and exit
   - `/V`: output version information and exit
- Demo: `genpwd Aan 32 1`

# ![null](null/icon1.ico) null.exe
程序直接退出自己，用于将普通 exe 替换来禁用其原有功能。例如替代软件中的广告弹窗 exe 。

- 该程序不做任何事情
- does nothing

# ![reflection](reflection/icon1.ico) reflection.exe

- 输入输出测试工具
- Input and output test tool

## 中文
- 这个程序可以用于开发调用其他程序的软件时进行测试，检查输入输出通讯管道是否正常。
- 使用: `reflection [模式]`
- 模式:
   - (留空,不提供) : 输入一行字符串，返回一行字符串（单个字符串长度不能超过 1000 半角字符）
   - `/T` : 每隔一秒钟，输出一行当前的时间（不再响应其他输入）
   - `/?`: 显示此帮助信息
   - `/V`: 显示版本信息
   - (其他字符串): 将输入的参数直接输出并退出

## English
- Usage: `reflection [MODE]`
- MODE:
   - (null) : Input as is, output as is. (lenth <= 1000)
   - `/T` : output the current time every second
   - `/?`: display this help and exit
   - `/V`: output version information and exit
   - (other): output parameters

# 其他说明
- 由于程序体积考虑，程序图标采用了最小规格 (16x16@1位) ，在 Windows 资源管理器显示其图标时，可能会发生以下情况，这是资源管理器程序的 bug ：
    - 图标显示为任意其他已安装应用程序的图标。
    - 没有图标。
    - 「空白文件」图标。
- 支持 x86, x64 ，不支持其他平台。
    - 程序制作环境： Windows 11 / VS 2022
- 由于 C 语言共通性，部分经少许修改或无需修改可以在 Linux / macOS 中运行。
    - 但本项目本身是为了补充 Windows 中缺失的命令，因此可能没有意义。

# 许可协议 License
```
NyarukoMiniTools is licensed under Mulan PSL v2 (木兰宽松许可证，第2版).
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
```
