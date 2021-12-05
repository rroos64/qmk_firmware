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
    TD_SPACE_TAB,
    TD_NAV,
    TD_BASE,
    TD_SHFT_UP,
    TD_SHFT_DOWN,
    TD_SHFT_R,
    TD_SHFT_L,
    TD_SHFT_HOME,
    TD_SHFT_END,
    TD_P_DEL,
    TD_SHFT_CAPS,
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
    [TD_SPACE_TAB] = ACTION_TAP_DANCE_DOUBLE(KC_SPC, KC_TAB),
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT_split_3x5_3(

  /* Layer 0: Base keys
 *
 * ,-----.-----.-----.-----.-----.                      ,-----.-----.-----.-----.-----.
 * | Q   | W   | E   | R   | T   |                      | Y   | U   | I   | O   | P   |
 * |-----+-----+-----+-----+-----|                      |-----+-----+-----+-----+-----|
 * | A   | S   | D   | F   | G   |                      | H   | J   | K   | L   | ' " |
 * |-----+-----+-----+-----+-----+                      |-----+-----+-----+-----+-----|
 * |SHF/Z| X   | C   | V   | B   |                      | N   | M   | < , | > . |SHF/?|
 * `-----'-----'-----'-----'-----'                      `-----'-----'-----'-----'-----'
 *               .-------.-------.-------.      .-------.-------.-------.
 *               |OPT/ESC|  CMD  |CTR/TAB|      | ENTER |L1/SPC |L2/BSP |
 *               '-------'-------'-------'      '-------'-------'-------'
 */

  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        TD(TD_Q_ESC),            KC_W,              KC_E,               KC_R,            KC_T,                  TT(_NUM),                                TT(_SPEC),           TD(TD_CTRL_Y),         KC_U,              KC_I,              KC_O,               TD(TD_P_DEL),
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+-----------------+-------------------+-------------------+-------------------|
        TD(TD_CTRL_A),      TD(TD_CTRL_S),          KC_D,               KC_F,            KC_G,             TD(TD_ENT_SHFTENT),                        TD(TD_SPACE_TAB),            KC_H,             KC_J,              KC_K,              KC_L,               KC_DQUO,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        TD(TD_CTRL_Z),      TD(TD_CTR_X),      TD(TD_CTRL_C),     TD(TD_CTRL_V),         KC_B,             LSFT_T(KC_CAPS),                           TD(TD_BSP_CTRLBS),         TD(TD_NAV),             KC_M,              KC_COMM,           KC_DOT,              KC_QUES
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

  [_NUM] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F1,               KC_F2,             KC_F3,             KC_F4,             KC_F5,                _______,                                  XXXXXXX,            KC_KP_SLASH,            KC_7,              KC_8,             KC_9,              KC_KP_MINUS,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F6,               KC_F7,             KC_F8,             KC_F9,            KC_F10,                _______,                                  _______,            KC_KP_ASTERISK,         KC_4,              KC_5,             KC_6,              KC_KP_PLUS,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_F11,              KC_F12,          XXXXXXX,            XXXXXXX,           XXXXXXX,               _______,                                  _______,                KC_0,               KC_1,              KC_2,             KC_3,              KC_DOT
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

  [_SPEC] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_EXLM,           KC_AT,              KC_HASH,           KC_DLR,              KC_PERC,              XXXXXXX,                                  _______,              KC_CIRC,           KC_AMPR,             KC_ASTR,           KC_LPRN,            KC_RPRN,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            XXXXXXX,           XXXXXXX,            XXXXXXX,           XXXXXXX,             XXXXXXX,              _______,                                  _______,               KC_MINS,           KC_EQL,              KC_LBRC,           KC_RBRC,            KC_BSLS,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            XXXXXXX,           XXXXXXX,            XXXXXXX,           XXXXXXX,             XXXXXXX,              _______,                                  _______,              KC_UNDS,           KC_PLUS,             KC_LCBR,           KC_RCBR,            KC_PIPE
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  ),

  [_NAV] = LAYOUT_split_3x5_3(
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
         KC_ESCAPE,           KC_PGUP,         TD(TD_SHFT_UP),        XXXXXXX,            XXXXXXX,              _______,                                  _______,              XXXXXXX,           XXXXXXX,             XXXXXXX,           XXXXXXX,            XXXXXXX,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
      TD(TD_SHFT_HOME),     TD(TD_SHFT_L),        XXXXXXX,         TD(TD_SHFT_R),     TD(TD_SHFT_END),          _______,                                  _______,              XXXXXXX,           XXXXXXX,             XXXXXXX,           XXXXXXX,            XXXXXXX,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        XXXXXXX,              KC_PGDOWN,      TD(TD_SHFT_DOWN),      XXXXXXX,             XXXXXXX,              _______,                                  _______,              TD(TD_BASE),           XXXXXXX,             XXXXXXX,           XXXXXXX,            XXXXXXX
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
  )

};



