#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include <gint/usb-ff-bulk.h>
#include <stdio.h>

#include "Engine/Lighting.h"
#include "Engine/Tracing.h"
#include "assets/scenes.h"

#define MAX_DEPTH 2

static inline void check_menu_key() {
    key_event_t ev;
    while ((ev = pollevent()).type != KEYEV_NONE) {
        if (ev.key == KEY_MENU) {
            gint_osmenu();
            dupdate();
        }
    }
}

void renderPass(const Scene* scene, int pass, int previous) {
    int step         = 1 << (pass - 1);
    int previousStep = previous ? 1 << (previous - 1) : 0;
    for (int y = 0; y < DHEIGHT; y += step) {
        for (int x = 0; x < DWIDTH; x += step) {
            if (previous && !(x & (previousStep - 1)) && !(y & (previousStep - 1))) continue;
            Ray ray   = cameraRay(&scene->camera, x, y);
            u16 color = color2rgb565(trace(&ray, MAX_DEPTH, scene));
            if (step == 1) dpixel(x, y, color);
            else drect(x, y, x + step - 1, y + step - 1, color);
        }
        check_menu_key();
    }
    if (usb_is_open()) usb_fxlink_videocapture(false);
    dupdate();
}

int render() {
    Scene scene = createDemoScene();
    u32   start = rtc_ticks();
    for (int pass = 4, prev = 0; pass > 0; prev = pass--) {
        uint32_t passStart = rtc_ticks();
        renderPass(&scene, pass, prev);
        float t = ((rtc_ticks() - passStart) / 128.0f);
        if (usb_is_open()) {
            usb_fxlink_videocapture(false);
            char buf[128];
            snprintf(buf, sizeof(buf), "Pass %d: %d ms (~%d s)\n", pass, (int)(t * 1000.0f), (int)t);
            usb_fxlink_text(buf, 0);
            snprintf(buf, sizeof(buf), "Shadow tests: %ld, Shadow hits: %ld\n", shadowTests, shadowHits);
            usb_fxlink_text(buf, 0);
            snprintf(buf, sizeof(buf), "Traced rays: %u\n", raycount);
            usb_fxlink_text(buf, 0);
        }
    }
    float seconds = ((rtc_ticks() - start) / 128.0f);
    char  buf[128];
    snprintf(buf, sizeof(buf), "Render: %d ms (~%d s)\n", (int)(seconds * 1000.0f), (int)seconds);
    if (usb_is_open()) usb_fxlink_text(buf, 0);
    Scene_free(&scene);
    return 0;
}
