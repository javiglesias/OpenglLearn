#include "UI.h"

bool UI::PositionWindow(float& x_, float& y_, float& z_)
{
	if (ImGui::Begin("Position"))
	{
		ImGui::Text("Set the position and rotation to spawn:");
		ImGui::Separator();
		float position[3]{ 0.f };
		ImGui::InputFloat3("position", position);
		float rotate_axis[3]{ 0.f };
		ImGui::InputFloat3("Rotation axis", rotate_axis);
		float rotation;
		ImGui::InputFloat("Degrees", &rotation);
		if (ImGui::Button("OK"))
		{
			x_ = position[0];

			return true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			return false;
		}
		ImGui::End();
	}
}
