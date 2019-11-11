//***************************************************************************************
// d3dApp.h by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

// ����Ҽ���ס��ת���
// ������������ͼ
// ����м���ס��ק��ͼ
// �ո����λ

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "DirectXMath.h"
#include "../d3dUtil/d3dUtil.h"
#include "../GameTimer/GameTimer.h"
#include "../MathHelper/MathHelper.h"
#include "../Mouse/Mouse.h"
#include "../Keyboard/Keyboard.h" 

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace DirectX;
using namespace DirectX::PackedVector;

class D3DApp
{
protected:

    D3DApp(HINSTANCE hInstance);
    D3DApp(const D3DApp& rhs) = delete;
    D3DApp& operator=(const D3DApp& rhs) = delete;
    virtual ~D3DApp();

public:

    static D3DApp* GetApp();
    
	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	float     AspectRatio()const;

    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);

	int Run();
 
    virtual bool Initialize();
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    virtual void CreateRtvAndDsvDescriptorHeaps();
	virtual void OnResize(); 
	virtual void Update(const GameTimer& gt) = 0;
	virtual void UpdateMouse() = 0; // �Զ���������
	virtual void UpdateKeyboard() = 0; // �Զ�����̼���
    virtual void Draw(const GameTimer& gt) = 0;

protected:

	bool InitMainWindow();
	bool InitDirect3D();
	void InitMouseDefault(); // ��Ĭ�ϲ�����ʼ�����
	void UpdateMouseDefault(); // ��Ĭ�Ϸ�ʽ�������
	void UpdateKeyboardDefault(); // ��Ĭ�Ϸ�ʽ���¼���
	void ResetCameraState(); // �������״̬
	void CreateCommandObjects();
    void CreateSwapChain();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	void CalculateFrameStats();

    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:

    static D3DApp* mApp;

    HINSTANCE mhAppInst = nullptr; // application instance handle
    HWND      mhMainWnd = nullptr; // main window handle
	bool      mAppPaused = false;  // is the application paused?
	bool      mMinimized = false;  // is the application minimized?
	bool      mMaximized = false;  // is the application maximized?
	bool      mResizing = false;   // are the resize bars being dragged?
    bool      mFullscreenState = false;// fullscreen enabled

	// Set true to use 4X MSAA (?.1.8).  The default is false.
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	// Used to keep track of the delta-time and game time (4.4).
	GameTimer mTimer;
	
    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
    UINT64 mCurrentFence = 0;
	
	// ���CommandList��Queue
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

    D3D12_VIEWPORT mScreenViewport; 
    D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption = L"d3d App";
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 800;
	int mClientHeight = 600;

	// ��������
	std::unique_ptr<DirectX::Mouse> mMouse;
	DirectX::Mouse::ButtonStateTracker mMouseTracker;
	std::unique_ptr<DirectX::Keyboard> mKeyboard;
	DirectX::Keyboard::KeyboardStateTracker mKeyboardTracker;
	float mMouseSpeed = 5.0f;

	// ���������ռ��µ�����
	// ������Ǩ�Ƶ����࣬��������ӽ�
	// ������ϵ���������ڱ��۲��
	float mTheta = DirectX::XM_PIDIV4;
	float mPhi = DirectX::XM_PIDIV4;
	float mRadius = 5.0f;
	// ֱ������ϵ����
	XMVECTOR mCameraPos = MathHelper::SphericalToCartesian(mRadius, mTheta, mPhi);
	// ���۲�㣬Ĭ��Ϊ����ռ�ԭ��
	XMVECTOR mTargetPos = XMVectorZero();
	// ����ռ��б�ʾ���ϵ�����
	const XMVECTOR mUpDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// ��׶��
	float mNearZ = 1.0f;
	float mFarZ = 1000.0f;
};
