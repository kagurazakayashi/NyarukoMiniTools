# ![icon](install_script/icon.ico) NyarukoMiniTools

English | [简体中文](README.chs.md)

- A repository for storing some very small Windows programs.
- These tools are all written in C language.
- These newly added tool commands can make bat batch processing more convenient.

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
- I am writing an old language program. If there are non-digits, English characters, or special symbols in the code, compilation errors will occur. Using `chkascii.exe` can check in advance.

## Install

1. Download the latest compiled version from `Release`.
2. Extract the architecture (x86/x64) version file corresponding to the current system, and put all the extracted `.exe` files in the same folder.

### Call as a command in cmd at any time

If the `path` component is selected during installation, the installation folder will be automatically added to the system `path` environment variable.

If it is a manual installation, you can choose one of the following:

- Copy all `.exe` in this assembly to the `%SystemRoot%\system32\` folder.
- Add the folder where this assembly is located to the `path` environment variable, which can be achieved using `path.exe`.

### Compatibility

- Supports Win32 x86, x64, ARM, ARM64.
- The following programs run on ARM platforms through emulation, and only support newer versions of Windows 10 on ARM and later:
- The installers for all platforms are built using the x86 architecture, and the installed files are the target platform.
- You can download portable.7z to skip the installation step.
- There is no ARM32 version of `path.exe`, so the x86 version will be provided in the ARM32 installer.

## Uninstall

Just follow the reverse installation steps.

## ![sleep](sleep/icon1.ico) sleep.exe

- sleep-like: Pause for a certain amount of time.
  - Supplement the missing delay command `sleep` commonly used in batch processing in Windows. No longer need to use commands such as ping as a delay substitute.
  - Support units (seconds, minutes, hours, days).
  - Support multiple parameters to add.
  - Support countdown display.

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

Get time and date.
It is more convenient to obtain the required format time in batch processing.

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

String substitution

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

Monitor Power Control.
Conveniently and quickly turn off the display and lock the system.

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

pwgen-like: Functions similar to `pwgen`: quickly generate passwords consisting of random characters. Password Generator.

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

The program does not do anything.
The program exits itself directly, which is used to replace the ordinary exe to disable its original function.
For example, it replaces the ad pop-up exe in the software.

## ![reflex](reflex/icon1.ico) reflex.exe

Input and output testing tool.
This program can be used to test when developing software that calls other programs to check whether the input and output communication channels are normal.

### reflex:Usage

- Usage: `reflex [MODE]`
- MODE:
  - (null) : Input as is, output as is. (lenth <= 1000)
  - `/T` : output the current time every second
  - `/?`: display this help and exit
  - `/V`: output version information and exit
  - (other): output parameters

## ![reflex](tspwd/icon1.ico) tspwd.exe

- Get the SHA256 of the current timestamp.
  - For example, if A and B have timestamps in the same interval, then the passwords on both sides are the same. As long as both sides know the time, they can encrypt and transmit without the need for password transmission, which is equivalent to having a 64-bit dynamic password. The timestamp accuracy needs to be determined based on the approximate decryption time of the other party. In order to prevent the timestamp from being reversed and guessed, a pre-shared fixed password can be added as a salt on this basis.

### tspwd:Usage

- Usage: `tspwd [timestamp precision] [salt]`
  - timestamp precision: `y` / `m` / `d` / `h` / `m` / `s`(default)
  - salt: any string of English letters/numbers, less than 200 characters (default is empty).
- Demo: `tspwd m "aabbcc"`
  - Generate a SHA256 temporary password for the current month, and add a fixed password `aabbcc` to prevent reverse engineering.

## ![tee](tee/icon1.ico) tee.exe

tee-like: Functionality similar to `tee`. Copy standard input to each FILE, and also to standard output.

### tee:Usage

- Usage: `tee [MODE] [logfile]`
- MODE supported:
  - `/A`: append to the given FILEs, do not overwrite
  - `/?`: display this help and exit
  - `/V`: output version information and exit

## ![chkascii](chkascii/icon1.ico) chkascii.exe

Checks whether the text in a file is within the standard ASCII code range.
After running, a statistical report output will be given.

### chkascii:Usage

- Usage:  `chkascii CHKASCII [MODE] [filepath]`
- MODE supported:
  - `/D`: display detailed check progress (output the check result of each word, slow)
  - `/?`: display this help and exit
  - `/V`: output version information and exit
- If `filepath` is not provided, the mode will be entered to accept user keyboard input.

## ![path](install_script\path.ico) path.exe

Add or remove paths from the PATH variable through commands.

Please go to: [kagurazakayashi/PathEd](https://github.com/kagurazakayashi/PathEd/blob/master/README.md)

## License

```txt
NyarukoMiniTools is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
```
