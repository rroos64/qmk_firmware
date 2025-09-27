
// SPDX-License-Identifier: GPL-2.0-or-later
// RRKB Skeletyl — clean keymap
//  - Thumbs (left cluster order): inner=NUM, middle=SPEC/Tab, outer=NAV/Esc
//  - Hold = momentary; while holding press C = latch; press C again = unlatch
//  - PDF_QW / PDF_DH / PDF_TOG (persistent default layer control)
//  - Home‑row Mod‑Taps tuned for fewer misfires
//  - Lightweight RGB: Caps magenta, NUM red, SPEC green, NAV blue, soft DH hint
//  - No TG() landmines on layers; Tap Dance optional (stub if enabled)

#include QMK_KEYBOARD_H
#include "skeletyl.h"
#ifdef COMBO_ENABLE
#    include "process_combo.h"
#endif
#ifdef TAP_DANCE_ENABLE
#    include "process_tap_dance.h"
#endif

// ─────────────────────────────────────────────────────────────────────────────
// Layers & custom keycodes
// ─────────────────────────────────────────────────────────────────────────────
enum layers { _BASE, _DH, _NUM, _SPEC, _NAV };

enum custom_keycodes {
    PDF_QW = SAFE_RANGE,
    PDF_DH,
    PDF_TOG,
};

// ─────────────────────────────────────────────────────────────────────────────
// Home‑row Mod‑taps helper (adjust if your HRMs differ)
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
// Thumb aliases (inner/middle/outer = NUM/SPEC/NAV)
// ─────────────────────────────────────────────────────────────────────────────
#define THUMB_OUTER  LT(_NAV,  KC_ESC)
#define THUMB_MIDDLE LT(_SPEC, KC_TAB)
#define THUMB_INNER  MO(_NUM)

// ─────────────────────────────────────────────────────────────────────────────
// Latch state (compact helpers to avoid big if‑chains)
// ─────────────────────────────────────────────────────────────────────────────
static int8_t latched_layer = -1; // -1 none, else one of _NAV/_SPEC/_NUM
static int8_t held_layer    = -1; // -1 none, else current held thumb layer
static bool   thumb_outer_down  = false;
static bool   thumb_middle_down = false;
static bool   thumb_inner_down  = false;

static inline int8_t thumb_layer_from_key(uint16_t keycode) {
    if (keycode == THUMB_OUTER)  return _NAV;
    if (keycode == THUMB_MIDDLE) return _SPEC;
    if (keycode == THUMB_INNER)  return _NUM;
    return -1;
}
static inline void set_thumb_flag(uint16_t keycode, bool down) {
    if (keycode == THUMB_OUTER)  thumb_outer_down  = down;
    if (keycode == THUMB_MIDDLE) thumb_middle_down = down;
    if (keycode == THUMB_INNER)  thumb_inner_down  = down;
}
static inline bool any_thumb_down(void) {
    return thumb_outer_down || thumb_middle_down || thumb_inner_down;
}

// ─────────────────────────────────────────────────────────────────────────────
    // Keymaps (LAYOUT_split_3x5_3) — left thumb order: inner, middle, outer
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
// Combo: outer thumb + Enter → toggle default layer
// ─────────────────────────────────────────────────────────────────────────────
#ifdef COMBO_ENABLE
// Two combos:
//  • CB_PDF_TOG  = THUMB_OUTER + Enter  → toggle default layer (QW↔DH)
//  • CB_BOOT     = THUMB_OUTER + THUMB_MIDDLE + THUMB_INNER → jump to bootloader

enum combo_events { CB_PDF_TOG, CB_BOOT };

const uint16_t PROGMEM combo_pdf_tog[] = { THUMB_OUTER, KC_ENT, COMBO_END };
const uint16_t PROGMEM combo_boot[]    = { THUMB_OUTER, THUMB_MIDDLE, THUMB_INNER, COMBO_END };

combo_t key_combos[] = {
    [CB_PDF_TOG] = COMBO_ACTION(combo_pdf_tog),
    [CB_BOOT]    = COMBO_ACTION(combo_boot),
};

// Strong symbol so combos link on all QMK branches (avoid LTO drop)
__attribute__((used)) const uint16_t COMBO_LEN =
    (uint16_t)(sizeof(key_combos) / sizeof(key_combos[0]));

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!pressed) return; // act on press only
    switch (combo_index) {
        case CB_PDF_TOG:
            if (layer_state_cmp(default_layer_state, _DH)) {
                set_single_persistent_default_layer(_BASE);
            } else {
                set_single_persistent_default_layer(_DH);
            }
            break;
        case CB_BOOT:
            reset_keyboard(); // soft bootloader entry
            break;
    }
}
#endif

// ─────────────────────────────────────────────────────────────────────────────
// process_record_user: default-layer keys + compact thumb latch handling
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

    // Thumb tracking (don’t block LT/MO normal behavior)
    int8_t tl = thumb_layer_from_key(keycode);
    if (tl >= 0) {
        if (record->event.pressed) {
            set_thumb_flag(keycode, true);
            held_layer = tl;
        } else {
            set_thumb_flag(keycode, false);

            if (latched_layer == tl) {
                // IMPORTANT: swallow the release so LT/MO can't layer_off()
                if (!any_thumb_down()) { held_layer = -1; }
                return false;
            }

            if (!any_thumb_down()) { held_layer = -1; }
        }
        return true;
    }

    // C toggles latch: prefer currently-held thumb; else toggle existing latch
    if (keycode == KC_C && record->event.pressed) {
        int8_t target = any_thumb_down() ? held_layer : latched_layer;
        if (target >= 0) {
            if (latched_layer == target) {
                layer_off(latched_layer);
                latched_layer = -1;
            } else {
                if (latched_layer >= 0) layer_off(latched_layer);
                layer_on(target);
                latched_layer = target;
            }
            return false; // swallow 'c' on toggle; delete if you want to type 'c'
        }
    }

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Mod‑Tap tuning — fewer HRM misfires, snappier thumbs
// ─────────────────────────────────────────────────────────────────────────────
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    if (is_hrm(keycode)) return 215;                              // HRMs
    if (keycode == THUMB_OUTER || keycode == THUMB_MIDDLE) return 160; // Esc/Tab LTs
    return TAPPING_TERM;
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    return is_hrm(keycode); // HRMs: rolling into next key stays a TAP
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    return false;           // Don’t auto‑promote to HOLD on overlap
}

#ifdef QUICK_TAP_TERM
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    if (is_hrm(keycode)) return 90; // helps double letters like "ll" / "ee"
    return QUICK_TAP_TERM;
}
#endif

// ─────────────────────────────────────────────────────────────────────────────
// RGBLIGHT: per‑layer colours + soft Colemak‑DH hint
// ─────────────────────────────────────────────────────────────────────────────
#ifdef RGBLIGHT_ENABLE
const rgblight_segment_t PROGMEM dh_hint_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLED_NUM, HSV_CYAN}
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

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    dh_hint_layer,  // 0: default DH hint (lowest priority)
    caps_layer,     // 1: CapsLock
    num_layer,      // 2: _NUM
    spec_layer,     // 3: _SPEC
    nav_layer       // 4: _NAV
);

void keyboard_post_init_user(void) {
    rgblight_layers = my_rgb_layers;                   // register segments
    rgblight_enable_noeeprom();                        // force neutral base
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(HSV_OFF);
    rgblight_set_layer_state(0, layer_state_cmp(default_layer_state, _DH));
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(1, led_state.caps_lock);
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(2, layer_state_cmp(state, _NUM));
    rgblight_set_layer_state(3, layer_state_cmp(state, _SPEC));
    rgblight_set_layer_state(4, layer_state_cmp(state, _NAV));
    return state;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _DH));
    return state;
}
#endif

// ─────────────────────────────────────────────────────────────────────────────
// Tap Dance (optional stub)
// ─────────────────────────────────────────────────────────────────────────────
#ifdef TAP_DANCE_ENABLE
qk_tap_dance_action_t tap_dance_actions[] = {};
#endif
