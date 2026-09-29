# Solution

## Using Ghidra

### Basic Setup

Start by [installing Ghidra](https://github.com/nationalsecurityagency/ghidra)
if you don't have it yet.

Open Ghidra, start a new project and import the binary.

### Static Analysis

Double click the binary to open the CodeBrowser
and let ghidra analyze the binary (if not analyzed).
Under 'Symbol Tree' select 'Functions' and select 'main'.
This will give you the assembly code for that function and on the 'Decompile' panel
you will have the pseudo C for that function.
The main function calls a `vuln()` function, double click it
and you will have the pseudo C for that function.

In the `vuln()` function you will see a `strcmp()` that takes our input
and compares it with the password, that's it!

## Using GDB (with GEF)

### Setup

I recommend doing this with [GDB Enhanced Features](https://github.com/hugsy/gef)
but you can do this with just GDB

### Dynamic Analysis

On a terminal, run gdb with the program: `gdb ./vuln`

Then check which function gdb found: `info functions`

Check the assembly for the `main` function: `disas main`

You will find a line with the following assembly: `call   0x118e <vuln>`

This means that the main function calls the `vuln` function, let's check the
assembly for that function: `disas vuln`

You will find the following lines:

```
   0x00000000000011c8 <+58>:    mov    rsi,rdx
   0x00000000000011cb <+61>:    mov    rdi,rax
   0x00000000000011ce <+64>:    call   0x1060 <strcmp@plt>
```

Note: `rsi` and `rdi` represent the first 2 arguments that are passed to the
next `call` since we are on Linux x86-64
(you can check this using the `file` command, like: `file vuln`)

Let's check what are the arguments passed to the `strcmp` function.
We can do this by placing a breakpoint on the `strcmp` line: `break *vuln+64`

Note: The breakpoint was placed at `vuln+64` because we wanted
to put the breakpoint on the `vuln` function with an offset of 64.
( 0x00000000000011ce <+**64**>: call 0x1060 <strcmp@plt>)

Now let's run the binary : `run`

Place a random input and GDB will stall at the placed breakpoint, if you have
GEF install, the registers will appear right
away, you will have something like this:

```
$rsi   : 0x0000555555556055  →  <password>
$rdi   : 0x00007fffffffdb20  →  <your-input>
```

This is it! We got the password.

If you don't have GEF installed, whenever
the program stalls, check the registers: `info registers`
You will have to check the `rdi` and `rsi` registers:

```
rsi            0x555555556055      0x555555556055
rdi            0x7fffffffdb20      0x7fffffffdb20
```

You can now print the value as a readable string using `x/s $rsi` to check
the password, if you did `x/s $rdi` you would get your input.

## Strings

Since this was a simple challenge with the password (and flag) hardcoded,
you could run the command `strings vuln | less` and you would get the password
and flag directly (this only works in simple challenges
but it's always worth the shot).
