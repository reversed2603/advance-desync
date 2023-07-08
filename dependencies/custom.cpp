#include "../nem.hpp"

using namespace ImGui;

bool Custom::Tab(const char* label, bool selected) {

    ImGuiWindow* window = GetCurrentWindow();

    ImGuiID id = window->GetID(label);

    ImRect total_bb(window->DC.CursorPos, { window->DC.CursorPos.x + 265, window->DC.CursorPos.y + 145 });

    ItemSize(total_bb);
    ItemAdd(total_bb, id);

    bool hovered, held;

    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, hovered ? ImColor(255, 48, 25) : ImColor(181, 181, 181), 10);

    PushFont(g_font_big);
    RenderText({ total_bb.Min.x + 265 / 2 - CalcTextSize(label).x / 2, total_bb.Min.y + 145 / 2 - CalcTextSize(label).y / 2 }, label);
    PopFont();

    return pressed;
}

bool Custom::SubTab(const char* label, bool selected) {

    ImGuiWindow* window = GetCurrentWindow();

    ImGuiID id = window->GetID(label);

    PushFont(g_font_medium);
    ImRect total_bb(window->DC.CursorPos, { window->DC.CursorPos.x + CalcTextSize(label).x, window->DC.CursorPos.y + +CalcTextSize(label).y});
    PopFont();

    ItemSize(total_bb);
    ItemAdd(total_bb, id);

    bool hovered, held;

    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    PushFont(g_font_medium);
    PushStyleColor(ImGuiCol_Text, selected ? GetStyleColorVec4(ImGuiCol_Text) : GetStyleColorVec4(ImGuiCol_TextDisabled));
    RenderText(total_bb.Min, label);
    PopStyleColor();
    PopFont();

    return pressed;
}

bool Custom::Toggle(const char* label, bool* v) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, { pos.x + 50 + 9 + label_size.x, pos.y + 25 });
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    window->DrawList->AddRectFilled(total_bb.Min, { total_bb.Min.x + (total_bb.Max.y - total_bb.Min.y)*2, total_bb.Max.y }, ImColor(181, 181, 181), 5);
    window->DrawList->AddRectFilled({ total_bb.Min.x + (*v ? 0 : (total_bb.Max.y - total_bb.Min.y)), total_bb.Min.y }, { total_bb.Min.x + (*v ? (total_bb.Max.y - total_bb.Min.y) : (total_bb.Max.y - total_bb.Min.y)*2), total_bb.Max.y}, * v ? ImColor(255, 48, 25) : ImColor(102, 102, 102), 5);

    RenderText({ total_bb.Max.x - label_size.x, total_bb.Min.y + (total_bb.Max.y - total_bb.Min.y) / 2 - label_size.y / 2 }, label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}
