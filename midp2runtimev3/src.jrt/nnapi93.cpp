#include "com_symbian_midp_runtime_properties_PropertySet_cc.h"
#include "common.h"

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1checkTLS12
	(JNIEnv *, jclass)
{
	TInt res = 0;
	TRAP(res, res = DoCheckMbedTlsL());
	return res;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1isAppInstalled
	(JNIEnv *, jclass, jint aUid)
{
	TInt res = 0;
	TRAP(res, res = DoIsAppInstalledL(TUid::Uid(aUid)));
	return res;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1isAppRunning
	(JNIEnv *, jclass, jint aUid)
{
	TInt res = 0;
	TRAP(res, res = DoIsAppRunningL(TUid::Uid(aUid)));
	return res;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1launchApp
	(JNIEnv *, jclass, jint, jstring)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1bringAppToForeground
	(JNIEnv *, jclass, jint, jboolean, jint, jstring)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getMidletUid
	(JNIEnv *, jclass, jstring, jstring, jint)
{
	// TODO
	return 0;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getMidletSuiteVersion
	(JNIEnv *, jclass, jstring, jstring, jintArray)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1launchMidlet
	(JNIEnv *, jclass, jstring, jstring, jint, jstring)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1bringMidletToForeground
	(JNIEnv *, jclass, jstring, jstring, jint)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_com_symbian_midp_runtime_properties_PropertySet_1cc__1getInstalledSisVersion
	(JNIEnv *, jclass, jint, jintArray)
{
	// TODO
	return KErrNotSupported;
}
