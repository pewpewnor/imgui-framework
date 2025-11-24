imgui.begin_window("Lua Control Panel")

imgui.text("Control Panel from Lua!")
imgui.separator()

local showDemo = globals.get_bool("showImguiDemoWindow")
imgui.text("Demo Window Status: " .. tostring(showDemo))

if imgui.button("Toggle Demo Window") then
	globals.set_bool("showImguiDemoWindow", not showDemo)
end

imgui.separator()

local width, height = imgui.get_display_size()
imgui.text(string.format("Display Size: %.0f x %.0f", width, height))

local fps = imgui.get_framerate()
imgui.text(string.format("FPS: %.1f", fps))

imgui.separator()

if imgui.button("Click Me!") then
	print("Button clicked from Lua!")
end

imgui.same_line()

if imgui.button("Another Button") then
	print("Another button clicked!")
end

imgui.end_window()

imgui.begin_window("Another Window")
imgui.text("This is another window created from Lua!")
imgui.end_window()
