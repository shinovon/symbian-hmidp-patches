#include "cc_nnproject_symbianext_PropertyExtensions.h"
#include "common.h"

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1checkTLS12
	(JNIEnv *, jclass)
{
	TInt res = 0;
	TRAP(res, res = DoCheckMbedTlsL());
	return res;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1isAppInstalled
	(JNIEnv *, jclass, jint aUid)
{
	TInt res = 0;
	TRAP(res, res = DoIsAppInstalledL(TUid::Uid(aUid)));
	return res;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1isAppRunning
	(JNIEnv *, jclass, jint aUid)
{
	TInt res = 0;
	TRAP(res, res = DoIsAppRunningL(TUid::Uid(aUid)));
	return res;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1launchApp
	(JNIEnv *, jclass, jint, jstring)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1bringAppToForeground
	(JNIEnv *, jclass, jint, jboolean, jint, jstring)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1getMidletUid
	(JNIEnv *, jclass, jstring, jstring, jint)
{
	// TODO
	return 0;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1getMidletSuiteVersion
	(JNIEnv *, jclass, jstring, jstring, jintArray)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1launchMidlet
	(JNIEnv *, jclass, jstring, jstring, jint, jstring)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1bringMidletToForeground
	(JNIEnv *, jclass, jstring, jstring, jint)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1getInstalledSisVersion
	(JNIEnv *, jclass, jint, jintArray)
{
	// TODO
	return KErrNotSupported;
}
