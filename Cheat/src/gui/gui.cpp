#include "../includes.h"
#include "gui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam
);

//windows process
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

bool gui::SetupWindowClass(const wchar_t* windowClassName) noexcept
{
	// populate window class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;
	
	//register class
	if (!RegisterClassEx(&windowClass))
	{
		return false;
	}

	return true;
}

void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const wchar_t* windowName) noexcept
{
	//create temp window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,
		100,
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window)
	{
		return false;
	}
	
	return true;
}

void gui::DestroyWindow() noexcept
{
	if (window)
	{
		DestroyWindow(window);
	}
}

bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle(L"d3d9.dll");
	
	if (!handle)
	{
		return false;
	}
	
	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);
	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));

	if (!create)
	{
		return false;
	}

	d3d9 = create(D3D_SDK_VERSION);
	
	if (!d3d9)
	{
		return false;
	}
	
	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	
	// replace D3D_OK with < 0 if giving issues
	) != D3D_OK)
	{
		return false;
	}

	return true;
}

void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup() noexcept
{
	if (!SetupWindowClass(L"ImGui Class"))
	{
		throw std::runtime_error("Failed to create window class");
	}

	if (!SetupWindow(L"ImGui Window"))
	{
		throw std::runtime_error("Failed to create window");
	}
	
	if (!SetupDirectX())
	{
		throw std::runtime_error("Failed to create DirectX");
	}
	
	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS { };
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;
	
	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(
			window,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(WndProc)
		)
	);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}

void gui::DestroyMenu() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}

void gui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//make another function for this shit later
	//drawing
	ImGui::Begin("ImGui Window", &open);
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	
}

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	// toggle menu
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		gui::open = !gui::open;
	}

	//pass messages to imgui (?)
	if (gui::open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return 1L;
	}

	return CallWindowProc(
		gui::originalWindowProcess,
		hWnd,
		msg,
		wParam,
		lParam
	);
}