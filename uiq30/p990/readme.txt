exe extracted from p990i R6E28, was already epoc format
SHA1: 861f39382021a2c6fd389e666aecc35117a6f21f
1200712 bytes, 2299628 bytes unpacked

unpacked
added -UseVerifier to disable preverification
added -comp to force AoT
increased bytecode size limit for jit compilation from 6000 to 65535
removed connection filters for 80 and 443 ports access
replaced load of midp2runtimev2.dll with midp2runtimev3.dll wrapper for custom dynamic system properties

patch in hex:
49288-4928D: 06 A8 58 F0 DE EC -> 00 00 00 00 00 20
7F878-7F879: 70 17 -> FF FF
8CC9B: D1 -> E0
8CDE5-8CDEF: 55 73 65 4A 61 72 43 61 63 68 65 -> 63 6F 6D 70 00 00 00 00 00 00 00
8CDF5-8CE07: 43 61 63 68 65 4A 61 72 46 69 6C 65 48 61 6E 64 6C 65 73 -> 55 73 65 56 65 72 69 66 69 65 72 00 00 00 00 00 00 00 00
1EDD12: 32 -> 33


take midp2runtimev3.dll from midp2runtimev3dll

untested

by shinovon, worked on this on 11.11.2026
