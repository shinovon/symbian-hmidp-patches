/*
 * Description: Automatically generated JNI lookup file. Do not modify manually.
 */

#include "javasymbianoslayer.h"
typedef void (*TFunc)();
#include "cc_nnproject_symbianext_PropertyExtensions.h"
const FuncTable funcTable[] = {
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1bringAppToForeground", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1bringAppToForeground},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1bringMidletToForeground", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1bringMidletToForeground},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1checkTLS12", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1checkTLS12},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1getInstalledSisVersion", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1getInstalledSisVersion},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1getMidletSuiteVersion", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1getMidletSuiteVersion},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1getMidletUid", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1getMidletUid},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1isAppInstalled", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1isAppInstalled},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1isAppRunning", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1isAppRunning},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1launchApp", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1launchApp},
   { "Java_cc_nnproject_symbianext_PropertyExtensions__1launchMidlet", (unsigned int) Java_cc_nnproject_symbianext_PropertyExtensions__1launchMidlet}
};

IMPORT_C TFunc jni_lookup(const char* name);
EXPORT_C TFunc jni_lookup(const char* name) {
    return (TFunc)findMethod(name, funcTable, sizeof(funcTable)/sizeof(FuncTable));
}
