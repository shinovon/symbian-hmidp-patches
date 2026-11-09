exe extracted from n93, was already epoc format
SHA1: ac4946ef3907ddaab6c43c56d10a8c360d8a41b9
1181838 bytes, 2219304 bytes unpacked

unpacked
added -UseVerifier to disable preverification
added -comp to force AoT
increased bytecode size limit for jit compilation from 1000 to 16000
removed connection filters for 80 and 443 ports access
replaced load of midp2runtimev2.dll with midp2runtimev3.dll wrapper for custom dynamic system properties

patch in hex:
4657A-4657F: 75 F0 FA EC 05 90 -> 00 00 00 00 00 20
7D33F: 00 -> 01
9C163: D1 -> E0
9C299-9C2A3: 55 73 65 4A 61 72 43 61 63 68 65 -> 63 6F 6D 70 00 00 00 00 00 00 00
9C2A9-9C2BB: 43 61 63 68 65 4A 61 72 46 69 6C 65 48 61 6E 64 6C 65 73 -> 55 73 65 56 65 72 69 66 69 65 72 00 00 00 00 00 00 00 00
1D073A: 32 -> 33

not tested

by shinovon, worked on this on 09.11.2026