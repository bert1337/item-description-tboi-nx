#include "lib.hpp"
#include "nn/ro.h"

bool initialized = false;
bool gamestart = false;
void* inputDeviceGamePad;

struct Color {
    float r;
    float g;
    float b;
    float a;
};

enum Buttons {
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3,
    A = 4,
    B = 5,
    X = 6,
    Y = 7,
    L = 8,
    ZL = 9,
    LEFT_STICK = 10,
    R = 11,
    ZR = 12,
    RIGHT_STICK = 13,
};

extern "C" {
    // Draw string ??
    void _ZNK4KAGE8Graphics4Font16DrawStringScaledEPKtffffNS0_5ColorEjb(void* that, char* msg,float x,float y, Color color, uint alling, bool center );
    bool _ZN4KAGE5Input15InputDeviceBase15IsButtonPressedEj(void* t,Buttons button);
    // void * _ZN15IsaacRepentance9g_ManagerE;
    void* _ZNK15IsaacRepentance10ItemConfig14GetCollectibleEi(void* t, uint itemID);
    void _ZNK4KAGE8Graphics4Font16DrawStringScaledEPKcffffNS0_5ColorEjb(float param_1,float param_2,float param_3,float param_4,long param_5,char const* text, float* param_7,uint param_8,uint param_9);
    void _ZN4KAGE8Graphics5ColorC1Ej(float *param_1,uint param_2);
    long long _ZN15IsaacRepentance9g_ManagerE;
    void _ZN15IsaacRepentance4Room19RenderDebugGridInfoERKN4KAGE4Math7Vector2E(long param3, long var14)
	//  Player Constructor
    // void _ZN15IsaacRepentance13Entity_PlayerC1Ev();
}

void renderText (const char* text) {
    float auStack_f8[16];
    _ZN4KAGE8Graphics5ColorC1Ej(auStack_f8,0xffffffff);
    svcOutputDebugString(text, 15);
    // long lVar36 = _ZN15IsaacRepentance9g_ManagerE + 0x76f90;
    _ZNK4KAGE8Graphics4Font16DrawStringScaledEPKcffffNS0_5ColorEjb(80,80,0x3f800000,0x3f800000,435664904080,text,auStack_f8,0,0);
}

HOOK_DEFINE_TRAMPOLINE(MainHook){
    /* Define the callback for when the function is called. Don't forget to make it static and name it Callback. */
    static void Callback(){
        /* Call the original function, with the argument always being false. */
        // INPUT NOT WORKING WTF
        Orig();
        if(initialized) {
            if(_ZN4KAGE5Input15InputDeviceBase15IsButtonPressedEj(inputDeviceGamePad, Buttons::A)){
                svcOutputDebugString("AAAAAAAAAAA", 15);
            }
            if(_ZN4KAGE5Input15InputDeviceBase15IsButtonPressedEj(inputDeviceGamePad, Buttons::B)){
                svcOutputDebugString("BBBBBBBBBBB", 15);
            }
        }
            // _ZNK4KAGE8Graphics4Font16DrawStringScaledEPKcffffNS0_5ColorEjb(80.0,80.0,1.0,1.0,435664904080,"MIAO",317503605544,0,0);
        std::string hackedText = "DIOCANE";
        renderText(hackedText.c_str());
        return;
    }
};



HOOK_DEFINE_TRAMPOLINE(GameStart){
    /* Define the callback for when the function is called. Don't forget to make it static and name it Callback. */
    static void Callback(void* t, long player_type, long challenge_type, long seeds,int difficulty){
        /* Call the original function, with the argument always being false. */
        std::string INFO = "\nPlayerType: " + std::to_string(player_type) +
                           "\nChallange_type: " +
                           std::to_string(challenge_type) +
                           "\nSeeds: " + std::to_string(seeds) +
                           "\nDifficulty: " + std::to_string(difficulty);
        svcOutputDebugString(INFO.c_str(), 100);
        gamestart = true;

        return Orig(t, player_type, challenge_type, seeds, difficulty);
    }
};

HOOK_DEFINE_TRAMPOLINE(ItemHook){
    /* Define the callback for when the function is called. Don't forget to make it static and name it Callback. */
    static void Callback(void* t, uint itemID){
        /* Call the original function, with the argument always being false. */
        // if(initialized) {
        //     std::string INFO = "\n::::::::ITEM::::: " + std::to_string(itemID);
        //     svcOutputDebugString(INFO.c_str(), 100);
        // }
        return Orig(t, itemID);
    }
};

HOOK_DEFINE_TRAMPOLINE(InitGamePad){
    /* Define the callback for when the function is called. Don't forget to make it static and name it Callback. */
    static void Callback(void* t,Buttons button){
        /* Call the original function, with the argument always being false. */
        if(!initialized) {
            // svcOutputDebugString(std::to_string(button).c_str(), 5);
            inputDeviceGamePad = t;
            initialized = true;
        }
        return Orig(t, button);
    }
};

HOOK_DEFINE_TRAMPOLINE(DrawString){
    /* Define the callback for when the function is called. Don't forget to make it static and name it Callback. */
    static void Callback (float x,float y,float param_3,float param_4,long param_5,char const* text, long param_7,uint param_8,uint param_9){
        std::string hackedText = "MIAO";
        std::string INFO = "\n1: " + std::to_string(x) +
                           "\n2: " + std::to_string(y) +
                           "\n3: " + std::to_string(param_3) +
                           "\n4: " + std::to_string(param_4) +
                           "\n5: " + std::to_string(param_5) + //435664904080
                           "\n7: " + std::to_string(param_7) + //317503605576
                           "\n8: " + std::to_string(param_8) +
                           "\n9: " + std::to_string(param_9);
        svcOutputDebugString(INFO.c_str(), 200);
        return Orig(80,80,param_3,param_4,param_5,hackedText.c_str(),param_7,param_8,param_9);
    }
};



HOOK_DEFINE_TRAMPOLINE(LoadModule) {

        static Result Callback(nn::ro::Module* pOutModule, const void* pImage, void* buffer, size_t bufferSize, int flag) {

            // Hooking all the functions here, as most of them are loaded from external we need to wait 
            // for the library to be loaded, then inject.

            Result ret = Orig(pOutModule, pImage, buffer, bufferSize, 1);
            uintptr_t pointer = 0;
            // Hook run_repentance function (Just to test if the hooking works) 
            nn::ro::LookupModuleSymbol(&pointer, pOutModule, "_ZN15IsaacRepentance11IsaacUpdateEv");
            MainHook::InstallAtPtr(pointer);

            nn::ro::LookupModuleSymbol(&pointer, pOutModule, "_ZN15IsaacRepentance4Game5StartENS_11ePlayerTypeENS_10eChallengeENS_5SeedsENS0_11eDifficultyE");
            GameStart::InstallAtPtr(pointer);

            nn::ro::LookupModuleSymbol(&pointer, pOutModule, "_ZN4KAGE5Input15InputDeviceBase15IsButtonPressedEj");
            InitGamePad::InstallAtPtr(pointer);

            nn::ro::LookupModuleSymbol(&pointer, pOutModule, "_ZNK15IsaacRepentance10ItemConfig14GetCollectibleEi");
            ItemHook::InstallAtPtr(pointer);

            nn::ro::LookupModuleSymbol(&pointer, pOutModule, "_ZNK4KAGE8Graphics4Font16DrawStringScaledEPKcffffNS0_5ColorEjb");
            DrawString::InstallAtPtr(pointer);

            return ret;
    }
};


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