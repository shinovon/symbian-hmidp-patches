#include "common.h"
#include <apgcli.h>
#include <apgtask.h>
#include <apacmdln.h>
#include <w32std.h>
#include "mjavaregistry.h"
#ifdef EKA2
#include <swi/sisregistrysession.h>
#include <swi/sisregistryentry.h>
#endif

#ifdef EKA2
TBool DoCheckMbedTlsL() {
	TBool res = EFalse;
	
	Swi::RSisRegistrySession session;
	if (session.Connect() == KErrNone) {
		CleanupClosePushL(session);
		
		Swi::RSisRegistryEntry entry;
		CleanupClosePushL(entry);
		res = entry.Open(session, TUid::Uid(0x20292B6B)) == KErrNone;

		CleanupStack::PopAndDestroy(); // entry
		CleanupStack::PopAndDestroy(); // session
		
		if (res) return ETrue;
	}
	
	// may be installed manually
	RLibrary lib;
	if (lib.Load(_L("mbedtls.dll")) == KErrNone) {
		res = ETrue;
		
		lib.Close();
	}
	
	return res;
}
#endif

TBool DoIsAppInstalledL(TUid aUid) {
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);
	
	TApaAppInfo appInfo;
	TBool res = ls.GetAppInfo(appInfo, aUid) == KErrNone;
	
	CleanupStack::PopAndDestroy(); // ls
	
	return res;
}

TBool DoIsAppRunningL(TUid aUid) {
	RWsSession ws;
	User::LeaveIfError(ws.Connect());
	CleanupClosePushL(ws);
		
	TApaTaskList tasklist(ws);
	TApaTask task = tasklist.FindApp(aUid);
	TBool res = task.Exists();

	CleanupStack::PopAndDestroy(); // ws
	
	return res;
}

void DoLaunchAppUidL(TUid aUid, const TDesC& aDoc) {
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);
	
	TApaAppInfo appInfo;
	User::LeaveIfError(ls.GetAppInfo(appInfo, aUid));
	
	if (aDoc.Length() != 0) {
		TThreadId tid;
		User::LeaveIfError(ls.StartDocument(aDoc, aUid, tid));
	} else {
		CApaCommandLine* cli = CApaCommandLine::NewLC();
		cli->SetExecutableNameL(appInfo.iFullName);
//		if (doc.Length() != 0) {
//			cli->SetDocumentNameL(doc);
//		}
		User::LeaveIfError(ls.StartApp(*cli));
		CleanupStack::PopAndDestroy(); // cli
	}

	CleanupStack::PopAndDestroy(); // ls
}

TBool DoBringAppToForegroundL(TUid aUid, TBool aSendMessage, TUid aMessageId, const TDesC8& aData) {
	RWsSession ws;
	User::LeaveIfError(ws.Connect());
	CleanupClosePushL(ws);
		
	TApaTaskList tasklist(ws);
	TApaTask task = tasklist.FindApp(aUid);

	TBool res = false;
	if (task.Exists()) {
		task.BringToForeground();
		res = true;
		if (aSendMessage) task.SendMessage(aMessageId, aData);
	}

	CleanupStack::PopAndDestroy(); // ws
	
	return res;
}

TInt32 DoGetMidletUidL(TPtrC aName, TPtrC aVendor, TUint32 aMidletId) {
	RLibrary lib;
	User::LeaveIfError(lib.Load(_L("javaregistry.dll")));
	CleanupClosePushL(lib);
	
	void* func = (void*) lib.Lookup(MJavaRegistry_CreateL);
	if (!func) User::Leave(KErrNotSupported);
	
	MJavaRegistry* registry = ((MJavaRegistry* (*)(void))func)();
	User::LeaveIfNull(registry);
	CleanupReleasePushL(*registry);

	MJavaRegistrySuiteEntry* suite = registry->SuiteEntryL(aName, aVendor);
	MJavaRegistryMIDletEntry* midlet = suite->MIDletByNumberL(aMidletId);
	TUid uid = midlet->UidL();

	TInt32 res = midlet->UidL().iUid;
	
	CleanupStack::PopAndDestroy(); // registry
	CleanupStack::PopAndDestroy(); // lib
	
	return res;
}

TAppVersion DoGetMidletSuiteVersionL(TPtrC aName, TPtrC aVendor) {
	RLibrary lib;
	User::LeaveIfError(lib.Load(_L("javaregistry.dll")));
	CleanupClosePushL(lib);
	
	void* func = (void*) lib.Lookup(MJavaRegistry_CreateL);
	if (!func) User::Leave(KErrNotSupported);
	
	MJavaRegistry* registry = ((MJavaRegistry* (*)(void))func)();
	User::LeaveIfNull(registry);
	CleanupReleasePushL(*registry);
	
	MJavaRegistrySuiteEntry* entry = registry->SuiteEntryL(aName, aVendor);
	
	TAppVersion version = entry->VersionL();
	
	CleanupStack::PopAndDestroy(); // registry
	CleanupStack::PopAndDestroy(); // lib
	
	return version;
}

void DoLaunchMidletL(TPtrC aParam, TPtrC aName, TPtrC aVendor, TUint32 aMidletId) {
	RLibrary lib;
	User::LeaveIfError(lib.Load(_L("javaregistry.dll")));
	CleanupClosePushL(lib);
		
	void* func = (void*) lib.Lookup(MJavaRegistry_CreateL);
	if (!func) User::Leave(KErrNotSupported);
	
	MJavaRegistry* registry = ((MJavaRegistry* (*)(void))func)();
	User::LeaveIfNull(registry);
	CleanupReleasePushL(*registry);
	
	MJavaRegistrySuiteEntry* suite = registry->SuiteEntryL(aName, aVendor);
	MJavaRegistryMIDletEntry* midlet = suite->MIDletByNumberL(aMidletId);
	
	if (DoIsAppRunningL(midlet->UidL())) {
		User::Leave(KErrAlreadyExists);
	}
	
	RProcess process;
	CleanupClosePushL(process);
	User::LeaveIfError(process.Create(_L("midp2launcher.exe"), aParam));
	
	TRequestStatus status;
	process.Logon(status);
	process.Resume();
	User::WaitForRequest(status);
	
	TInt reason = process.ExitReason();
	if (reason != 0) User::Leave(reason);

	CleanupStack::PopAndDestroy(); // process
	CleanupStack::PopAndDestroy(); // registry
	CleanupStack::PopAndDestroy(); // lib
}

void DoBringMidletToForegroundL(TPtrC aName, TPtrC aVendor, TUint32 aMidletId) {
	RLibrary lib;
	User::LeaveIfError(lib.Load(_L("javaregistry.dll")));
	CleanupClosePushL(lib);
	
	void* func = (void*) lib.Lookup(MJavaRegistry_CreateL);
	if (!func) User::Leave(KErrNotSupported);
	
	MJavaRegistry* registry = ((MJavaRegistry* (*)(void))func)();
	User::LeaveIfNull(registry);
	CleanupReleasePushL(*registry);

	MJavaRegistrySuiteEntry* suite = registry->SuiteEntryL(aName, aVendor);
	MJavaRegistryMIDletEntry* midlet = suite->MIDletByNumberL(aMidletId);
	TUid uid = midlet->UidL();

	if (!DoBringAppToForegroundL(uid)) User::Leave(KErrNotFound);
	
	CleanupStack::PopAndDestroy(); // registry
	CleanupStack::PopAndDestroy(); // lib
}

void DoCreateGlobalDataL(TPtrC aName) {
	RChunk chunk;
	User::LeaveIfError(chunk.CreateGlobal(aName, sizeof(TInt), 1024 * sizeof(TUint16) + sizeof(TInt)));
	
	// let chunk handle leak
}

void DoSetGlobalDataL(TPtrC aName, TPtrC aData) {
	TInt size = sizeof(TInt) + aData.Size();
	
	RChunk chunk;
//	TInt err = chunk.CreateGlobal(aName, size, 1024 * sizeof(TUint16) + sizeof(TInt));
//	if (err == KErrAlreadyExists) {
//		err = chunk.OpenGlobal(aName, EFalse);
//	}
	User::LeaveIfError(chunk.OpenGlobal(aName, EFalse));
	CleanupClosePushL(chunk);
	
//	if (size > chunk.Size()) {
	User::LeaveIfError(chunk.Adjust(size));
//	}
	
	TUint8* base = chunk.Base();
	*reinterpret_cast<TInt*>(base) = aData.Length();
	TUint16* destBuf = reinterpret_cast<TUint16*>(base + sizeof(TInt));
	TInt destLen = (chunk.Size() - sizeof(TInt)) / sizeof(TUint16);
	TPtr dest(destBuf, destLen);
	dest.Copy(aData);
	
	CleanupStack::PopAndDestroy(); // chunk
}

HBufC* DoGetGlobalDataL(TPtrC aName) {
	RChunk chunk;
	User::LeaveIfError(chunk.OpenGlobal(aName, EFalse));
	CleanupClosePushL(chunk);
	
	if (chunk.Size() < sizeof(TInt)) User::Leave(KErrUnderflow);
	
	const TUint8* base = chunk.Base();
	TInt len = *reinterpret_cast<const TInt*>(base);
	if (len < 0 || len > 1024) User::Leave(KErrCorrupt);
	
	TPtrC16 data(reinterpret_cast<const TUint16*>(base + sizeof(TInt)), len);
	HBufC* res = data.AllocL();
	
	CleanupStack::PopAndDestroy(); // chunk
	
	return res;
}

#ifdef EKA2
TVersion DoGetInstalledSisVersionL(TUid aUid) {
	Swi::RSisRegistrySession session;
	CleanupClosePushL(session);
	User::LeaveIfError(session.Connect());
	
	Swi::RSisRegistryEntry entry;
	CleanupClosePushL(entry);
	User::LeaveIfError(entry.Open(session, aUid));
	
	TVersion res = entry.VersionL();

	CleanupStack::PopAndDestroy(); // entry
	CleanupStack::PopAndDestroy(); // session
	
	return res;
}
#endif
