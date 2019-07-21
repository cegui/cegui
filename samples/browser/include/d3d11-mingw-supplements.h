/***********************************************************************
    created:    1/7/2016
    author:     Yaron Cohen-Tal

    purpose:    Defines Direct3D stuff missing from some MinGW-w64 versions.
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2016 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/

#ifndef _d3d11_mingw_supplements_h_
#define _d3d11_mingw_supplements_h_

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <windef.h>

#ifdef __MINGW64_VERSION_MAJOR

extern "C"
{

HRESULT WINAPI D3D11CreateDeviceAndSwapChain(
    IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
    IDXGISwapChain** ppSwapChain,
    ID3D11Device** ppDevice,
    D3D_FEATURE_LEVEL* pFeatureLevel,
    ID3D11DeviceContext** ppImmediateContext);

#if __MINGW64_VERSION_MAJOR < 5

#ifndef __ID3D11Debug_FWD_DEFINED__
#define __ID3D11Debug_FWD_DEFINED__
typedef interface ID3D11Debug ID3D11Debug;
#endif

typedef enum D3D11_RLDO_FLAGS {
    D3D11_RLDO_SUMMARY = 1,
    D3D11_RLDO_DETAIL = 2
} D3D11_RLDO_FLAGS;

/*****************************************************************************
 * ID3D11Debug interface
 */
#ifndef __ID3D11Debug_INTERFACE_DEFINED__
#define __ID3D11Debug_INTERFACE_DEFINED__

DEFINE_GUID(IID_ID3D11Debug, 0x79cf2233, 0x7536, 0x4948, 0x9d,0x36, 0x1e,0x46,0x92,0xdc,0x57,0x60);
#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("79cf2233-7536-4948-9d36-1e4692dc5760")
ID3D11Debug : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE SetFeatureMask(
        UINT Mask) = 0;

    virtual UINT STDMETHODCALLTYPE GetFeatureMask(
        ) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetPresentPerRenderOpDelay(
        UINT Milliseconds) = 0;

    virtual UINT STDMETHODCALLTYPE GetPresentPerRenderOpDelay(
        ) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetSwapChain(
        IDXGISwapChain *pSwapChain) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetSwapChain(
        IDXGISwapChain **ppSwapChain) = 0;

    virtual HRESULT STDMETHODCALLTYPE ValidateContext(
        ID3D11DeviceContext *pContext) = 0;

    virtual HRESULT STDMETHODCALLTYPE ReportLiveDeviceObjects(
        D3D11_RLDO_FLAGS Flags) = 0;

    virtual HRESULT STDMETHODCALLTYPE ValidateContextForDispatch(
        ID3D11DeviceContext *pContext) = 0;

};
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(ID3D11Debug, 0x79cf2233, 0x7536, 0x4948, 0x9d,0x36, 0x1e,0x46,0x92,0xdc,0x57,0x60)
#endif
#else
typedef struct ID3D11DebugVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        ID3D11Debug* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        ID3D11Debug* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        ID3D11Debug* This);

    /*** ID3D11Debug methods ***/
    HRESULT (STDMETHODCALLTYPE *SetFeatureMask)(
        ID3D11Debug* This,
        UINT Mask);

    UINT (STDMETHODCALLTYPE *GetFeatureMask)(
        ID3D11Debug* This);

    HRESULT (STDMETHODCALLTYPE *SetPresentPerRenderOpDelay)(
        ID3D11Debug* This,
        UINT Milliseconds);

    UINT (STDMETHODCALLTYPE *GetPresentPerRenderOpDelay)(
        ID3D11Debug* This);

    HRESULT (STDMETHODCALLTYPE *SetSwapChain)(
        ID3D11Debug* This,
        IDXGISwapChain *pSwapChain);

    HRESULT (STDMETHODCALLTYPE *GetSwapChain)(
        ID3D11Debug* This,
        IDXGISwapChain **ppSwapChain);

    HRESULT (STDMETHODCALLTYPE *ValidateContext)(
        ID3D11Debug* This,
        ID3D11DeviceContext *pContext);

    HRESULT (STDMETHODCALLTYPE *ReportLiveDeviceObjects)(
        ID3D11Debug* This,
        D3D11_RLDO_FLAGS Flags);

    HRESULT (STDMETHODCALLTYPE *ValidateContextForDispatch)(
        ID3D11Debug* This,
        ID3D11DeviceContext *pContext);

    END_INTERFACE
} ID3D11DebugVtbl;
interface ID3D11Debug {
    CONST_VTBL ID3D11DebugVtbl* lpVtbl;
};

#ifdef COBJMACROS
#ifndef WIDL_C_INLINE_WRAPPERS
/*** IUnknown methods ***/
#define ID3D11Debug_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define ID3D11Debug_AddRef(This) (This)->lpVtbl->AddRef(This)
#define ID3D11Debug_Release(This) (This)->lpVtbl->Release(This)
/*** ID3D11Debug methods ***/
#define ID3D11Debug_SetFeatureMask(This,Mask) (This)->lpVtbl->SetFeatureMask(This,Mask)
#define ID3D11Debug_GetFeatureMask(This) (This)->lpVtbl->GetFeatureMask(This)
#define ID3D11Debug_SetPresentPerRenderOpDelay(This,Milliseconds) (This)->lpVtbl->SetPresentPerRenderOpDelay(This,Milliseconds)
#define ID3D11Debug_GetPresentPerRenderOpDelay(This) (This)->lpVtbl->GetPresentPerRenderOpDelay(This)
#define ID3D11Debug_SetSwapChain(This,pSwapChain) (This)->lpVtbl->SetSwapChain(This,pSwapChain)
#define ID3D11Debug_GetSwapChain(This,ppSwapChain) (This)->lpVtbl->GetSwapChain(This,ppSwapChain)
#define ID3D11Debug_ValidateContext(This,pContext) (This)->lpVtbl->ValidateContext(This,pContext)
#define ID3D11Debug_ReportLiveDeviceObjects(This,Flags) (This)->lpVtbl->ReportLiveDeviceObjects(This,Flags)
#define ID3D11Debug_ValidateContextForDispatch(This,pContext) (This)->lpVtbl->ValidateContextForDispatch(This,pContext)
#else
/*** IUnknown methods ***/
static FORCEINLINE HRESULT ID3D11Debug_QueryInterface(ID3D11Debug* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}
static FORCEINLINE ULONG ID3D11Debug_AddRef(ID3D11Debug* This) {
    return This->lpVtbl->AddRef(This);
}
static FORCEINLINE ULONG ID3D11Debug_Release(ID3D11Debug* This) {
    return This->lpVtbl->Release(This);
}
/*** ID3D11Debug methods ***/
static FORCEINLINE HRESULT ID3D11Debug_SetFeatureMask(ID3D11Debug* This,UINT Mask) {
    return This->lpVtbl->SetFeatureMask(This,Mask);
}
static FORCEINLINE UINT ID3D11Debug_GetFeatureMask(ID3D11Debug* This) {
    return This->lpVtbl->GetFeatureMask(This);
}
static FORCEINLINE HRESULT ID3D11Debug_SetPresentPerRenderOpDelay(ID3D11Debug* This,UINT Milliseconds) {
    return This->lpVtbl->SetPresentPerRenderOpDelay(This,Milliseconds);
}
static FORCEINLINE UINT ID3D11Debug_GetPresentPerRenderOpDelay(ID3D11Debug* This) {
    return This->lpVtbl->GetPresentPerRenderOpDelay(This);
}
static FORCEINLINE HRESULT ID3D11Debug_SetSwapChain(ID3D11Debug* This,IDXGISwapChain *pSwapChain) {
    return This->lpVtbl->SetSwapChain(This,pSwapChain);
}
static FORCEINLINE HRESULT ID3D11Debug_GetSwapChain(ID3D11Debug* This,IDXGISwapChain **ppSwapChain) {
    return This->lpVtbl->GetSwapChain(This,ppSwapChain);
}
static FORCEINLINE HRESULT ID3D11Debug_ValidateContext(ID3D11Debug* This,ID3D11DeviceContext *pContext) {
    return This->lpVtbl->ValidateContext(This,pContext);
}
static FORCEINLINE HRESULT ID3D11Debug_ReportLiveDeviceObjects(ID3D11Debug* This,D3D11_RLDO_FLAGS Flags) {
    return This->lpVtbl->ReportLiveDeviceObjects(This,Flags);
}
static FORCEINLINE HRESULT ID3D11Debug_ValidateContextForDispatch(ID3D11Debug* This,ID3D11DeviceContext *pContext) {
    return This->lpVtbl->ValidateContextForDispatch(This,pContext);
}
#endif
#endif

#endif

HRESULT STDMETHODCALLTYPE ID3D11Debug_SetFeatureMask_Proxy(
    ID3D11Debug* This,
    UINT Mask);
void __RPC_STUB ID3D11Debug_SetFeatureMask_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
UINT STDMETHODCALLTYPE ID3D11Debug_GetFeatureMask_Proxy(
    ID3D11Debug* This);
void __RPC_STUB ID3D11Debug_GetFeatureMask_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ID3D11Debug_SetPresentPerRenderOpDelay_Proxy(
    ID3D11Debug* This,
    UINT Milliseconds);
void __RPC_STUB ID3D11Debug_SetPresentPerRenderOpDelay_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
UINT STDMETHODCALLTYPE ID3D11Debug_GetPresentPerRenderOpDelay_Proxy(
    ID3D11Debug* This);
void __RPC_STUB ID3D11Debug_GetPresentPerRenderOpDelay_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ID3D11Debug_SetSwapChain_Proxy(
    ID3D11Debug* This,
    IDXGISwapChain *pSwapChain);
void __RPC_STUB ID3D11Debug_SetSwapChain_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ID3D11Debug_GetSwapChain_Proxy(
    ID3D11Debug* This,
    IDXGISwapChain **ppSwapChain);
void __RPC_STUB ID3D11Debug_GetSwapChain_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ID3D11Debug_ValidateContext_Proxy(
    ID3D11Debug* This,
    ID3D11DeviceContext *pContext);
void __RPC_STUB ID3D11Debug_ValidateContext_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ID3D11Debug_ReportLiveDeviceObjects_Proxy(
    ID3D11Debug* This,
    D3D11_RLDO_FLAGS Flags);
void __RPC_STUB ID3D11Debug_ReportLiveDeviceObjects_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE ID3D11Debug_ValidateContextForDispatch_Proxy(
    ID3D11Debug* This,
    ID3D11DeviceContext *pContext);
void __RPC_STUB ID3D11Debug_ValidateContextForDispatch_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __ID3D11Debug_INTERFACE_DEFINED__ */

#endif

}

#endif

#endif
