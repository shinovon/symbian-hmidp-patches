exe extracted from n73me, was already epoc format
SHA1: 276a719645d65f707105aeed996015749369dbc4
1181848 bytes, 2219412 bytes unpacked

unpacked
added -UseVerifier to disable preverification
added -comp to force AoT
increased bytecode size limit for jit compilation from 1000 to 16000
replaced "MidiClient{000a0000}[101f7d00].dll" import with "MidiClien2{000a0000}[201f7d00].dll" wrapper for volume control support
removed connection filters for 80 and 443 ports access
replaced load of midp2runtimev2.dll with midp2runtimev3.dll wrapper for custom dynamic system properties

patch in hex:
46546-4654B: 75 F0 F6 EC 05 90 -> 00 00 00 00 00 20
7D30B: 00 -> 01
9C12F: E1 -> E0
9C265-9C26F: 55 73 65 4A 61 72 43 61 63 68 65 -> 63 6F 6D 70 00 00 00 00 00 00 00
9C275-9C287: 43 61 63 68 65 4A 61 72 46 69 6C 65 48 61 6E 64 6C 65 73 -> 55 73 65 56 65 72 69 66 69 65 72 00 00 00 00 00 00 00 00
1D07AA: 32 -> 33
1F4717: 74 -> 32
1F4723: 31 -> 32

not tested

by shinovon, worked on this on 09.11.2026