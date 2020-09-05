#include "ImguiLayer.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Platforms/DirectX11/D3D11Video.h"

#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_dx11.h>



namespace rtr
{
	ImguiLayer::ImguiLayer()
		: Layer("Imgui Layer")
	{

	}

	ImguiLayer::~ImguiLayer()
	{

	}

	void ImguiLayer::OnUpdate()
	{
		
	}

	void ImguiLayer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		
	}

	void ImguiLayer::End()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		Application& app = Application::Get();
		const Window& window = app.GetWindow();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(window.GetClientWidth(), window.GetClientHeight());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImguiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		ImGui_ImplWin32_Init(app.GetWindow().GetNativeWindow());

		D3D11Video* video = static_cast<D3D11Video*>(app.GetVideo());
		ImGui_ImplDX11_Init(video->GetDevice(), video->GetDeviceContext());
	}

	void ImguiLayer::OnDetach()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

}