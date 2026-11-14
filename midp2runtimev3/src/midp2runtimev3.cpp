#include <e32base.h>
#include <hal.h>
#include <hal_data.h>
#include <etel3rdparty.h>
#include <bautils.h>
#include "common.h"
#ifndef UIQ
#include "javamobinfo.h"
#endif

#ifdef EKA2
#define sizeof_Runtime 48
#define sizeof_Runtime_vtable 64
#else
#define sizeof_Runtime 30
#define sizeof_Runtime_vtable 64
#endif

_LIT(KIsAppInstalledPrefix, "cc.nnproject.symbianext.isAppInstalled:");
_LIT(KGetInstalledSisVersionPrefix, "cc.nnproject.symbianext.getInstalledSisVersion:");
_LIT(KLaunchAppPrefix, "cc.nnproject.symbianext.launchApp:");
_LIT(KGetMidletUidPrefix, "cc.nnproject.symbianext.getMidletUid:");
_LIT(KLaunchMidletPrefix, "cc.nnproject.symbianext.launchMidlet:");
_LIT(KGetMidletSuiteVersionPrefix, "cc.nnproject.symbianext.getMidletSuiteVersion:");
_LIT(KIsAppRunningPrefix, "cc.nnproject.symbianext.isAppRunning:");
_LIT(KBringAppToForegroundPrefix, "cc.nnproject.symbianext.bringAppToForeground:");
_LIT(KBringMidletToForegroundPrefix, "cc.nnproject.symbianext.bringMidletToForeground:");
_LIT(KSetGlobalDataPrefix, "cc.nnproject.symbianext.setGlobalData:");
_LIT(KGetGlobalDataPrefix, "cc.nnproject.symbianext.getGlobalData:");
_LIT(KTrue, "true");
_LIT(KFalse, "false");
_LIT(KApiVersion, "1.0");

typedef HBufC* (*TPropertyFunction0)(void*);
typedef HBufC* (*TPropertyFunction1)(void*, TDesC*);

static TBool ParseInt(TPtrC aDes, TUint32& aVal) {
	TLex lexer(aDes);
	TBool hex = aDes.Find(_L("0x")) == 0;
	if (hex) lexer.Inc(2);
	return lexer.Val(aVal, hex ? EHex : EDecimal) == KErrNone;
}

static HBufC* GetVersionL() {
	HBufC* res = HBufC16::NewLC(8);
	res->Des().Append(KApiVersion);
	CleanupStack::Pop();
	return res;
}

static HBufC* GetVariantL() {
	HBufC* res = HBufC16::NewLC(10);
	// jrt1 on j9
#ifdef EKA2
	res->Des().Append(_L("hmidp_eka2"));
#else
	res->Des().Append(_L("hmidp_eka1"));
#endif
	CleanupStack::Pop();
	return res;
}

static HBufC* GetFreeRamL() {
	HBufC* res = HBufC::NewLC(16);
	int tmp;
	HAL::Get(HALData::EMemoryRAMFree, tmp);
	res->Des().AppendNum((TInt)tmp);
	CleanupStack::Pop();
	return res;
}

static HBufC* GetTotalRamL() {
	HBufC* res = HBufC::NewLC(16);
	int tmp;
	HAL::Get(HALData::EMemoryRAM, tmp);
	res->Des().AppendNum((TInt)tmp);
	CleanupStack::Pop();
	return res;
}

static TInt RunTelephonyThread(TAny* aParam) {
	HBufC* res = (HBufC*) aParam;
	TBool success = EFalse;
	TInt error = KErrNone;
	
	CTrapCleanup* cleanup = CTrapCleanup::New();
	CActiveScheduler* scheduler = new CActiveScheduler();
	CActiveScheduler::Install(scheduler);
	
	CTelephony* telephony = NULL;
	TRAP(error, telephony = CTelephony::NewL());
	if (error == KErrNone && telephony != NULL) {
		CTelephony::TBatteryInfoV1 info;
		CTelephony::TBatteryInfoV1Pckg infoPkg(info);
		TRequestStatus status;
		telephony->GetBatteryInfo(status, infoPkg);
		
		while (CActiveScheduler::RunIfReady(error, CActive::EPriorityIdle) || status.Int() == KRequestPending) {
			;
		}
		
		if (status.Int() == KErrNone) {
			res->Des().AppendNum((TInt)info.iChargeLevel);
			success = ETrue;
		}
		
		delete telephony;
	}

	CActiveScheduler::Install(NULL);
	delete scheduler;
	delete cleanup;
	
	return success ? 0 : 1;
}

static HBufC* GetBatteryLevelL() {
	HBufC* res = HBufC16::NewL(16);
	
	RThread thread;
	if (thread.Create(KNullDesC, RunTelephonyThread, 0x2000, NULL, res) != KErrNone) {
		delete res;
		return NULL;
	}
	thread.Resume();
	
	TRequestStatus status;
	thread.Logon(status);
	User::WaitForRequest(status);
	
	if (thread.ExitReason() != 0) {
		delete res;
		res = NULL;
	}
	
	thread.Close();
	
	return res;
}

static HBufC* CheckTlsL() {
	HBufC* res = NULL;
#ifdef EKA2
	if (DoCheckMbedTlsL()) {
		res = _L("mbedtls").AllocL();
	} else {
		res = HBufC16::NewLC(5);
		res->Des().Append(KFalse);
		CleanupStack::Pop();
	}
#else
	RFs fs;
	if (fs.Connect() != KErrNone) return NULL;
	if (BaflUtils::FileExists(fs, _L("C:\\system\\libs\\ssl.dll"))
			|| BaflUtils::FileExists(fs, _L("C:\\system\\libs\\ssladapto2.dll"))
			|| BaflUtils::FileExists(fs, _L("C:\\system\\libs\\ssladaptor.dll"))) {
		res = HBufC16::NewLC(7);
		res->Des().Append(_L("bearssl"));
		CleanupStack::Pop();
	} else {
		res = HBufC16::NewLC(5);
		res->Des().Append(KFalse);
		CleanupStack::Pop();
	}
	fs.Close();
#endif
	return res;
}

static HBufC* IsAppInstalledL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KIsAppInstalledPrefix.iTypeLength) return NULL;
	
	TUint32 uid;
	if (!ParseInt(aProperty->Mid(KIsAppInstalledPrefix.iTypeLength), uid)) {
		return NULL;
	}
	
	HBufC* res = NULL;
	if (DoIsAppInstalledL(TUid::Uid(uid))) {
		res = HBufC16::NewLC(4);
		res->Des().Append(KTrue);
		CleanupStack::Pop();
	} else {
		res = HBufC16::NewLC(5);
		res->Des().Append(KFalse);
		CleanupStack::Pop();
	}
	return res;
}

static HBufC* IsAppRunningL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KIsAppRunningPrefix.iTypeLength) return NULL;
	
	TUint32 uid;
	if (!ParseInt(aProperty->Mid(KIsAppRunningPrefix.iTypeLength), uid)) {
		return NULL;
	}

	HBufC* res = NULL;
	if (DoIsAppRunningL(TUid::Uid(uid))) {
		res = HBufC16::NewLC(4);
		res->Des().Append(KTrue);
		CleanupStack::Pop();
	} else {
		res = HBufC16::NewLC(5);
		res->Des().Append(KFalse);
		CleanupStack::Pop();
	}
	return res;
}

static HBufC* BringAppToForegroundL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KBringAppToForegroundPrefix.iTypeLength) return NULL;
	
	// TODO
	
	TUint32 uid;
	if (!ParseInt(aProperty->Mid(KBringAppToForegroundPrefix.iTypeLength), uid)) {
		return NULL;
	}
	
	HBufC* res = NULL;
	if (DoBringAppToForegroundL(TUid::Uid(uid))) {
		res = HBufC16::NewLC(4);
		res->Des().Append(_L("true"));
		CleanupStack::Pop();
	} else {
		res = HBufC16::NewLC(5);
		res->Des().Append(_L("false")); // not found
		CleanupStack::Pop();
	}
	return res;
}

static HBufC* LaunchAppL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KLaunchAppPrefix.iTypeLength) return NULL;
	
	TPtrC param = aProperty->Mid(KLaunchAppPrefix.iTypeLength);
	
	TUint32 uid;
	TPtrC doc(KNullDesC);
	{
		TInt splitIdx = param.Locate(';');
		if (!ParseInt(splitIdx != -1 ? param.Left(splitIdx) : param, uid)) {
			return NULL;
		}
		
		if (splitIdx != -1) {
			doc.Set(param.Mid(splitIdx + 1));
		}
	}

	TRAPD(err, DoLaunchAppUidL(TUid::Uid(uid), doc));
	HBufC* res = res = HBufC16::NewLC(16);
	res->Des().AppendNum((TInt)err);
	CleanupStack::Pop();
	
	return res;
}

static HBufC* GetMidletSuiteVersionL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KGetMidletSuiteVersionPrefix.iTypeLength) return NULL;
	
	TPtrC param = aProperty->Mid(KGetMidletSuiteVersionPrefix.iTypeLength);
	TInt splitIdx = param.Locate(';');
	
	if (splitIdx == -1) return NULL;
	TPtrC name = param.Left(splitIdx);
	TPtrC vendor = param.Mid(splitIdx + 1);
	
	TAppVersion version = DoGetMidletSuiteVersionL(name, vendor);
	HBufC* res = HBufC16::NewLC(16);
	TPtr16 des = res->Des();
	des.AppendNum((TInt)version.iMajor);
	des.Append('.');
	des.AppendNum((TInt)version.iMinor);
	des.Append('.');
	des.AppendNum((TInt)version.iBuild);
	CleanupStack::Pop();
	
	return res;
}

static HBufC* GetMidletUidL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KGetMidletUidPrefix.iTypeLength) return NULL;
	
	TPtrC param = aProperty->Mid(KGetMidletUidPrefix.iTypeLength);
	TInt splitIdx = param.Locate(';');
	
	if (splitIdx == -1) return NULL;
	TPtrC name = param.Left(splitIdx);
	TPtrC vendor = param.Mid(splitIdx + 1);
	TUint32 midletId = 1;
	
	splitIdx = vendor.Locate(';');
	if (splitIdx != -1) {
		TPtrC midletIdDes = vendor.Mid(splitIdx + 1);
		vendor.Set(vendor.Left(splitIdx));
		
		TLex lexer(midletIdDes);
		if (lexer.Val(midletId, EDecimal) != KErrNone) return NULL;
	}
	
	TInt32 uid = DoGetMidletUidL(name, vendor, midletId);
	HBufC* res = HBufC16::NewLC(16);
	res->Des().AppendFormat(_L("0x%x"), uid);
	CleanupStack::Pop();
	return res;
}

static HBufC* LaunchMidletL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KLaunchMidletPrefix.iTypeLength) return NULL;
	
	TPtrC param = aProperty->Mid(KLaunchMidletPrefix.iTypeLength);
	TInt splitIdx = param.Locate(';');
	
	if (splitIdx == -1) return NULL;
	TPtrC name = param.Left(splitIdx);
	TPtrC vendor = param.Mid(splitIdx + 1);
	TUint32 midletId = 1;
	TPtrC cmd(KNullDesC);
	
	splitIdx = vendor.Locate(';');
	if (splitIdx != -1) {
		TPtrC midletIdDes = vendor.Mid(splitIdx + 1);
		vendor.Set(vendor.Left(splitIdx));
		
		splitIdx = midletIdDes.Locate(';');
		if (splitIdx != -1) {
			cmd.Set(midletIdDes.Mid(splitIdx + 1));
			midletIdDes.Set(midletIdDes.Left(splitIdx));
		}
		
		TLex lexer(midletIdDes);
		if (lexer.Val(midletId, EDecimal) != KErrNone) return NULL;
	}
	
	TRAPD(err, DoLaunchMidletL(param, name, vendor, midletId))
	HBufC* res = HBufC16::NewLC(16);
	res->Des().AppendNum((TInt)err);
	CleanupStack::Pop();
	return res;
}

static HBufC* BringMidletToForegroundL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KBringMidletToForegroundPrefix.iTypeLength) return NULL;

	TPtrC param = aProperty->Mid(KBringMidletToForegroundPrefix.iTypeLength);
	TInt splitIdx = param.Locate(';');
	
	if (splitIdx == -1) return NULL;
	TPtrC name = param.Left(splitIdx);
	TPtrC vendor = param.Mid(splitIdx + 1);
	TUint32 midletId = 1;
	
	splitIdx = vendor.Locate(';');
	if (splitIdx != -1) {
		TPtrC midletIdDes = vendor.Mid(splitIdx + 1);
		vendor.Set(vendor.Left(splitIdx));
		
		TLex lexer(midletIdDes);
		if (lexer.Val(midletId, EDecimal) != KErrNone) return NULL;
	}
	
	// TODO
	TRAPD(err, DoBringMidletToForegroundL(name, vendor, midletId));
	HBufC* res = HBufC16::NewLC(16);
	res->Des().AppendNum((TInt)err);
	CleanupStack::Pop();
	
	return res;
}

static HBufC* SetGlobalDataL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KSetGlobalDataPrefix.iTypeLength) return NULL;

	TPtrC param = aProperty->Mid(KSetGlobalDataPrefix.iTypeLength);
	TInt splitIdx = param.Locate(';');
	
	if (splitIdx == -1) return NULL;
	TPtrC name = param.Left(splitIdx);
	TPtrC data = param.Mid(splitIdx + 1);
	if (name.Length() == 0 || name.Length() > 32 || data.Length() > 1024) return NULL; 
	
	HBufC* nameBuf = HBufC::NewLC(name.Length() + 10);
	TPtr name2 = nameBuf->Des();
	name2.Append(_L("MIDP2Data."));
	name2.Append(name);
	
	TRAPD(err, DoSetGlobalDataL(name2, data));
	HBufC* res = HBufC16::NewLC(16);
	res->Des().AppendNum((TInt)err);
	CleanupStack::Pop();
	
	CleanupStack::PopAndDestroy(); // nameBuf
	
	return res;
}

static HBufC* GetGlobalDataL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KGetGlobalDataPrefix.iTypeLength) return NULL;

	TPtrC name = aProperty->Mid(KGetGlobalDataPrefix.iTypeLength);
	if (name.Length() == 0 || name.Length() > 32) return NULL; 
	
	HBufC* nameBuf = HBufC::NewLC(name.Length() + 10);
	TPtr name2 = nameBuf->Des();
	name2.Append(_L("MIDP2Data."));
	name2.Append(name);
	
	HBufC* res = DoGetGlobalDataL(name2);
	
	CleanupStack::PopAndDestroy(); // nameBuf
	
	return res;
}

#ifdef EKA2
static HBufC* GetInstalledSisVersionL(TDesC* aProperty) {
	if ((TUint32)aProperty->Length() <= KGetInstalledSisVersionPrefix.iTypeLength) return NULL;

	TUint32 uid;
	if (!ParseInt(aProperty->Mid(KGetInstalledSisVersionPrefix.iTypeLength), uid)) {
		return NULL;
	}
	
	TVersion version = DoGetInstalledSisVersionL(TUid::Uid(uid));
	HBufC* res = res = HBufC::NewLC(16);
	TPtr16 des = res->Des();
	des.AppendNum((TInt)version.iMajor);
	des.Append('.');
	des.AppendNum((TInt)version.iMinor);
	des.Append('.');
	des.AppendNum((TInt)version.iBuild);
	CleanupStack::Pop();
	
	return res;
}
#endif

HBufC* GetDynamicProperty(void* aThis, TDesC* aProperty) {
	HBufC* res = NULL;
#ifndef EKA2
	if (!aProperty->Compare(_L("microedition.locale"))) {
			TRAP_IGNORE(res = (*(TPropertyFunction0**)aThis)[4](aThis));
			if (res != NULL) return res->Alloc();
			return NULL;
		}
		if (!aProperty->Compare(_L("microedition.timezone"))) {
			TRAP_IGNORE(res = (*(TPropertyFunction0**)aThis)[5](aThis));
			if (res != NULL) return res->Alloc();
			return NULL;
		}
#endif
	if (!aProperty->Compare(_L("com.nokia.memoryramfree"))) {
		TRAP_IGNORE(res = GetFreeRamL());
		return res;
	}
	if (!aProperty->Compare(_L("com.nokia.memoryramtotal"))) { // imaginary
		TRAP_IGNORE(res = GetTotalRamL());
		return res;
	}
	if (!aProperty->Compare(_L("com.nokia.mid.batterylevel"))) {
		TRAP_IGNORE(res = GetBatteryLevelL());
		return res;
	}
#ifndef UIQ
	if (!aProperty->Compare(_L("com.nokia.network.access"))) {
		TRAP_IGNORE(res = GetNetworkAccessL());
		return res;
	}
#endif
	if (!aProperty->Compare(_L("cc.nnproject.symbianext.version"))) {
		TRAP_IGNORE(res = GetVersionL());
		return res;
	}
	if (!aProperty->Compare(_L("cc.nnproject.symbianext.variant"))) {
		TRAP_IGNORE(res = GetVariantL());
		return res;
	}
	if (!aProperty->Compare(_L("cc.nnproject.symbianext.checkTLS12"))) {
		TRAP_IGNORE(res = CheckTlsL());
		return res;
	}
	if (aProperty->Find(KIsAppInstalledPrefix) == 0) {
		TRAP_IGNORE(res = IsAppInstalledL(aProperty));
		return res;
	}
	if (aProperty->Find(KIsAppRunningPrefix) == 0) {
		TRAP_IGNORE(res = IsAppRunningL(aProperty));
		return res;
	}
	if (aProperty->Find(KLaunchAppPrefix) == 0) {
		TRAP_IGNORE(res = LaunchAppL(aProperty));
		return res;
	}
	if (aProperty->Find(KBringAppToForegroundPrefix) == 0) {
		TRAP_IGNORE(res = BringAppToForegroundL(aProperty));
		return res;
	}
	if (aProperty->Find(KGetMidletUidPrefix) == 0) {
		TRAP_IGNORE(res = GetMidletUidL(aProperty));
		return res;
	}
	if (aProperty->Find(KGetMidletSuiteVersionPrefix) == 0) {
		TRAP_IGNORE(res = GetMidletSuiteVersionL(aProperty));
		return res;
	}
	if (aProperty->Find(KLaunchMidletPrefix) == 0) {
		TRAP_IGNORE(res = LaunchMidletL(aProperty));
		return res;
	}
	if (aProperty->Find(KBringMidletToForegroundPrefix) == 0) {
		TRAP_IGNORE(res = BringMidletToForegroundL(aProperty));
		return res;
	}
	if (aProperty->Find(KSetGlobalDataPrefix) == 0) {
		TRAP_IGNORE(res = SetGlobalDataL(aProperty));
		return res;
	}
	if (aProperty->Find(KGetGlobalDataPrefix) == 0) {
		TRAP_IGNORE(res = GetGlobalDataL(aProperty));
		return res;
	}
#ifdef EKA2
	if (aProperty->Find(KGetInstalledSisVersionPrefix) == 0) {
		TRAP_IGNORE(res = GetInstalledSisVersionL(aProperty));
		return res;
	}

	return (*(TPropertyFunction1**)aThis)[sizeof_Runtime_vtable >> 2](aThis, aProperty);
#else
	return NULL;
#endif
}

EXPORT_C void* GetRuntimeL() {
	RLibrary lib;
	User::LeaveIfError(lib.Load(_L("midp2runtimev2.dll")));
	unsigned int* runtime = ((unsigned int* (*)()) lib.Lookup(1))();
	User::LeaveIfNull(runtime);
	runtime -= 1;

	unsigned int* vtable = (unsigned int*) User::AllocL(sizeof_Runtime_vtable + 4);
	Mem::Copy(vtable, (void*)runtime[1], sizeof_Runtime_vtable);

#ifdef EKA2
	vtable[sizeof_Runtime_vtable >> 2] = vtable[11]; // store pointer to original function
	vtable[11] = (unsigned int)GetDynamicProperty;
#else
	vtable[16] = (unsigned int)GetDynamicProperty;
#endif
	runtime[1] = (unsigned int)vtable;
	
	return runtime + 1;
}

#ifndef EKA2
TInt E32Dll(TDllReason) {
	return 0;
}
#endif
