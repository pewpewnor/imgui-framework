imgui.begin_window("Lua Control Panel")

local showDemo = globals.get_bool("showImguiDemoWindow")
imgui.text("Demo Window Status: " .. tostring(showDemo))

if imgui.button("Toggle Demo Window") then
	globals.set_bool("showImguiDemoWindow", not showDemo)
end

imgui.end_window()
