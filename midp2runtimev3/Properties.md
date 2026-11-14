# Custom properties

### `cc.nnproject.symbianext.version`

Returns version of the extension

Example:
```
if (System.getProperty("cc.nnproject.symbianext.version") != null) {
    // patch installed
}
```

---

### `cc.nnproject.symbianext.variant`

Returns installed patch variant, possible values: `hmidp_eka2`, `hmidp_eka1`, or `jrt1`.

---

## Helpers

### `cc.nnproject.symbianext.checkTLS12`

Checks if TLS 1.2 patch is available

Returns `mbedtls`, `bearssl`, or `false`

Example:
```
String s = System.getProperty("cc.nnproject.symbianext.checkTLS12");
if (s != null && !s.equals("false")) {
    // go proxy-less...
}
```

---


## Java applications

### `cc.nnproject.symbianext.getMidletSuiteVersion:<name>;<vendor>`

Gets version of installed Java application

Parameters:
- name: MIDlet suite name
- vendor: MIDlet suite vendor

Returns version in `<major>.<minor>.<build>` format if MIDlet suite is found, null otherwise

Example:
```
if (System.getProperty("cc.nnproject.symbianext.getMidletSuiteVersion:MPGram;nnproject") == null) {
    // not installed...
}
```

---

### `cc.nnproject.symbianext.launchMidlet:<name>;<vendor>[;<num>[;<cmd>]]`

Launches Java application

Parameters:
- name: MIDlet suite name
- vendor: MIDlet suite vendor
- num: MIDlet number starting from 1, defaults to 1 if not specified
- cmd: Command line arguments, optional

Returns 0 if operation was successful, -11 if midlet is already running, or any other error code

Example:
```
if ("0".equals(System.getProperty("cc.nnproject.symbianext.launchMidlet:MPGram;nnproject;1;-Dasd=dsa -Ddsa=asd"))) {
    // ...
}
```

---

### `cc.nnproject.symbianext.bringMidletToForeground:<name>;<vendor>[;<num>]`

Brings Java application to foreground if it is running

Parameters:
- name: MIDlet suite name
- vendor: MIDlet suite vendor
- num: MIDlet number starting from 1, defaults to 1 if not specified

Returns 0 if operation was successful

Example:
```
if (!"0".equals(System.getProperty("cc.nnproject.symbianext.bringMidletToForeground:MPGram;nnproject"))) {
    if ("0".equals(System.getProperty("cc.nnproject.symbianext.launchMidlet:MPGram;nnproject"))) {
        // ...
    }
}
```

---

### `cc.nnproject.symbianext.getMidletUid:<name>;<vendor>[;<num>]`

Gets registered UID of Java application

Parameters:
- name: MIDlet suite name
- vendor: MIDlet suite vendor
- num: MIDlet number starting from 1, defaults to 1 if not specified

Returns application UID

Example:
```
String gh2meUid = System.getProperty("cc.nnproject.symbianext.getMidletUid:GH2ME;nnproject");
if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:" + gh2meUid))) {
    // ...
}
```

---


## Java IPC

### `cc.nnproject.symbianext.createGlobalData:<name>`

Allocates global OS chunk attached to current process

Parameters:
- name: Chunk name, maximum length is 32

Returns 0 if operation was successful

Example:
```
System.getProperty("cc.nnproject.symbianext.createGlobalData:some_launchUrl");
```

---

### `cc.nnproject.symbianext.setGlobalData:<name>;<data>`

Writes data to global OS chunk

Parameters:
- name: Chunk name, maximum length is 32
- data: Data to write, maximum length is 1024

Returns 0 if operation was successful

Example:
```
System.getProperty("cc.nnproject.symbianext.setGlobalData:some_launchUrl;https://example.com/");
System.getProperty("cc.nnproject.symbianext.bringMidletToForeground:Some;Vendor");
```

---

### `cc.nnproject.symbianext.getGlobalData:<name>`

Reads data from global OS chunk

Parameters:
- name: Chunk name, maximum length is 32

Returns written data

Example:
```
// check on resume: repeated startApp(), showNotify(), etc.
String data = System.getProperty("cc.nnproject.symbianext.getGlobalData:some_launchUrl");
if (data != null && data.length() != 0) {
    // acknowledge
    System.getProperty("cc.nnproject.symbianext.setGlobalData:some_launchUrl;");
    handleUrl(data);
}
```


---



## Symbian applications

### `cc.nnproject.symbianext.isAppInstalled:<uid>`

Checks if Symbian application is installed

Parameters:
- uid: Application UID (decimal, or hexadecimal with 0x)

Returns true if application can be run, false otherwise

Example:
```
// Is ClassiCube installed
if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppInstalled:0xe212a5c2"))) {
    // ...
}
```

---

### `cc.nnproject.symbianext.isAppRunning:<uid>`

Checks if application is running

Parameters:
- uid: Application UID (decimal, or hexadecimal with 0x)

Returns true if application is running, false otherwise

Example:
```
// Is X-Plore running
if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:0xa0000bcd"))) {
    // ...
}
```

---

### `cc.nnproject.symbianext.launchApp:<uid>[,<document>]`

Launches Symbian application by UID

Parameters:
- uid: Application UID (decimal, or hexadecimal with 0x)
- document: ApaCommandLine document path, optional

Returns 0 if operation was successful, otherwise numeric error code

Example:
```
// Launch CorePlayer
if ("0".equals(System.getProperty("cc.nnproject.symbianext.launchApp:0x20012fa6"))) {
    // ...
}
```

---

### `cc.nnproject.symbianext.bringAppToForeground:<uid>[;<message>;<data>]`

Brings application to foreground if it is running, optionally sends a message.

Sending message is not yet supported!

Parameters:
- uid: Application UID (decimal, or hexadecimal with 0x)
- message: Message UID (decimal), optional
- data: Message parameters (non-Unicode), optional

Returns true if operation was successful, false otherwise

Example:
```
String browserUid = "0x10008d39";
if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:" + browserUid))) {
    if ("true".equals(System.getProperty("cc.nnproject.symbianext.bringAppToForeground:" + browserUid + ";0;http://nnproject.cc/"))) {
        // ...
    }
} else if ("0".equals(System.getProperty("cc.nnproject.symbianext.launchApp:" + browserUid + ";http://nnproject.cc/"))) {
    // ...
}
```

---



## SIS registry

### `cc.nnproject.symbianext.getInstalledSisVersion:<uid>`

Checks if SIS package is installed, only for EKA2

Parameters:
- uid: SIS package UID (decimal, or hexadecimal with 0x)

Returns version in `<major>.<minor>.<build>` format if SIS package is installed, null otherwise

Example:
```
// MBedTLS library
String version = System.getProperty("cc.nnproject.symbianext.getInstalledSisVersion:20292b6b");
if (version == null) {
    // not installed...
} else if (parseVersion(version)[1] < 4) {
    // ask user to update...
}
```

---



## Ported Nokia extensions

### `com.nokia.memoryramfree`

Returns amount of free RAM in bytes, mimics S60v3 FP2 behavior

Check Java Developers Library for documentation

---

### `com.nokia.mid.batterylevel`

Returns battery charge level 0-100, mimics S60v3 FP2 behavior

Check Java Developers Library for documentation

---

### `com.nokia.network.access`

Returns network access type of connection currently used, almost mimics S60v3 FP2 behavior, explanation below:

Possible values: `lan`, `pd`, `pd.EDGE`, `pd.3G`, `csd`, `bt_pan`, `wlan`, or `na`.\
Since there is no way to differentiate WCDMA and HSDPA in older versions of OS, it will return `pd.3G` for both.

Unlike official API, it will not return type of default access point when not connected.

Not available on UIQ3.

---


