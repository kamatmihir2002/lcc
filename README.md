# lcc

This is a recovered copy of the source code of a C-subset compiler I worked on a couple years ago, cleaned up and tidied a bit. Outputs single pass SSA code with additional bookkeeping for function declarations.

I had completed it before, but the final copy outputted pretty bad NASM code, with some very basic optimizations like dead code elimination for if-conditionals with pure constant statements. 

I plan on making this a modular multipass optimizing compiler that outputs fairly optimized nasm code. I plan on writing an `opt` module that handles propagations in multiple passes, along with dead code elimination and (maybe) loop unrolling. I also plan on writing a basic assembly emitter `toasm` to emit nasm code.

For now this has no symbol checking. Errors are also printed to stdout.

# Build

To build, do ```make```.

# Test

To test, feed any file from test.c to `lcc`.
```
bin/lcc < test/test.c

```

# Roadmap

1. Add symbol checking.
2. Add multipass `opt` module:

    Hypothetically this should work like this:
    ```
        lcc < sample_file.c | opt | opt ...
    ```
    where each iteration propagates different constants and variables.

3. Add backend translator `toasm` to NASM.
4. Chain all modules through a simple driver app.