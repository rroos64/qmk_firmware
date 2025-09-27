/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "skeletyl.h"

#define _BASE 0
#define _NUM 1
#define _SPEC 2
#define _NAV 3
#define _DH 4

// Tap Dance declarations
enum {
    TD_ENT_SHFTENT,
    TD_BSP_CTRLBS,
    TD_SHFT_UP,
    TD_SHFT_DOWN,
    TD_SHFT_R,
    TD_SHFT_L,
    TD_SHFT_HOME,
    TD_SHFT_END
};

// --- Custom keycode to toggle default layer persistently (QW <-> DH)
enum custom_keycodes {
  PDF_QW = SAFE_RANGE,  // set default to _BASE
  PDF_DH,               // set default to _DH
  PDF_TOG,              // toggle default between _BASE and _DH
};

// --- Latch/unlatch logic for the two left-thumb LT() keys ---
static uint8_t lt_left_layer = 0;
static bool    lt_left_held  = false;
static bool    lt_latched    = false;

#define LT_LEFT_OUTER LT(_NAV, KC_ESC)
#define LT_LEFT_INNER LT(_SPEC, KC_TAB)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case PDF_QW:
                set_single_persistent_default_layer(_BASE);
                return false;
            case PDF_DH:
                set_single_persistent_default_layer(_DH);
                return false;
            case PDF_TOG:
                if (layer_state_cmp(default_layer_state, _DH)) {
                    set_single_persistent_default_layer(_BASE);
                } else {
                    set_single_persistent_default_layer(_DH);
                }
                return false;
        }
    }

    // Track when a left-thumb LT() is held
    if (keycode == LT_LEFT_OUTER || keycode == LT_LEFT_INNER) {
        if (record->event.pressed) {
            lt_left_held  = true;
            lt_left_layer = (keycode == LT_LEFT_OUTER) ? _NAV : _SPEC;
        } else {
            lt_left_held  = false;
            // if latched, we keep the layer on; if not latched, LT() releases it for us
        }
        return true; // don't block LT's normal tap/hold behavior (Esc/Tab taps remain)
    }

    // C toggles latch if a left-thumb layer is being (or was) held
    if (keycode == KC_C && record->event.pressed) {
        if (lt_left_held && !lt_latched) {
            layer_on(lt_left_layer);
            lt_latched = true;
            return false; // swallow 'c' when latching (delete if you want it to type 'c')
        } else if (lt_latched) {
            layer_off(lt_left_layer);
            lt_latched = false;
            return false; // swallow 'c' on unlatch
        }
    }

    return true;
};

// Identify your home-row MTs (adjust to match your keymap)
static inline bool is_hrm(uint16_t kc) {
    switch (kc) {
        case LCTL_T(KC_S):
        case LGUI_T(KC_D):
        case LALT_T(KC_F):
        case LSFT_T(KC_G):
        case RSFT_T(KC_H):
        case RALT_T(KC_J):
        case RGUI_T(KC_K):
        case RCTL_T(KC_L):
            return true;
    }
    return false;
}

// --- Combos
enum combo_events {
  CB_PDF_TOG,   // both-thumbs persistent flip
  COMBO_LENGTH
};

// replace these with your actual **outer-left** and **outer-right** thumb keys:
const uint16_t PROGMEM pdf_tog_combo[] = { LT(_NAV, KC_ESC), KC_ENT, COMBO_END };
// Left outer thumb is an LT() key (Esc/Tab on tap, SPEC/NAV on hold).
// Right outer thumb is Backspace on base, per your layout.
combo_t key_combos[COMBO_LENGTH] = {
  [CB_PDF_TOG] = COMBO(pdf_tog_combo, PDF_TOG),
};

// --- Make this combo *hold-only* and give it a slightly longer window
// Let QMK know how many combos exist
uint16_t COMBO_LEN = sizeof(key_combos) / sizeof(key_combos[0]);

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
  switch (index) {
    case CB_PDF_TOG:
      // A bit above TAPPING_TERM avoids accidental taps; tweak to taste
      return TAPPING_TERM + 75;
  }
  return COMBO_TERM;
}

bool get_combo_must_hold(uint16_t index, combo_t *combo) {
  // Require holds (prevents taps from ever firing the combo)
  return (index == CB_PDF_TOG);
}

void dance_ons_shft_caps(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(OSM(MOD_LSFT)); }
    else { tap_code(KC_CAPS); }
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ENT_SHFTENT] = ACTION_TAP_DANCE_DOUBLE(KC_ENT, LSFT(KC_ENT)),
    [TD_BSP_CTRLBS] = ACTION_TAP_DANCE_DOUBLE(KC_BSPACE, LCTL(KC_BSPACE)),
    [TD_SHFT_UP] = ACTION_TAP_DANCE_DOUBLE(KC_UP, LSFT(KC_UP)),
    [TD_SHFT_DOWN] = ACTION_TAP_DANCE_DOUBLE(KC_DOWN, LSFT(KC_DOWN)),
    [TD_SHFT_R] = ACTION_TAP_DANCE_DOUBLE(KC_RIGHT, LSFT(KC_RIGHT)),
    [TD_SHFT_L] = ACTION_TAP_DANCE_DOUBLE(KC_LEFT, LSFT(KC_LEFT)),
    [TD_SHFT_HOME] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, LSFT(KC_HOME)),
    [TD_SHFT_END] = ACTION_TAP_DANCE_DOUBLE(KC_END, LSFT(KC_END))
};

// 1) Slightly longer tap window for HRMs so quick taps register reliably
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    if (is_hrm(keycode)) return 220;   // try 210–240ms
    switch (keycode) {
        case LT_LEFT_OUTER:
        case LT_LEFT_INNER:
            return 170; // a bit shorter than global TAPPING_TERM
    };
    return TAPPING_TERM;
}

// 2) Don’t let “pressing another key” convert an HRM tap into a hold
bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    return is_hrm(keycode);            // favor TAP on rolls
}

// 3) Don’t force HRMs to become holds just because another key was pressed
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    return false;                      // or: return !is_hrm(keycode);
}

// Light LEDs 4 and 8 red when caps lock is active. Hard to ignore!
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {3, 1, HSV_MAGENTA},       // Light 1 LEDs, starting with LED 4
    {7, 1, HSV_MAGENTA}       // Light 1 LEDs, starting with LED 8
);
// Switch all LED's off when default layer is active
const rgblight_segment_t PROGMEM my_base_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_BLACK}
);
// Light LEDs 11 & 12 in purple when keyboard layer 2 is active
const rgblight_segment_t PROGMEM my_num_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_RED}
);
// Light LEDs 13 & 14 in green when keyboard layer 3 is active
const rgblight_segment_t PROGMEM my_spec_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_GREEN}
);
// Light LEDs 13 & 14 in green when keyboard layer 3 is active
const rgblight_segment_t PROGMEM my_nav_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_BLUE}
);

const rgblight_segment_t PROGMEM my_dh_hint_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    { 0, 2, HSV_TURQUOISE }  // small, subtle hint (LEDs 0–1)
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_base_layer,
    my_capslock_layer,  // Overrides base layer
    my_num_layer,    // Overrides other layers
    my_spec_layer,     // Overrides other layers
    my_nav_layer,
    my_dh_hint_layer
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
};

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(1, led_state.caps_lock);
    return true;
};

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _BASE));
    // DH hint lights only when _DH is default (persisted)
    rgblight_set_layer_state(5, layer_state_cmp(state, _DH));
    return state;
};

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(2, layer_state_cmp(state, _NUM));
    rgblight_set_layer_state(3, layer_state_cmp(state, _SPEC));
    rgblight_set_layer_state(4, layer_state_cmp(state, _NAV));
    return state;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT_split_3x5_3(

/* Layer 0: Base keys
 *
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |    Q    |    W    |    E    |    R    |    T    |                      |    Y    |    U    |    I    |    O    |    P    |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |    A    |  CTRL/S |  GUI/D  |  ALT/F  |    G    |                      |    H    |  ALT/J  |  GUI/K  | CTRL/L  |   DEL   |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |  SHF/Z  |    X    |    C    |    V    |    B    |                      |    N    |    M    |   < ,   |   > .   | SHF/ /? |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 *                                .--------.--------.--------.      .--------.--------.--------.
 *                                |  _NUM |_SPEC/TAB|_NAV/ESC|      |   ENT  |  SPC   |   BSP  |
 *                                .--------.--------.--------.      .--------.--------.--------.
 */

  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        KC_Q,                   KC_W,              KC_E,               KC_R,            KC_T,               LT(_NAV, KC_ESC),                             KC_ENT,                 KC_Y,             KC_U,              KC_I,              KC_O,                  KC_P,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        KC_A,               LCTL_T(KC_S),       LGUI_T(KC_D),       LALT_T(KC_F),       KC_G,               LT(_SPEC, KC_TAB),                            KC_SPC,                 KC_H,           RALT_T(KC_J),      RGUI_T(KC_K),       RCTL_T(KC_L),           KC_DEL,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        LSFT_T(KC_Z),           KC_X,              KC_C,               KC_V,            KC_B,                   MO(_NUM),                                 KC_BSPC,                KC_N,             KC_M,              KC_COMM,           KC_DOT,           RSFT_T(KC_SLSH)
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

/* Layer 1: Numbers and Function keys
 *
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |    F1   |    F2   |    F3   |    F4   |    F5   |                      |    *    |    8    |    8    |    O    |    -    |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |    F6   |    F7   |    F8   |    F9   |    F10  |                      |    /    |    5    |    5    |    6    |    +    |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |    F11  |    F12  |  _NUM   |    xxx  |    xxx  |                      |    0    |    2    |    2    |    3    |    .    |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 *                                .--------.--------.--------.      .--------.--------.--------.
 *                                |  _NUM |_SPEC/TAB|_NAV/ESC|      |   ENT  |  SPC   |   BSP  |
 *                                .--------.--------.--------.      .--------.--------.--------.
 */

  [_NUM] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F1,               KC_F2,             KC_F3,             KC_F4,             KC_F5,                _______,                                  _______,            KC_KP_ASTERISK,         KC_7,              KC_8,             KC_9,              KC_KP_MINUS,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F6,               KC_F7,             KC_F8,             KC_F9,            KC_F10,                _______,                                  _______,            KC_KP_SLASH,            KC_4,              KC_5,             KC_6,              KC_KP_PLUS,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F11,              KC_F12,          TG(_NUM),            XXXXXXX,          XXXXXXX,               XXXXXXX,                                  _______,                KC_0,               KC_1,              KC_2,             KC_3,              KC_DOT
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

/* Layer 2: Special characters and symbols
 *
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |    !    |    @    |    #    |    $    |    %    |                      |    &    |    &    |    *    |    (    |    )    |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |   ~ `   |   xxx   |   INS   |   xxx   |   xxx   |                      |   { [   |   } ]   |   | \   |   : ;   |   " '   |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |   SHF   |   xxx   |  _SPEC  |   xxx   |   xxx   |                      |   _ -   |   + =    |   < ,  |   > .   | SHF/ /? |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 *                                .--------.--------.--------.      .--------.--------.--------.
 *                                |  _NUM |_SPEC/TAB|_NAV/ESC|      |   ENT  |  SPC   |   BSP  |
 *                                .--------.--------.--------.      .--------.--------.--------.
 */

 [_SPEC] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_EXLM,           KC_AT,              KC_HASH,           KC_DLR,              KC_PERC,              _______,                                 _______,              KC_CIRC,           KC_AMPR,             KC_ASTR,           KC_LPRN,            KC_RPRN,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_GRAVE,          XXXXXXX,             KC_INS,           XXXXXXX,             XXXXXXX,              XXXXXXX,                                  _______,              KC_LBRC,           KC_RBRC,             KC_BSLS,           KC_SCLN,            KC_QUOT,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            _______,           KC_CAPS,           TG(_SPEC),          XXXXXXX,             XXXXXXX,              _______,                                  _______,              KC_MINS,           KC_EQL,              _______,           _______,            _______
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

/* Layer 3: Navigation keys
 *
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |   xxx   |   xxx   |   xxx   |   xxx   |   xxx   |                      |   Home  |  PgUp   |Ctrl+Left|Ctrl+Right|  xxx   |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |   xxx   |   xxx   |   xxx   |   xxx   |   xxx   |                      |  Left   |  Down   |    Up   |  Right  | Ctrl+W  |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |   SHF   |   xxx   |   _NAV  |   xxx   |   xxx   |                      |   xxx   |  PgDn   | Ctrl+^  | Ctrl+]  |   End   |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 *                                .--------.--------.--------.      .--------.--------.--------.
 *                                |  _NUM |_SPEC/TAB|_NAV/ESC|      |   ENT  |  SPC   |   BSP  |
 *                                .--------.--------.--------.      .--------.--------.--------.
 */

 [_NAV] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
         XXXXXXX,             XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,              XXXXXXX,                                  _______,              KC_HOME,           KC_PGUP,         LCTL(KC_LEFT),       LCTL(KC_RGHT),        XXXXXXX,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
         XXXXXXX,             XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,              _______,                                  _______,              KC_LEFT,           KC_DOWN,               KC_UP,          KC_RIGHT,          LCTL(KC_W),
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        _______,              XXXXXXX,           TG(_NAV),            XXXXXXX,            XXXXXXX,              _______,                                  _______,              XXXXXXX,           KC_PGDN,          LCTL(KC_6),         LCTL(KC_RBRC),         KC_END
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

/* Layer 4: Colemak-DH (alternate base)
 *
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |    Q    |    W    |    F    |    P    |    B    |                      |    J    |    L    |    U    |    Y    |    P    |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |    A    |  R/CTL  |  S/GUI  |  T/ALT  |    G    |                      |    H    |  N/ALT  |  E/GUI  |  I/CTL  |    O    |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * | SHF/Z   |    X    |    C    |    D    |    V    |                      |    K    |    M    |    ,    |    .    | SHF/ /? |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 *                                .--------.--------.--------.      .--------.--------.--------.
 *                                | _NUM  |_SPEC/TAB|_NAV/ESC|      |   ENT  |  SPC   |   BSP  |
 *                                .--------.--------.--------.      .--------.--------.--------.
 */

 [_DH] = LAYOUT_split_3x5_3(
 //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
          KC_Q,               KC_W,               KC_F,                KC_P,               KC_B,           LT(_NAV, KC_ESC),                             KC_ENT,                  KC_J,               KC_L,              KC_U,               KC_Y,               KC_P,
 //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
          KC_A,            LCTL_T(KC_R),       LGUI_T(KC_S),        LALT_T(KC_T),          KC_G,           LT(_SPEC, KC_TAB),                            KC_SPC,                  KC_H,          RALT_T(KC_N),       RGUI_T(KC_E),       RCTL_T(KC_I),           KC_O,
 //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
          LSFT_T(KC_Z),       KC_X,               KC_C,                KC_D,                KC_V,              MO(_NUM),                                 KC_BSPC,                 KC_K,               KC_M,              KC_COMM,            KC_DOT,       RSFT_T(KC_SLSH)
 //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
    )
};
