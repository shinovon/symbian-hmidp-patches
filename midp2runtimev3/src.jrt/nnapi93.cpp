#include "cc_nnproject_symbianext_PropertyExtensions.h"
#include "common.h"

HBufC* jstringToDes(JNIEnv* aEnv, jstring aStr)
{
	HBufC* res = 0;
	jboolean iscopy;
	const jchar* jchr = aEnv->GetStringChars(aStr, &iscopy);
	if (!jchr) {
		return NULL;
	}
	jint len = aEnv->GetStringLength(aStr);
	res = HBufC::New(len + 1);
	TPtr ptr = res->Des();
	TPtrC16 ptr16((const TUint16 *) jchr, len);
	ptr.Copy(ptr16);
	ptr.ZeroTerminate();
	aEnv->ReleaseStringChars(aStr, jchr);
	return res;
}

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
	(JNIEnv *aEnv, jclass, jint, jstring)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1bringAppToForeground
	(JNIEnv *aEnv, jclass, jint, jboolean, jint, jstring)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1getMidletUid
	(JNIEnv *aEnv, jclass, jstring aName, jstring aVendor, jint aNum)
{
	HBufC* name = jstringToDes(aEnv, aName);
	HBufC* vendor = jstringToDes(aEnv, aVendor);
	
	TInt32 res = 0;
	TRAP_IGNORE(res = DoGetMidletUidL(name->Des(), vendor->Des(), aNum));
	
	delete vendor;
	delete name;
	
	return res;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1getMidletSuiteVersion
	(JNIEnv *aEnv, jclass, jstring, jstring, jintArray)
{
	// TODO
	return KErrNotSupported;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1launchMidlet
	(JNIEnv *aEnv, jclass, jstring aName, jstring aVendor, jint aNum, jstring aCmd)
{
	HBufC* name = jstringToDes(aEnv, aName);
	HBufC* vendor = jstringToDes(aEnv, aVendor);
	HBufC* cmd = jstringToDes(aEnv, aCmd);
	
	TInt res;
	TRAP(res, DoLaunchMidletL(cmd->Des(), name->Des(), vendor->Des(), aNum));
	
	delete cmd;
	delete vendor;
	delete name;
	
	return res;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1bringMidletToForeground
	(JNIEnv *aEnv, jclass, jstring aName, jstring aVendor, jint aNum)
{
	HBufC* name = jstringToDes(aEnv, aName);
	HBufC* vendor = jstringToDes(aEnv, aVendor);
	
	TInt res;
	TRAP(res, DoBringMidletToForegroundL(name->Des(), vendor->Des(), aNum));
	
	delete vendor;
	delete name;
	
	return res;
}

JNIEXPORT jint JNICALL Java_cc_nnproject_symbianext_PropertyExtensions__1getInstalledSisVersion
	(JNIEnv *aEnv, jclass, jint aUid, jintArray aOut)
{
	// TODO
	return KErrNotSupported;
}
