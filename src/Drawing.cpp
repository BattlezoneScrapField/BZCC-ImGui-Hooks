#include "Drawing.h"
#include "Hook.h"
#include "ScriptUtils.h"
#include <DXGI.h>
#include <Windows.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

BOOL Drawing::bInit = FALSE; // Status of the initialization of ImGui.
bool Drawing::bDisplay = true; // Status of the menu display.
bool Drawing::bSetPos = false; // Status to update ImGui window size / position.
ImVec2 Drawing::vWindowPos = { 0, 0 }; // Last ImGui window position.
ImVec2 Drawing::vWindowSize = { 0, 0 }; // Last ImGui window size.

/**
	@brief : Hook function of Present, the function render ImGui menu.
			 (https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-present)
**/
HRESULT Drawing::hkPresent(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flag)
{
	if (!Hook::pSwapChain)
		Hook::pSwapChain = pThis;

	if (!bInit)
		InitImGui(pThis);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDisplay = !bDisplay;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderAdvancedOverlay();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return Hook::oPresent(pThis, SyncInterval, Flag);
}

/**
	@brief : Funation that init ImGui and retrieve D3D11 device and device context.
	@param pSwapChain : Pointer to the SwapCahin from the Present hook.
**/
void Drawing::InitImGui(IDXGISwapChain* pSwapChain)
{
	Hook::HookWindow();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Hook::window);

	pSwapChain->GetDevice(__uuidof(Hook::pDevice), (void**)&Hook::pDevice);
	Hook::pDevice->GetImmediateContext(&Hook::pDeviceContext);

	ImGui_ImplDX11_Init(Hook::pDevice, Hook::pDeviceContext);

	bInit = TRUE;
}

void Drawing::RenderAdvancedOverlay()
{
	if (bDisplay)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMouseInputs;

		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;

		window_pos.x = (work_pos.x + work_size.x - PAD);
		window_pos.y = (work_pos.y + (PAD * 4));
		window_pos_pivot.x = 1.0f;
		window_pos_pivot.y = 0.0f;
		window_flags |= ImGuiWindowFlags_NoMove;

		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

		ImGui::SetNextWindowSize(ImVec2(550, 0));
		ImGui::SetNextWindowBgAlpha(0.35f);
		ImGui::PushFont(NULL, 22);

		if (ImGui::Begin("Leaderboard", &bDisplay, window_flags))
		{
			static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

			ImGui::Text("Team 1");
			if (ImGui::BeginTable("team1", 5, flags))
			{
				ImGui::TableSetupColumn("Player");
				ImGui::TableSetupColumn("Kills");
				ImGui::TableSetupColumn("Deaths");
				ImGui::TableSetupColumn("Score");
				ImGui::TableSetupColumn("K/D Ratio");
				ImGui::TableHeadersRow();

				ImGui::EndTable();
			}

			ImGui::Separator();

			ImGui::Text("Team 2");
			if (ImGui::BeginTable("team2", 5, flags))
			{
				ImGui::TableSetupColumn("Player");
				ImGui::TableSetupColumn("Kills");
				ImGui::TableSetupColumn("Deaths");
				ImGui::TableSetupColumn("Score");
				ImGui::TableSetupColumn("K/D Ratio");
				ImGui::TableHeadersRow();

				ImGui::EndTable();
			}
		}

		ImGui::PopFont();
		ImGui::End();
	}
}