/*
Copyright (c) 2026 Arman Jussupgaliyev
*/

#include <e32base.h>
#include "appversion.h"

#ifdef EKA2
#define MJavaRegistry_CreateL 5
#else
#define MJavaRegistry_CreateL 3
#endif

#ifdef EKA2
TBool DoCheckMbedTlsL();
#endif
TBool IsAppInstalled(TUid aUid);
TBool IsAppRunning(TUid aUid);
void DoLaunchAppUidL(TUid aUid, const TDesC& aDoc = KNullDesC());
TBool DoBringAppToForeground(TUid aUid, TBool aSendMessage = EFalse, TUid aMessageId = TUid::Null(), const TDesC8& aDoc = KNullDesC8());

TInt32 DoGetMidletUidL(TPtrC aName, TPtrC aVendor, TUint32 aMidletId);
TAppVersion DoGetMidletSuiteVersionL(TPtrC aName, TPtrC aVendor);
void DoLaunchMidletL(TPtrC aParam, TPtrC aName, TPtrC aVendor, TUint32 aMidletId);
void DoBringMidletToForegroundL(TPtrC aName, TPtrC aVendor, TUint32 aMidletId);
void DoCreateGlobalDataL(TPtrC aName);
void DoSetGlobalDataL(TPtrC aName, TPtrC aData);
HBufC* DoGetGlobalDataL(TPtrC aName);

#ifdef EKA2
TVersion DoGetInstalledSisVersionL(TUid aUid);
#endif
