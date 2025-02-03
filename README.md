# HvP2
Xbox360 HvP2 source to allow loading of debug builds

HvP2 - This project allows you to play debug builds of games that previously won't launch due to unresolved imports errors mostly because of XBDM.

How does it work? This project patches the Hypervisor function 'HvxResolveImports' to continue instead of erroring a '0xC0000225' HRESULT due to not being able to find a function import address.

We also hook 'XexpCompleteLoad' & 'XexpResolveImageImports' in the xboxkrnl.exe. 'XexpCompleteLoad' is used to apply patches to the modules loading and to print out the HRESULT for any error's while debugging. 'XexpResolveImageImports' is used to check what the loading module is trying to resolve and if it doesn't exist or is xbdm it replaces it with 'HvP2.xex'.
