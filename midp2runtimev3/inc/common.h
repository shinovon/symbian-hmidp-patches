/*
Copyright (c) 2026 Arman Jussupgaliyev
*/

#include <e32base.h>
#include <apacmdln.h>
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
void DoLaunchAppUidL(TUid aUid, /*TApaCommand aCommand, */const TDesC& aDoc = KNullDesC());
void DoBringAppToForegroundL(TUid aUid, TInt aSendMessage = 0, TUid aMessageId = TUid::Null(), TPtrC aDoc = KNullDesC());

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
