# ─────────────────────────────────────────────────────────────────────────────
# MCU / Board
# ─────────────────────────────────────────────────────────────────────────────

# MCU name
# Arduino Pro Micro with the Caterina bootloader
MCU = atmega32u4   # 8-bit AVR microcontroller used by Pro Micro

# Bootloader selection
BOOTLOADER = caterina   # Caterina bootloader (Pro Micro default)


# ─────────────────────────────────────────────────────────────────────────────
# Core Build Options
#   change yes to no to disable
# ─────────────────────────────────────────────────────────────────────────────

BOOTMAGIC_ENABLE = no     # Virtual DIP switch configuration (Bootmagic)
MOUSEKEY_ENABLE  = no     # Mouse keys
EXTRAKEY_ENABLE  = no     # Consumer/System control keys
TAP_DANCE_ENABLE = no     # Tap Dance feature
COMBO_ENABLE     = yes    # Combo keys
#AUTO_SHIFT_ENABLE = yes  # Auto Shift (currently disabled / optional)


# ─────────────────────────────────────────────────────────────────────────────
# Debug / Console (leave disabled unless actively debugging)
# ─────────────────────────────────────────────────────────────────────────────

#DEBUG_ENABLE  = yes      # Enable full debug
#DEBUG_ACTION  = yes      # Enable debug actions
#debug_matrix  = true     # Matrix debug (runtime flag)
#debug_keyboard= true     # Keyboard debug (runtime flag)
#debug_mouse   = true     # Mouse debug (runtime flag)
#CONSOLE_ENABLE = yes     # Console for debug (USB CDC)
#COMMAND_ENABLE = no      # Commands for debug & configuration


# ─────────────────────────────────────────────────────────────────────────────
# Power / Sleep / NKRO / Backlight
# ─────────────────────────────────────────────────────────────────────────────

# Do not enable SLEEP_LED_ENABLE. It uses the same timer as BACKLIGHT_ENABLE.
SLEEP_LED_ENABLE = no     # Breathing sleep LED during USB suspend
# If NKRO doesn't work, see: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE      = yes    # USB N-Key Rollover
BACKLIGHT_ENABLE = no     # Switch backlight (not underglow)


# ─────────────────────────────────────────────────────────────────────────────
# RGB Lighting / Matrix
# (Matrix and underglow are mutually exclusive in most configs — you’ve chosen underglow)
# ─────────────────────────────────────────────────────────────────────────────

RGB_MATRIX_ENABLE   = no      # Per-key RGB matrix (disabled)
RGBLIGHT_ENABLE     = yes     # RGB underglow (enabled)
RGB_MATRIX_DRIVER   = WS2812  # RGB matrix driver support (kept for completeness)
RGBLIGHT_DEFAULT_VAL= 50      # Default underglow brightness (0–255)


# ─────────────────────────────────────────────────────────────────────────────
# Audio
# ─────────────────────────────────────────────────────────────────────────────

AUDIO_ENABLE = no            # Audio output (piezo/buzzer)


# ─────────────────────────────────────────────────────────────────────────────
# Split / Link-Time Optimization / VIA
# ─────────────────────────────────────────────────────────────────────────────

SPLIT_KEYBOARD = yes   # Split keyboard build
LTO_ENABLE     = yes   # Link Time Optimization (smaller/faster firmware)
VIA_ENABLE     = no    # VIA configurator support


# ─────────────────────────────────────────────────────────────────────────────
# Feature Support Flags (capability hints; do not toggle runtime behavior)
# ─────────────────────────────────────────────────────────────────────────────

AUDIO_SUPPORTED     = no
RGB_MATRIX_SUPPORTED= no
RGBLIGHT_SUPPORTED  = yes


# ─────────────────────────────────────────────────────────────────────────────
# Layouts
# ─────────────────────────────────────────────────────────────────────────────

LAYOUTS = split_3x5_3  # Community layout macro used by this keymap
