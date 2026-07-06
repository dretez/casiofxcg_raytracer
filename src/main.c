#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/usb-ff-bulk.h>
#include <gint/usb.h>

#define ENFORCE_USB false

static void usb_init(void) {
    usb_interface_t const* interfaces[] = { &usb_ff_bulk, NULL };

    int rc = usb_open(interfaces, GINT_CALL_NULL);
    if (rc == 0 && ENFORCE_USB) usb_open_wait();
}

int main(void) {
    cleareventflips();
    clearevents();
    usb_init();
    dclear(C_WHITE);
    dtext_opt(DWIDTH >> 1,
              DHEIGHT >> 1,
              C_BLACK,
              C_NONE,
              DTEXT_CENTER,
              DTEXT_MIDDLE,
              "Press any key to start");
    if (usb_is_open()) usb_fxlink_videocapture(false);
    dupdate();

    while (true) { getkey(); }
    return 1;
}
