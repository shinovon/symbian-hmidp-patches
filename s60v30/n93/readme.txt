exe extracted from n93, was already epoc format
SHA1: ac4946ef3907ddaab6c43c56d10a8c360d8a41b9
1181838 bytes, 2219304 bytes unpacked

unpacked
Disabled UseVerifier
Disabled MixedMode
increased bytecode size limit for jit compilation from 1000 to 16000
removed connection filters for 80 and 443 ports access
replaced load of midp2runtimev2.dll with midp2runtimev3.dll wrapper for custom dynamic system properties
replaced load of lcdgr.dll with lcdg2.dll wrapper
patched CMIDToolkit::HandleForegroundL to add Nokia-MIDlet-Background-Event support
Set CompilerAreaPercentage to 0

patch in hex:
4657A-4657F: 75 F0 FA EC 05 90 -> 00 00 00 00 00 20
6F960-6F965: 00 6D 88 34 07 00 -> 07 00 00 6D 88 34
6F968-6F969: 28 37 -> 00 00
7D33F: 00 -> 01
9C163: D1 -> E0
9C299-9C2A3: 55 73 65 4A 61 72 43 61 63 68 65 -> 63 6F 6D 70 00 00 00 00 00 00 00
9C2A9-9C2BB: 43 61 63 68 65 4A 61 72 46 69 6C 65 48 61 6E 64 6C 65 73 -> 55 73 65 56 65 72 69 66 69 65 72 00 00 00 00 00 00 00 00
1CE834: 72 -> 32
1D073A: 32 -> 33
1F0C3C: 14 -> 00


not tested

by shinovon, worked on this on 09.11.2026, 17.11.2026