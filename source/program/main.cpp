#include "lib.hpp"
#include "nn/ro.h"

HOOK_DEFINE_TRAMPOLINE(MainHook){
    /* Define the callback for when the function is called. Don't forget to make it static and name it Callback. */
    static void Callback(){
        /* Call the original function, with the argument always being false. */
        svcOutputDebugString("run_repentance", 15);
        return Orig();
    }
};

HOOK_DEFINE_TRAMPOLINE(LoadModule) {

        static Result Callback(nn::ro::Module* pOutModule, const void* pImage, void* buffer, size_t bufferSize, int flag) {

            // Hooking all the functions here, as most of them are loaded from external we need to wait 
            // for the library to be loaded, then inject.

            Result ret = Orig(pOutModule, pImage, buffer, bufferSize, 1);
            uintptr_t pointer = 0;
            // Hook run_repentance function (Just to test if the hooking works) 
            nn::ro::LookupModuleSymbol(&pointer, pOutModule, "_Z14run_repentancev");
            MainHook::InstallAtPtr(pointer);

            return ret;
    }
};
extern "C" {
    // Draw string ??
    // void _ZNK4KAGE8Graphics4Font16DrawStringScaledEPKtffffNS0_5ColorEjb(float param_1,float param_2,float param_3,float param_4,long param_5,ushort *param_6 ,long param_7,uint param_8,uint param_9);
	//  Player Constructor
    // void _ZN15IsaacRepentance13Entity_PlayerC1Ev();
}

namespace nn::fs {
    /*
        If not set to true, instead of returning result with error code
        in case of any fs function failing, Application will abort.
    */
    Result SetResultHandledByApplication(bool enable);
};


extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking enviroment. */
    exl::hook::Initialize();

    /* Install the hook at the provided function pointer. Function type is checked against the callback function. */

    LoadModule::InstallAtFuncPtr(nn::ro::LoadModule);

    nn::fs::SetResultHandledByApplication(true);
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}