#include <e32base.h>

class MMIDEnv {
public:
    virtual TBool PostJavaEvent1();
    virtual TBool PostJavaEvent2();
    virtual TBool PostJavaEvent3();
    virtual TBool PostMidletEvent(TInt aEventType);
    virtual void NewNotifyL();
    virtual void PostJavaNotify();
    virtual void TranslateKeyL();
    virtual void PostKeyEvent();
    virtual void ResetKeys();
    virtual void Current();
    virtual void MidletName();
    virtual void MidletUid();
	virtual void MidletHome();
	virtual void NumColors();
	virtual void DisplayMode();
	virtual void SetCanvasZoomSize();
	virtual void CanvasZoomSize();
	virtual void CanvasAssumedSize();
	virtual void AddObserverL();
	virtual void RemoveObserver();
#ifdef EKA2
	virtual TInt MidletAttribute(const TDesC& aAttributeName, TPtrC& aAttributeValue);
	virtual void SetCanvasAssumedSize();
#endif
};

#ifdef EKA2
#define CMIDToolkit_MMIDObserver_offset 5
#define CMIDToolkit_MMIDObserver_size 16
#define CMIDToolkit_iEnv 23
#else
#define CMIDToolkit_MMIDObserver_offset 6
#define CMIDToolkit_MMIDObserver_size 24
#define CMIDToolkit_iEnv 22
#endif

static int HandleForegroundEventHook(int* aThis, int foreground) {
	int r = (((int (*)(int*, int)) (*(int*)(*aThis + CMIDToolkit_MMIDObserver_size))))(aThis, foreground);
	
	MMIDEnv* env = (MMIDEnv*) ((aThis - CMIDToolkit_MMIDObserver_offset)[CMIDToolkit_iEnv] + 4);
#ifdef EKA2
	TPtrC attributeValue;
	TInt err = env->MidletAttribute(_L("Nokia-MIDlet-Background-Event"), attributeValue);
	if (err != KErrNone || attributeValue.Compare(_L("pause")))
		return r;
#endif
		
	if (foreground) {
		env->PostMidletEvent(22);
	} else {
		env->PostMidletEvent(21);
	}
	return r;
}

EXPORT_C void* NewFactoryL(int* a, int b) {
	RLibrary lib;
	User::LeaveIfError(lib.Load(_L("lcdgr")));
#ifdef EKA2
	void* r = ((void* (*)(int, int)) lib.Lookup(1))((a[20] + 40), b);
	int* toolkit = a;
	int* vtable = (int*) User::AllocL(CMIDToolkit_MMIDObserver_size + 4);
	Mem::Copy(vtable, ((int**)toolkit)[CMIDToolkit_MMIDObserver_offset], CMIDToolkit_MMIDObserver_size);
	vtable[4] = vtable[1];
	vtable[1] = (int)HandleForegroundEventHook;
#else
	void* r = ((void* (*)(int, int)) lib.Lookup(1))((int) a, b);
	int* toolkit = (int*) (a - 1)[2];
	int* vtable = (int*) User::AllocL(CMIDToolkit_MMIDObserver_size + 4);
	Mem::Copy(vtable, ((int**)toolkit)[CMIDToolkit_MMIDObserver_offset], CMIDToolkit_MMIDObserver_size);
	vtable[6] = vtable[3];
	vtable[3] = (int)HandleForegroundEventHook;
#endif
	((int**) toolkit)[CMIDToolkit_MMIDObserver_offset] = vtable;
	
	return r;
}

#ifndef EKA2
TInt E32Dll(TDllReason) {
	return 0;
}
#endif
