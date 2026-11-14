/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  javamobinfo implementation
 *
*/

#include <rconnmon.h>

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <es_sock.h>
#include <es_enum.h>
#else
#include <es_enum_internal.h>
#include <es_sock_partner.h>
#endif

_LIT(KNetworkAccessPdLAN, "lan"); // Local Area Network
_LIT(KNetworkAccessPdGSM, "pd"); // Packet data in GSM network
_LIT(KNetworkAccessPdEDGE, "pd.EDGE"); // Packet data in EDGE network
_LIT(KNetworkAccessPd3G, "pd.3G"); // Packet data in 3G network
_LIT(KNetworkAccessHSPDA, "pd.HSPDA"); // High speed downlink packet access for 3G networks
_LIT(KNetworkAccessCSD, "csd"); // Circuit switched data, for example GSM CSD/HSCSD data call
_LIT(KNetworkAccessBtPan, "bt_pan"); // Bluetooth PAN network
_LIT(KNetworkAccessWLAN, "wlan"); // Wireless network, for example 802.11b, 802.11G or WiMaX
_LIT(KNetworkAccessNa, "na"); // Not applicable, the implementation can’t determine the type

static TUint32 GetUsedIapL()
{
	RSocketServ socketServer;
	RConnection connection;
	TConnEnumArgBuf enumArgBuf;
	TConnGetClientInfoArgBuf clientInfoBuf;
	TUint connectionCount(0);
	TConnectionInfoBuf connInfoBuf;
	TUint32 iapId(0);

	// Get MIDlet process ID
	RProcess process;
	RThread thread;
	TProcessId currProcessId(process.Id());
	TThreadId currThreadId(thread.Id());

	// Connect to socket server and use system global pool (-1)
	// for message slots.
	User::LeaveIfError(socketServer.Connect(static_cast<TUint> (-1)));
	CleanupClosePushL(socketServer);

	// Open RConnection,
	// this object is not attached to any connection
	User::LeaveIfError(connection.Open(socketServer));
	CleanupClosePushL(connection);

	// Get count of current active connections.

	// Go through all active connections
	for (TUint connIdx = 1; connIdx <= connectionCount && iapId == 0; connIdx++) {
		enumArgBuf().iIndex = connIdx;

		// Get the number of clients
		User::LeaveIfError(
			connection.Control(KCOLConnection, KCoEnumerateConnectionClients, enumArgBuf));

		// Go through all clients
		for (TUint clientIdx = 1; clientIdx <= enumArgBuf().iCount && iapId == 0; clientIdx++) {
			clientInfoBuf().iIndex = clientIdx;
			User::LeaveIfError(
				connection.Control(KCOLConnection, KCoGetConnectionClientInfo, clientInfoBuf));

			TUid uid(clientInfoBuf().iClientInfo.iUid);
			TProcessId processId(clientInfoBuf().iClientInfo.iProcessId);
			TThreadId threadId(clientInfoBuf().iClientInfo.iThreadId);

			// MIDlet's active connection found
			if (/*uid.iUid == KJVMProcessUid &&*/ currProcessId == processId) {
				TConnectionInfoBuf connInfoBuf;

				User::LeaveIfError(connection.GetConnectionInfo(connIdx, connInfoBuf));
				iapId = connInfoBuf().iIapId;
			}
		}
	}

	CleanupStack::PopAndDestroy(&connection);
	CleanupStack::PopAndDestroy(&socketServer);
	return iapId;
}

static HBufC* TranslateBearerInfoToNetworkAccessL(TConnMonBearerType aBearerInfo)
{
    HBufC* bearerType(NULL);
    switch (aBearerInfo)
    {
    case EBearerLAN:
        bearerType = KNetworkAccessPdLAN().AllocL();
        break;
    case EBearerGPRS:
        bearerType = KNetworkAccessPdGSM().AllocL();
        break;
    case EBearerEdgeGPRS:
        bearerType = KNetworkAccessPdEDGE().AllocL();
        break;
    case EBearerWCDMA:
        bearerType = KNetworkAccessPd3G().AllocL();
        break;
    case EBearerCSD:
    case EBearerHSCSD:
    case EBearerWcdmaCSD:
        bearerType = KNetworkAccessCSD().AllocL();
        break;
    case EBearerBluetooth:
        bearerType = KNetworkAccessBtPan().AllocL();
        break;
    case EBearerWLAN:
        bearerType = KNetworkAccessWLAN().AllocL();
        break;
//    case EBearerHSDPA:
//    case EBearerHSUPA:
//    case EBearerHSxPA:
//        bearerType = KNetworkAccessHSPDA().AllocL();
//        break;
    case EBearerUnknown:
    case EBearerCDMA2000:
    case EBearerVirtual:
    case EBearerVirtualVPN:
    default:
        bearerType = KNetworkAccessNa().AllocL();
        break;
    }
    return bearerType;
}

static HBufC* GetBearerTypeL(TUint32 aIapId)
{
    RConnectionMonitor      monitor;
    TUint                   connectionCount(0);
    TUint                   iapId(0);
    TRequestStatus          status(KErrNone);
    HBufC*                  bearerType(NULL);
    TUint                   connectionId(0);
    TUint                   subConnectionCount(0);
    TConnMonBearerType      bearerInfo(EBearerUnknown);

    User::LeaveIfError(monitor.ConnectL());
    CleanupClosePushL(monitor);

    status = KRequestPending;
    monitor.GetConnectionCount(connectionCount, status);
    User::WaitForRequest(status);
    User::LeaveIfError(status.Int());

    // Go through all connections
    for (TUint connIdx = 1; connIdx <= connectionCount && iapId == 0;
            ++connIdx)
    {
        connectionId = 0;
        subConnectionCount = 0;

        // Get connection ID
        monitor.GetConnectionInfo(connIdx, connectionId, subConnectionCount);
        User::LeaveIfError(connectionId);

        // Get connection IAP ID
        status = KRequestPending;
        monitor.GetUintAttribute(connectionId, 0, KIAPId, iapId, status);
        User::WaitForRequest(status);
        User::LeaveIfError(status.Int());

        if (iapId == aIapId)
        {
            // MIDlet's IAP found

            // Get bearer type
            status = KRequestPending;
            monitor.GetIntAttribute(connectionId,
                                    0,
                                    KBearer,
                                    (TInt&)bearerInfo,
                                    status);
            User::WaitForRequest(status);
            User::LeaveIfError(status.Int());
        }
    }

    // Translate bearer type to type of network access
    bearerType = TranslateBearerInfoToNetworkAccessL(bearerInfo);

    CleanupStack::PopAndDestroy(&monitor);
    return bearerType;
}

static HBufC* GetNetworkAccessL(/*TUint32 aAppDefaultApn, const TUint32 aAppDefaultApType*/)
{
    HBufC* bearerType = NULL;
    TUint32 iapId = GetUsedIapL();
    if (iapId != 0)
    {
        // active connection present, opened by MIDlet.
        bearerType = GetBearerTypeL(iapId);

    }
    else
    {
        // if there is no active connection by the application, return the app default bearer type.
//        if (aAppDefaultApType == 3)
//        {
//            // app default is IAP
//            bearerType = GetAppDefIapBearerTypeL(aAppDefaultApn);
//
//        }
//        else
//        {
//            bearerType = GetIapBearerTypeL(aAppDefaultApn,aAppDefaultApType);
//        }
    	bearerType = KNetworkAccessNa().AllocL();
    }
    return bearerType;
}
