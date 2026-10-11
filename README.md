# ucnmod

A partially complete mod for the game of Ultimate Custom Night.

It was supposed to be a versatile utility with multiple funcitonalities, hence
it's got a fully-pledged command line. Currently it only supports amending the
number of frames per seconds, which also changes the pace of the game due to how
Clickteam Fusion works. If you ever wanted to make 50/20 work like 200/20, this
is one of the ways to do this.

Here are a few ideas to implement in the future:
- Asset replacement
- Enemy AI level override
- Endless mode

# Usage

Having downloaded the source code, build with
```bash
make all
```

Run the executable in `bin`. You will see a command line with which you interact
with the mod. Run the game and, if you are on Linux, record its PID. Attach the
mod to the game with
```
> attach <PID>
```
If you are on Windows, you can omit the PID - the mod will find the process by
itself. This should pause the game, and to unpause it you need to use
```
> detach
```
After detaching, you can attach again to the same process with `attach prev`.

As I've mentioned, the only operation currently supported is changing the number
of FPS. The command
```
> fps
```
shows you the current number (60 by default), and
```
> fps <FPS>
```
overrides this number. The supported range is \[1-1000\] due to a frame having
to last for at least a millisecond.

There are also two commands `status`, which prints out the attachment status,
and `quit`.
