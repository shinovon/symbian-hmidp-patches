/*
 * Description: Automatically generated JNI lookup file. Do not modify manually.
 */

#include "javasymbianoslayer.h"
typedef void (*TFunc)();
#include "com_symbian_midp_runtime_properties_PropertySet_cc.h"
#include "javaosheaders.h"
#include "javasymbianoslayer.h"
const FuncTable funcTable[] = {
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1bringAppToForeground", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1bringAppToForeground},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1bringMidletToForeground", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1bringMidletToForeground},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1checkTLS12", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1checkTLS12},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getInstalledSisVersion", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getInstalledSisVersion},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getMidletSuiteVersion", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getMidletSuiteVersion},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getMidletUid", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getMidletUid},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1isAppInstalled", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1isAppInstalled},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1isAppRunning", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1isAppRunning},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1launchApp", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1launchApp},
   { "Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1launchMidlet", (unsigned int) Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1launchMidlet}
};

IMPORT_C TFunc jni_lookup(const char* name);
EXPORT_C TFunc jni_lookup(const char* name) {
    return (TFunc)findMethod(name, funcTable, sizeof(funcTable)/sizeof(FuncTable));
}
