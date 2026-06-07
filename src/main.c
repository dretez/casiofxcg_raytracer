#include <gint/display.h>
#include <gint/keyboard.h>

int main(void) {
	dclear(C_WHITE);
	dtext_opt(DWIDTH / 2, DHEIGHT / 2,
			  C_BLACK, C_NONE,
			  DTEXT_CENTER, DTEXT_MIDDLE,
			  "Sample add-in");
	dupdate();

	getkey();
	return 1;
}
