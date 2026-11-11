exe extracted from e61i, was already epoc format
SHA1: fedd013434b18c0898f4712c7e760da82ccf362c
1203528 bytes, 2274656 bytes unpacked

unpacked
added -UseVerifier to disable preverification
added -comp to force AoT
increased bytecode size limit for jit compilation from 1500 to 16384
replaced "MidiClient{000a0000}[101f7d00].dll" import with "MidiClien2{000a0000}[201f7d00].dll" wrapper for volume control support
removed connection filters for 80 and 443 ports access
replaced load of midp2runtimev2.dll with midp2runtimev3.dll wrapper for custom dynamic system properties

patch in hex:
4BF16-4BF1B: 75 F0 28 EF 05 90 -> 00 00 00 00 00 20
82EE8-82EE9: DC 05 -> 00 40
A1CD3: D1 -> E0
A1E09-A1E13: 55 73 65 4A 61 72 43 61 63 68 65 -> 63 6F 6D 70 00 00 00 00 00 00 00
A1E19-A1E2B: 43 61 63 68 65 4A 61 72 46 69 6C 65 48 61 6E 64 6C 65 73 -> 55 73 65 56 65 72 69 66 69 65 72 00 00 00 00 00 00 00 00
1DC662: 32 -> 33
20105D: 74 -> 32
201069: 31 -> 32

not tested

by shinovon, worked on this on 09.11.2026, 11.11.2026
