#pragma once
#include <core/globals.h>
#include <menu/imgui/impl/imgui_impl_dx9.h>
#include <menu/imgui/impl/imgui_impl_win32.h>
#include <menu/imwrap.h>
#include "PathFinder/PathFinder.h"
#include<menu/etc_elements.h>


//TODO: setup config

//forward declare
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace menu {
    void TouchdrawBox();
    void misc();
    void cheats_tab();
    void Automation();
    void itemdata();
    void autofarmmenu();
    void visual_tab();
    void realtime_tab();
    void player_tab();
    void networking_tab();
    void animate();
    void EndScene(IDirect3DDevice9* device, bool active);
    bool WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
} // namespace menu