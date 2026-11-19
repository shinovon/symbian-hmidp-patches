/*
Copyright (c) 2026 Arman Jussupgaliyev
*/

#include "common.h"
#include <apgcli.h>
#include <apgtask.h>
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

TBool IsAppInstalled(TUid aUid) {
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);
	
	TApaAppInfo appInfo;
	TBool res = ls.GetAppInfo(appInfo, aUid) == KErrNone;
	
	CleanupStack::PopAndDestroy(); // ls
	
	return res;
}

TBool IsAppRunning(TUid aUid) {
	RWsSession ws;
	User::LeaveIfError(ws.Connect());
	CleanupClosePushL(ws);
		
	TApaTaskList tasklist(ws);
	TApaTask task = tasklist.FindApp(aUid);
	TBool res = task.Exists();

	CleanupStack::PopAndDestroy(); // ws
	
	return res;
}

void DoLaunchAppUidL(TUid aUid, /*TApaCommand aCommand, */const TDesC& aDoc) {
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
//		if (aDoc.Length() != 0) {
//			cli->SetDocumentNameL(aDoc);
//		}
//		cli->SetCommandL(aCommand);
		
		User::LeaveIfError(ls.StartApp(*cli));
		CleanupStack::PopAndDestroy(); // cli
	}
	CleanupStack::PopAndDestroy(); // ls
}

void DoBringAppToForegroundL(TUid aUid, TInt aSendMessage, TUid aMessageId, TPtrC aData) {
	RWsSession ws;
	User::LeaveIfError(ws.Connect());
	CleanupClosePushL(ws);
		
	TApaTaskList tasklist(ws);
	TApaTask task = tasklist.FindApp(aUid);

	if (!task.Exists()) User::Leave(KErrNotFound);
	
	task.BringToForeground();
	if (aSendMessage == 1) {
		HBufC8* dataBuf = HBufC8::NewLC(aData.Length());
		dataBuf->Des().Append(aData);
		User::LeaveIfError(task.SendMessage(aMessageId, *dataBuf));
		CleanupStack::PopAndDestroy(); // dataBuf
	} else if (aSendMessage == 2) {
		TPtrC8 messageBuffer((TUint8*) aData.Ptr(), aData.Length()<<1);
		User::LeaveIfError(task.SendMessage(KUidApaMessageSwitchOpenFile, messageBuffer));
	}

	CleanupStack::PopAndDestroy(); // ws
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
	
	if (IsAppRunning(midlet->UidL())) {
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

	DoBringAppToForegroundL(uid);
	
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
	User::LeaveIfError(chunk.OpenGlobal(aName, EFalse));
	CleanupClosePushL(chunk);
	
	User::LeaveIfError(chunk.Adjust(size));
	
	TUint8* base = chunk.Base();
	*reinterpret_cast<TInt*>(base) = aData.Length();
	TUint16* destBuf = reinterpret_cast<TUint16*>(base + sizeof(TInt));
	TInt len = (chunk.Size() - sizeof(TInt)) / sizeof(TUint16);
	
	TPtr dest(destBuf, len);
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
