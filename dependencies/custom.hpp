#pragma once

namespace Custom {
    bool Tab(const char* label, bool selected);
    bool Tab2(const char* label, bool selected);
    bool SubTab(const char* label, bool selected);
    bool Toggle(const char* label, bool* v);
    void NoLabelCombo(const char* name, int* current_item, const char* items_separated_by_zeros);
}

inline ImFont* g_font_big{};
inline ImFont* g_font_medium{};
