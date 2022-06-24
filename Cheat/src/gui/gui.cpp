#include "../includes.h"
#include "gui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

// window process
LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

bool gui::SetupWindowClass(const char* windowClassName) noexcept
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

	// register
	if (!RegisterClassEx(&windowClass))
		return false;

	return true;
}

void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	// create temp window
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
		return false;

	return true;
}

void gui::DestroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);
}

bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
		handle,
		"Direct3DCreate9"
	));

	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

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
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) return false;

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

void gui::Setup()
{
	if (!SetupWindowClass("hackClass001"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("Hack Window"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to create device.");

	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
		);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}

void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// retore wnd proc
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}

void gui::SetMenuStartSize() noexcept {
	if (!setup) return;

	ImGui::SetNextWindowSize(ImVec2(400, 400)); // we set the size only once this way
}

// Saves some information about the ImGui menu being drawn. Only call before ImGui::End, otherwise it wont store proper data.
void gui::StoreMenuState() noexcept {
	if (!setup || !ImGui::GetCurrentWindowRead()) return;

	menuSize = ImGui::GetWindowSize();
	menuRelativePos = ImGui::GetWindowPos();
}

// TODO: fix shitty menu bar from shifting the coordinates calculation in y-coordinate plane
// NB! do not call any imgui functions here
bool gui::IsMouseInsideMenu() noexcept {
	if (!setup) return false;

	HWND csgoWindow = GetActiveWindow();
	if (!csgoWindow) return false;

	POINT cursorPos = {0};
	if (!GetCursorPos(&cursorPos)) return false;

	RECT csgoWindowBounds = {0};
	GetWindowRect(csgoWindow, &csgoWindowBounds);
	
	// calculate the coordinate bounds for our menu
	RECT menuBounds = { 0 };
	menuBounds.left = csgoWindowBounds.left + (LONG)menuRelativePos.x;
	menuBounds.right = menuBounds.left + (LONG)menuSize.x;
	menuBounds.top = csgoWindowBounds.top + (LONG)menuRelativePos.y;
	menuBounds.bottom = menuBounds.top + (LONG)menuSize.y;

	// check if cursorPos is between these bounds
	if (menuBounds.left < cursorPos.x && cursorPos.x < menuBounds.right && menuBounds.top < cursorPos.y && cursorPos.y < menuBounds.bottom) return true;
	else return false;
}

void gui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//do drawing here
	//for styling edits, find function in IMGUI file and modify it to style to your liking
	ImGui::Begin("REAPER | CS:GO", &open);

	// im just so retarded having been unable to add absolutely nothing else to the menu than gui::StoreMenuState

	gui::StoreMenuState();
	ImGui::End();
	//do drawing here

	ImGui::EndFrame();		
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	// toogle menu
	if (GetAsyncKeyState(VK_INSERT) & 1)
		gui::open = !gui::open;

	// pass messages to imgui
	if (gui::open && ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		wideParam,
		longParam
	)) return 1L;

	// block keyboard/mouse input from being passed on to cs:go if we are clicking buttons inside our menu
	if (gui::IsMouseInsideMenu()) return 1L;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}