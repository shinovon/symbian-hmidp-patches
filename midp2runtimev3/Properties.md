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

Returns installed patch variant, possible values: `hmidps60v3`, `hmidps60v2`, `hmidpuiq3`, or `jrt1`.

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

See example in bringMidletToForeground below.

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
if ("-11".equals(System.getProperty("cc.nnproject.symbianext.launchMidlet:MPGram;nnproject"))) {
    // app is already running, resume
    if ("0".equals(System.getProperty("cc.nnproject.symbianext.bringMidletToForeground:MPGram;nnproject"))) {
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

Launches Symbian application by UID, similar to `localapp://native/launch?application-uid=<uid>;content=<document>` on Symbian^3

Parameters:
- uid: Application UID (decimal, or hexadecimal with 0x)
- document: document name, optional

Returns 0 if operation was successful, otherwise numeric error code

See example in bringAppToForeground below.

---

### `cc.nnproject.symbianext.bringAppToForeground:<uid>[;<message>;<data>]`

Brings application to foreground if it is running, optionally sends a message.

Parameters:
- uid: Application UID (decimal, or hexadecimal with 0x)
- message: `open`, `create`, or Message UID (decimal, or hexadecimal with 0x), optional
- data: File name if message is `open` or `create`, or non-Unicode message data, optional

Returns 0 if operation was successful, otherwise numeric error code

Example:
```
String operaUid = "0x2002aa96";
if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:" + operaUid))) {
    if ("true".equals(System.getProperty("cc.nnproject.symbianext.bringAppToForeground:" + operaUid + ";open;https://nnproject.cc/"))) {
        // ...
    }
} else if ("0".equals(System.getProperty("cc.nnproject.symbianext.launchApp:" + operaUid + ";https://nnproject.cc/"))) {
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



# Reference implementation of launch handling

```java
static final String SYMEXT = "cc.nnproject.symbianext.";
static final String IPC_ID = "example";

static boolean started;
static int launchInstance;
static boolean launchHandled;

static boolean symbianext;

public void startApp() {
    if (started) {
        // resumed
        String p = System.getProperty("com.nokia.mid.cmdline.instance");
        if (p != null) {
            // handle Symbian^3 javaapp: launch request
            int i = Integer.parseInt(p);
            if (i > launchInstance) {
                launchInstance = i;
                handleLaunch(true);
            }
        } else if (symbianext) {
            p = System.getProperty(SYMEXT + "getGlobalData:" + IPC_ID + "_launchUrl");
            if (p != null && p.length() != 0) {
                handleLaunch(true);
            }
        }
        return;
    }
    started = true;
    
    // check symbianext support
    symbianext = System.getProperty(SYMEXT + "version") != null;
    if (System.getProperty(SYMEXT + "version") != null) {
        // allocate chunk
        System.getProperty(SYMEXT + "createGlobalData:" + IPC_ID + "_launchUrl");
    }
    
    // ...
    
    // after loading is done and authorization is checked
    handleLaunch(false);
}

static synchronized void handleLaunch(boolean restart) {
    if (restart && !launchHandled) return;
    
    // check if application is ready to handle launch requests
    // if (authId != null) return;
    
    launchHandled = true;
    try {
        String cmd = System.getProperty("com.nokia.mid.cmdline.instance");
        String url = null;
        if (cmd != null) {
            launchInstance = Integer.parseInt(cmd);
        } else {
            url = System.getProperty(SYMEXT + "getGlobalData:" + IPC_ID + "_launchUrl");
            if (url != null && url.length() != 0) {
                // clear launchUrl
                System.getProperty(SYMEXT + "setGlobalData:" + IPC_ID + "_launchUrl;");
            } else {
                url = null;
            }
        }

        if (url == null) {
            // handle Symbian^3 javaapp: and MIDletIntegration launch
            cmd = System.getProperty("com.nokia.mid.cmdline");
            if (cmd == null) {
                cmd = System.getProperty("launchcmd");
            }
            if ("empty=1".equals(cmd)) cmd = "";

            if (cmd != null && cmd.length() != 0) {
                int i = cmd.indexOf("url=");
                if (i != -1) {
                    int j = cmd.indexOf(';', i + 4);
                    url = cmd.substring(i + 4, j == -1 ? cmd.length() : j).trim();
                }
            }
        }

        if (url == null || url.length() == 0) {
            url = System.getProperty("url");
        }

        if (url != null && url.length() != 0) {
            // do something with the url...
        }
    } catch (Exception ignored) {}
    return false;
}
```

Add these to MANIFEST.MF:
```
Nokia-MIDlet-Launch-Params: url
Nokia-MIDlet-Background-Event: pause
```

Full launch request example:
```java
static final String SYMEXT = "cc.nnproject.symbianext.";

static boolean launch(String suite, String vendor, String id, String url) {
    String r = System.getProperty(SYMEXT + "getMidletSuiteVersion:" + suite + ';' + vendor);
    if (r == null) {
        // not installed
        return false;
    }
    
    r = System.getProperty(SYMEXT + "launchMidlet:" + suite + ';' + vendor + (url != null ? ";1;-Durl=" + url : ""));
    if ("0".equals(r)) {
        // launched successfully
        return true;
    }
    
    if ("-11".equals(r)) {
        // app is already running
        if (url != null && !"0".equals(System.getProperty(SYMEXT + "setGlobalData:" + id + "_launchUrl;" + url))) {
            // data chunk not allocated
            return false;
        }
        if ("0".equals(System.getProperty(SYMEXT + "bringMidletToForeground:" + suite + ';' + vendor))) {
            // resumed successfully
            return true;
        }
    }
    
    return false;
}
```

```
launch("MPGram", "nnproject", "mpgram", "https://t.me/nnmidlets");
```
