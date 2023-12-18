# a653lib

## Introduction

The libApex LIBRARY is an arinc 653 scheduler for Linux (uses POSIX standard), standardized by the A653.

## License

Refer to [LICENCE](./LICENSE.md) file.

## Limitations

- not all functionality is implemented now, but partitions with Processes using sampling and queueing ports
  is shown by the demonstration code.
- not implemented function are implemented as empty functions and will be added if needed step by step.
- The implementation is compliant only with A653.

## Operating system and compilers

The libApex LIBRARY can be built for the following operating systems and compilers:

- Linux 32 bits (RHEL7)
- Linux 64 bits (RHEL7)

## Dependencies

```make
LDLIBS   = -lpthread -lrt -lm -ldl
```

## Logging

The logging strategy can be controlled through by

```c
setDebug(int level)
```

call and

```c
printDebug(int level, const char *format, ... ){
```

can by used with the same interface as printf() with a leading debug level

## Compilation

call Make in base directory to build `libapex.a` and bind to demo implementation. Result will be located at `~/bin` in your home
directory. There will be the files:

- `a635_main`
- `partition_a`
- `partition_b`

## Run Demo

go to `~/bin` and start `./a653_main`

this will generate following output:

```text
pid: 578773 <1702486050.317812578>: Current local time and date: Wed Dec 13 17:47:30 2023

pid: 578773 <1702486050.318212993>: apex_shm id: 0x63801e ptr: 0x7f5e57802000

pid: 578773 <1702486050.318233554>: > taskset --cpu-list 0 ./partition_a & :

pid: 578773 <1702486050.349241033>: a653 start (other pid) 578775

pid: 578773 <1702486050.349254883>: > taskset --cpu-list 1 ./partition_b & :
```

......

## Useful targets

|   target    |         Purpose        |
| :---------- | :--------------------- |
| `all` | Build everything. This is the default target. |
| `amain` | Only build the main program (scheduler a653_main). |
| `part_a` | Only build the partition a. |
| `part_b` | Only build the partition b. |
| `alib` | Only build the `libapex.a`. |

## Handle version

## Tests debugging

## Build with Make
