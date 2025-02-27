#include "FovChanger.h"

namespace cheat {
    void InLevelCameraSetFov_Hook(app::Camera* __this, float value);

    FovChanger::FovChanger() {
        f_Enabled = config::getValue("functions:FovChanger", "enabled", false);
        f_Fov = config::getValue("functions:FovChanger", "fov", 45.0f);
        f_Hotkey = Hotkey("functions:FovChanger");

        HookManager::install(app::Camera_set_fieldOfView, InLevelCameraSetFov_Hook);
    }

    FovChanger& FovChanger::getInstance() {
        static FovChanger instance;
        return instance;
    }

    void FovChanger::GUI() {
        ConfigCheckbox("Fov Changer", f_Enabled, "Custom camera field of view.");

        if (f_Enabled.getValue()) {
            ImGui::Indent();
            ConfigSliderFloat("Fov value", f_Fov, 10.0f, 170.0f, "Set a field of view value.");
            f_Hotkey.Draw();
            ImGui::Unindent();
        }
    }

    void FovChanger::Outer() {
        if (f_Hotkey.IsPressed())
            f_Enabled.setValue(!f_Enabled.getValue());
    }

    void FovChanger::Status() {
        if (f_Enabled.getValue())
            ImGui::Text("FOV (%.1f)", f_Fov.getValue());
    }

    std::string FovChanger::getModule() {
        return _("Visuals");
    }

    void InLevelCameraSetFov_Hook(app::Camera* __this, float value) {
        auto& fovChanger = FovChanger::getInstance();

        if (fovChanger.f_Enabled.getValue())
            value = fovChanger.f_Fov.getValue();
        CALL_ORIGIN(InLevelCameraSetFov_Hook, __this, value);
    }
}
