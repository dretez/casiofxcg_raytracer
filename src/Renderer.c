#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include <gint/usb-ff-bulk.h>
#include <stdio.h>

#include "assets/scenes.h"

#define MAX_DEPTH 3

static inline void check_menu_key() {
    key_event_t ev;
    while ((ev = pollevent()).type != KEYEV_NONE) {
        if (ev.key == KEY_MENU) {
            gint_osmenu();
            dupdate();
        }
    }
}
static inline int elapsed_seconds(uint32_t start) {
    return (rtc_ticks() - start) / 128;
}

int renderPass(const Scene* scene, int pass, int previous) {
    int step         = 1 << (pass - 1);
    int previousStep = previous ? 1 << (previous - 1) : 0;
    for (int y = 0; y < DHEIGHT; y += step) {
        for (int x = 0; x < DWIDTH; x += step) {
            if (previous && !(x & (previousStep - 1)) && !(y & (previousStep - 1))) continue;
            Ray   ray   = cameraRay(&scene->camera, x, y);
            Color color = trace(ray, MAX_DEPTH, scene);
            if (step == 1) dpixel(x, y, color2rgb565(color));
            else drect(x, y, x + step - 1, y + step - 1, color2rgb565(color));
        }
        check_menu_key();
    }
    if (usb_is_open()) usb_fxlink_videocapture(false);
    dupdate();
    return 0;
}

int render() {
    Scene  scene  = createDemoScene();
    int    seconds;
    u32    start = rtc_ticks();
    for (int pass = 6, prev = 0; pass > 0; prev = pass--) {
        uint32_t passStart = rtc_ticks();
        if (renderPass(&scene, pass, prev)) goto exit;
        int t = elapsed_seconds(passStart);
        if (usb_is_open()) {
            usb_fxlink_videocapture(false);
            char buf[64];
            snprintf(buf, sizeof(buf), "Pass %d: %d s\n", pass, t);
            usb_fxlink_text(buf, 0);
        }
    }
exit:
    seconds = elapsed_seconds(start);
    char buf[128];
    snprintf(buf, sizeof(buf), "Render: %d s\n", seconds);
    usb_fxlink_text(buf, 0);
    Scene_free(&scene);
    return 0;
}
