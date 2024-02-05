#if !defined(GG_STYLING_H)
#define GG_STYLING_H

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

// Angus Goucher Nuklear styling
void ApplyStyle(struct nk_context* ctx) {
    // Green text on buttons
    ctx->style.button.text_normal = (struct nk_color){62, 230, 107, 255};
    ctx->style.button.text_active = (struct nk_color){42, 210, 87, 255};
    ctx->style.button.text_hover = (struct nk_color){92, 255, 127, 255};
}

#endif // GG_STYLING_H

