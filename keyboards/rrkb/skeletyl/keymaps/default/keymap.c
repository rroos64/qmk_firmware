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

// Tap Dance declarations
enum {
    TD_Q_ESC,
    TD_ENT_SHFTENT,
    TD_BSP_CTRLBS,
    TD_CTRL_A,
    TD_CTRL_S,
    TD_CTRL_Z,
    TD_CTR_X,
    TD_CTRL_C,
    TD_CTRL_V,
    TD_CTRL_Y,
    TD_A_TAB,
    TD_NAV,
    TD_BASE,
    TD_SHFT_UP,
    TD_SHFT_DOWN,
    TD_SHFT_R,
    TD_SHFT_L,
    TD_SHFT_HOME,
    TD_SHFT_END,
    TD_P_DEL,
    TD_SHFT_CAPS
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
  return true;
};

void dance_ons_shft_caps(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code16(OSM(MOD_LSFT));
    } else {
        register_code(KC_CAPS);
    }
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_Q_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESCAPE),
    [TD_ENT_SHFTENT] = ACTION_TAP_DANCE_DOUBLE(KC_ENT, LSFT(KC_ENT)),
    [TD_BSP_CTRLBS] = ACTION_TAP_DANCE_DOUBLE(KC_BSPACE, LCTL(KC_BSPACE)),
    [TD_CTRL_A] = ACTION_TAP_DANCE_DOUBLE(KC_A, LCTL(KC_A)),
    [TD_CTRL_S] = ACTION_TAP_DANCE_DOUBLE(KC_S, LCTL(KC_S)),
    [TD_CTRL_Z] = ACTION_TAP_DANCE_DOUBLE(KC_Z, LCTL(KC_Z)),
    [TD_CTR_X] = ACTION_TAP_DANCE_DOUBLE(KC_X, LCTL(KC_X)),
    [TD_CTRL_C] = ACTION_TAP_DANCE_DOUBLE(KC_C, LCTL(KC_C)),
    [TD_CTRL_V] = ACTION_TAP_DANCE_DOUBLE(KC_V, LCTL(KC_V)),
    [TD_CTRL_Y] = ACTION_TAP_DANCE_DOUBLE(KC_Y, LCTL(KC_Y)),
    [TD_A_TAB] = ACTION_TAP_DANCE_DOUBLE(KC_A, KC_TAB),
    [TD_NAV] = ACTION_TAP_DANCE_LAYER_MOVE(KC_N, _NAV),
    [TD_BASE] = ACTION_TAP_DANCE_LAYER_MOVE(XXXXXXX, _BASE),
    [TD_SHFT_UP] = ACTION_TAP_DANCE_DOUBLE(KC_UP, LSFT(KC_UP)),
    [TD_SHFT_DOWN] = ACTION_TAP_DANCE_DOUBLE(KC_DOWN, LSFT(KC_DOWN)),
    [TD_SHFT_R] = ACTION_TAP_DANCE_DOUBLE(KC_RIGHT, LSFT(KC_RIGHT)),
    [TD_SHFT_L] = ACTION_TAP_DANCE_DOUBLE(KC_LEFT, LSFT(KC_LEFT)),
    [TD_SHFT_HOME] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, LSFT(KC_HOME)),
    [TD_SHFT_END] = ACTION_TAP_DANCE_DOUBLE(KC_END, LSFT(KC_END)),
    [TD_P_DEL] = ACTION_TAP_DANCE_DOUBLE(KC_P, KC_DEL),
    [TD_SHFT_CAPS] = ACTION_TAP_DANCE_FN(dance_ons_shft_caps),
};

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

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_base_layer,
    my_capslock_layer,  // Overrides base layer
    my_num_layer,    // Overrides other layers
    my_spec_layer,     // Overrides other layers
    my_nav_layer
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
 * |  Q/ESC  |    W    |    E    |    R    |    T    |                      |    Y    |    U    |    I    |    O    |  P/DEL  |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |  A/TAB  |    S    |    D    |    F    |    G    |                      |    H    |    J    |    K    |    L    |   ' "   |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 * |  SHF/Z  |    X    |    C    |    V    |    B    |                      |    N    |    M    |   < ,   |   > .   | SHF/ /? |
 * ,---------.---------.---------.---------.---------.                      ,---------.---------.---------.---------.---------.
 *                                .--------.--------.--------.      .--------.--------.--------.
 *                                |  _NUM  |  _SPEC |  _NUM  |      |ENT/CTRL|SPC/WIN |ALT/BSP |
 *                                .--------.--------.--------.      .--------.--------.--------.
 */

  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        TD(TD_Q_ESC),           KC_W,              KC_E,               KC_R,            KC_T,                   TT(_NAV),                              RCTL_T(KC_ENT),            KC_Y,             KC_U,              KC_I,              KC_O,             TD(TD_P_DEL),
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        TD(TD_A_TAB),           KC_S,              KC_D,               KC_F,            KC_G,                   TT(_SPEC),                             RGUI_T(KC_SPC),            KC_H,             KC_J,              KC_K,              KC_L,               KC_QUOT,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        LSFT_T(KC_Z),           KC_X,              KC_C,               KC_V,            KC_B,                   TT(_NUM),                              RALT_T(KC_BSPC),           KC_N,             KC_M,              KC_COMM,           KC_DOT,           RSFT_T(KC_SLSH)
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

  [_NUM] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F1,               KC_F2,             KC_F3,             KC_F4,             KC_F5,                _______,                                  _______,            KC_KP_ASTERISK,         KC_7,              KC_8,             KC_9,              KC_KP_MINUS,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F6,               KC_F7,             KC_F8,             KC_F9,            KC_F10,                _______,                                  _______,            KC_KP_SLASH,            KC_4,              KC_5,             KC_6,              KC_KP_PLUS,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F11,              KC_F12,          XXXXXXX,            XXXXXXX,           XXXXXXX,               _______,                                  _______,                KC_0,               KC_1,              KC_2,             KC_3,              KC_DOT
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

  [_SPEC] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_EXLM,           KC_AT,              KC_HASH,           KC_DLR,              KC_PERC,              _______,                                  _______,              KC_CIRC,           KC_AMPR,             KC_ASTR,           KC_LPRN,            KC_RPRN,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_GRAVE,          XXXXXXX,            XXXXXXX,           XXXXXXX,             XXXXXXX,              _______,                                  _______,              KC_LBRC,           KC_RBRC,             KC_BSLS,           KC_SCLN,            _______,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            _______,           KC_CAPS,            KC_INS,            XXXXXXX,             XXXXXXX,              _______,                                  _______,              KC_MINS,           KC_EQL,              _______,           _______,            _______
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

  [_NAV] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
         KC_ESCAPE,           XXXXXXX,         TD(TD_SHFT_UP),        KC_PGUP,            XXXXXXX,              _______,                                  _______,              XXXXXXX,           XXXXXXX,             XXXXXXX,           XXXXXXX,            XXXXXXX,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
      TD(TD_SHFT_HOME),     TD(TD_SHFT_L),        XXXXXXX,         TD(TD_SHFT_R),     TD(TD_SHFT_END),          _______,                                  _______,              XXXXXXX,           XXXXXXX,             XXXXXXX,           XXXXXXX,            XXXXXXX,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        _______,              XXXXXXX,         TD(TD_SHFT_DOWN),      KC_PGDOWN,          XXXXXXX,              _______,                                  _______,              XXXXXXX,           XXXXXXX,             XXXXXXX,           XXXXXXX,            _______
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  )

};



