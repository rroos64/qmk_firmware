
// SPDX-License-Identifier: GPL-2.0-or-later
// RRKB Skeletyl — cleaned keymap with:
//  - Left thumbs: LT(NAV,Esc) / LT(SPEC,Tab) / MO(NUM)
//  - Hold = momentary; while holding press C = latch; press C again = unlatch
//  - Persistent default layer switchers: PDF_QW / PDF_DH / PDF_TOG
//  - Home‑row mod‑taps tuned to reduce misfires ("se" etc.)
//  - No layer self‑toggles (TG on layers removed)
//  - No Tap Dance here (set TAP_DANCE_ENABLE = no in rules.mk to save space)
//  - Optional: one combo (outer‑thumb + Enter => PDF_TOG) if COMBO_ENABLE=yes

#include QMK_KEYBOARD_H   // pulls in core QMK headers incl. action_tapping, rgblight, etc.
#include "skeletyl.h"     // keyboard layout header (LAYOUT_split_3x5_3)
#ifdef COMBO_ENABLE
#    include "process_combo.h"
#endif
#ifdef TAP_DANCE_ENABLE
#    include "process_tap_dance.h"
// We aren't using Tap Dance actions, but if the feature is enabled in rules.mk
// we must still provide the symbol to satisfy the linker.
qk_tap_dance_action_t tap_dance_actions[] = {};
#endif

// ─────────────────────────────────────────────────────────────────────────────
// Layers
// ─────────────────────────────────────────────────────────────────────────────
enum layers { _BASE, _NUM, _SPEC, _NAV, _DH };

// ─────────────────────────────────────────────────────────────────────────────
// Custom keycodes (persistent default layer control)
// ─────────────────────────────────────────────────────────────────────────────
enum custom_keycodes {
    PDF_QW = SAFE_RANGE,
    PDF_DH,
    PDF_TOG,
};

// ─────────────────────────────────────────────────────────────────────────────
// Home‑row Mod‑taps helper (adjust to match your actual HRMs)
// Left:  S(Dual=Ctrl), D(=GUI), F(=Alt), G(=Shift)
// Right: H(=Shift),   J(=Alt), K(=GUI), L(=Ctrl)
// ─────────────────────────────────────────────────────────────────────────────
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

// ─────────────────────────────────────────────────────────────────────────────
// Thumb‑latch state (for LT(_NAV,Esc), LT(_SPEC,Tab), MO(_NUM))
// ─────────────────────────────────────────────────────────────────────────────
static uint8_t lt_left_layer = 0;  // remembered layer to latch (NAV/SPEC/NUM)
static bool    lt_left_held  = false;
static bool    lt_latched    = false;

// Convenience aliases — RIGHT/LEFT thumb cluster mapping (inner/middle/outer)
// As requested: inner = _NUM, middle = _SPEC/Tab, outer = _NAV/Esc
#define THUMB_OUTER  LT(_NAV,  KC_ESC)
#define THUMB_MIDDLE LT(_SPEC, KC_TAB)
#define THUMB_INNER  MO(_NUM)

// ─────────────────────────────────────────────────────────────────────────────
// Keymaps (LAYOUT_split_3x5_3)
// NOTE: This is a clean, conservative QWERTY + Colemak-DH example. Replace any
// positions to suit your exact board, but keep the three left thumbs as below.
// ─────────────────────────────────────────────────────────────────────────────
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
        KC_Q,                   KC_W,              KC_E,               KC_R,            KC_T,                THUMB_OUTER,                                  KC_ENT,                 KC_Y,             KC_U,              KC_I,              KC_O,                  KC_P,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
            KC_A,               LCTL_T(KC_S),       LGUI_T(KC_D),       LALT_T(KC_F),       KC_G,            THUMB_MIDDLE,                                 KC_SPC,                 KC_H,           RALT_T(KC_J),      RGUI_T(KC_K),       RCTL_T(KC_L),           KC_DEL,
  //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
        LSFT_T(KC_Z),           KC_X,              KC_C,               KC_V,            KC_B,                 THUMB_INNER,                                KC_BSPC,                KC_N,             KC_M,              KC_COMM,           KC_DOT,           RSFT_T(KC_SLSH)
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
            KC_F11,              KC_F12,           _______,            XXXXXXX,          XXXXXXX,               XXXXXXX,                                  _______,                KC_0,               KC_1,              KC_2,             KC_3,              KC_DOT
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
            _______,           KC_CAPS,             _______,          XXXXXXX,             XXXXXXX,              _______,                                  _______,              KC_MINS,           KC_EQL,              _______,           _______,            _______
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
        _______,              XXXXXXX,            _______,            XXXXXXX,            XXXXXXX,              _______,                                  _______,              XXXXXXX,           KC_PGDN,          LCTL(KC_6),         LCTL(KC_RBRC),         KC_END
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
          KC_Q,               KC_W,               KC_F,                KC_P,               KC_B,             THUMB_OUTER,                                KC_ENT,                  KC_J,               KC_L,              KC_U,               KC_Y,               KC_P,
 //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
          KC_A,            LCTL_T(KC_R),       LGUI_T(KC_S),        LALT_T(KC_T),          KC_G,             THUMB_MIDDLE,                                KC_SPC,                  KC_H,          RALT_T(KC_N),       RGUI_T(KC_E),       RCTL_T(KC_I),           KC_O,
 //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
          LSFT_T(KC_Z),       KC_X,               KC_C,                KC_D,                KC_V,            THUMB_INNER,                                KC_BSPC,                 KC_K,               KC_M,              KC_COMM,            KC_DOT,       RSFT_T(KC_SLSH)
 //|-------------------+-------------------+------------------+-------------------+-------------------+  +-------------------|                    |-------------------+  +-------------------+------------------+-------------------+-------------------+-------------------|
    )
};

// ─────────────────────────────────────────────────────────────────────────────
// Optional: simple combo to toggle default layer (outer‑thumb + Enter)
// Requires: COMBO_ENABLE = yes and (optionally) COMBO_ALLOW_ACTION_KEYS
// ─────────────────────────────────────────────────────────────────────────────
#ifdef COMBO_ENABLE
enum combo_events { CB_PDF_TOG };
const uint16_t PROGMEM combo_pdf_tog[] = { THUMB_OUTER, KC_ENT, COMBO_END };
combo_t key_combos[] = { [CB_PDF_TOG] = COMBO_ACTION(combo_pdf_tog), };
// Provide COMBO_LEN for branches that require a strong symbol; mark used to avoid LTO stripping
__attribute__((used)) uint16_t COMBO_LEN = (uint16_t)(sizeof(key_combos) / sizeof(key_combos[0]));
// Also provide legacy macro in case your core references it at compile time
#ifndef COMBO_LEN
# define COMBO_LEN (sizeof(key_combos) / sizeof(key_combos[0]))
#endif
void process_combo_event(uint16_t combo_index, bool pressed) {
    if (combo_index == CB_PDF_TOG && pressed) {
        // Toggle persistent default layer between QWERTY and DH
        if (layer_state_cmp(default_layer_state, _DH)) {
            set_single_persistent_default_layer(_BASE);
        } else {
            set_single_persistent_default_layer(_DH);
        }
    }
}
#endif

// ─────────────────────────────────────────────────────────────────────────────
// Persistent default layer keycodes
// ─────────────────────────────────────────────────────────────────────────────
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case PDF_QW: set_single_persistent_default_layer(_BASE); return false;
            case PDF_DH: set_single_persistent_default_layer(_DH);   return false;
            case PDF_TOG:
                if (layer_state_cmp(default_layer_state, _DH)) {
                    set_single_persistent_default_layer(_BASE);
                } else {
                    set_single_persistent_default_layer(_DH);
                }
                return false;
        }
    }

    // ── Thumb‑hold tracking (don’t block normal LT/MO behavior) ────────────
    if (keycode == THUMB_OUTER || keycode == THUMB_MIDDLE || keycode == THUMB_INNER) {
        if (record->event.pressed) {
            lt_left_held  = true;
            lt_left_layer = (keycode == THUMB_OUTER) ? _NAV
                           : (keycode == THUMB_MIDDLE) ? _SPEC
                                                        : _NUM;
        } else {
            lt_left_held  = false;
            // If latched, re‑assert the layer after MO/LT tries to clear it
            if (lt_latched) { layer_on(lt_left_layer); }
        }
        return true; // keep Esc/Tab taps working
    }

    // ── C key latches/unlatches the last held left‑thumb layer ─────────────
    if (keycode == KC_C && record->event.pressed) {
        if (lt_left_held && !lt_latched) {
            layer_on(lt_left_layer);
            lt_latched = true;
            return false; // swallow 'c' on latch (delete if you prefer to type 'c')
        } else if (lt_latched) {
            layer_off(lt_left_layer);
            lt_latched = false;
            return false; // swallow 'c' on unlatch
        }
    }

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Mod‑Tap tuning — reduce HRM misfires on fast rolls
// * Per‑key TAPPING_TERM: 220ms on HRMs; snappier 170ms on the two LT thumbs
// * Favor TAP on HRMs when interrupted by another key
// * Do not auto‑promote to HOLD when another key is pressed (safer HRMs)
// ─────────────────────────────────────────────────────────────────────────────
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    if (is_hrm(keycode)) return 220;                             // HRMs
    if (keycode == THUMB_OUTER || keycode == THUMB_MIDDLE) return 170; // LT thumbs
    return TAPPING_TERM;
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    return is_hrm(keycode); // HRMs: rolling into the next key stays a TAP
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    return false;           // Don’t auto‑promote to HOLD on overlap (safer HRMs)
    // If you want it ON for non‑HRMs only: return !is_hrm(keycode);
}

#ifdef QUICK_TAP_TERM
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    if (is_hrm(keycode)) return 90; // helps double‑letters like "ll" / "ee"
    return QUICK_TAP_TERM;
}
#endif

// ─────────────────────────────────────────────────────────────────────────────
// RGBLIGHT per‑layer cues + soft hint when default = Colemak‑DH
// ─────────────────────────────────────────────────────────────────────────────
#ifdef RGBLIGHT_ENABLE
// • Keep this lightweight: RGBLIGHT_LAYERS only (no animations)
// • rules.mk:  RGBLIGHT_ENABLE = yes (RGB_MATRIX_ENABLE = no)
// • config.h:  pin/count already defined; add #define RGBLIGHT_LAYERS

// Colour plan (all LEDs, easy to see):
//  DH default hint → soft cyan (low V)  [lowest priority layer]
//  CapsLock       → magenta
//  _NUM           → red
//  _SPEC          → green
//  _NAV           → blue

// Soft cyan hint when default layer is _DH
const rgblight_segment_t PROGMEM dh_hint_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLED_NUM, HSV_CYAN}   // H≈cyan, lower brightness for a soft glow
);
const rgblight_segment_t PROGMEM caps_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLED_NUM, HSV_MAGENTA}
);
const rgblight_segment_t PROGMEM num_layer[]  = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLED_NUM, HSV_RED}
);
const rgblight_segment_t PROGMEM spec_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLED_NUM, HSV_GREEN}
);
const rgblight_segment_t PROGMEM nav_layer[]  = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLED_NUM, HSV_BLUE}
);

// Order matters: later layers take priority over earlier ones where they overlap.
// Put the DH hint first so NAV/SPEC/NUM and Caps can override it when active.
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    dh_hint_layer,  // index 0
    caps_layer,     // index 1
    num_layer,      // index 2
    spec_layer,     // index 3
    nav_layer       // index 4
);

void keyboard_post_init_user(void) {
    rgblight_layers = my_rgb_layers; // register layers

    // Ensure a known, neutral base on boot (overrides any red color left in EEPROM)
    rgblight_enable_noeeprom(); // turn RGB on (no EEPROM write)
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT); // static mode so layers show cleanly
    rgblight_sethsv_noeeprom(HSV_OFF); // base OFF; layers will add color

    // Apply DH hint immediately if DH is the default layer
    rgblight_set_layer_state(0 /*dh_hint_layer*/, layer_state_cmp(default_layer_state, _DH));
}

// Host LED feedback (CapsLock)
bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(1 /*caps_layer*/, led_state.caps_lock);
    return true; // let QMK handle the actual host LED too
}

// Momentary per-layer colours
layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(2 /*num_layer*/,  layer_state_cmp(state, _NUM));
    rgblight_set_layer_state(3 /*spec_layer*/, layer_state_cmp(state, _SPEC));
    rgblight_set_layer_state(4 /*nav_layer*/,  layer_state_cmp(state, _NAV));
    return state;
}

// Default-layer change: show soft cyan hint when default = _DH
layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0 /*dh_hint_layer*/, layer_state_cmp(state, _DH));
    return state;
}
#endif
