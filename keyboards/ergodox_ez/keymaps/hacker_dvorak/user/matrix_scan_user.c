#include "../master/leader_setup.c"

bool MATRIX_SCANNED = false;

void matrix_scan_user(void) {
    if (!(MATRIX_SCANNED)) {
        rgblight_sethsv_noeeprom_green();
        MATRIX_SCANNED = true;
    }

    #include "../master/leader_dictionary.c"
};
