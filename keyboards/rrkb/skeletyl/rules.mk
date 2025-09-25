# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = caterina

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no     # Virtual DIP switch configuration
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
TAP_DANCE_ENABLE = yes
COMBO_ENABLE = yes		# Combo keys
#AUTO_SHIFT_ENABLE = yes

# Debug settigs
#DEBUG_ENABLE= yes
#DEBUG_ACTION = yes
#debug_matrix = true
#debug_keyboard= true
#debug_mouse=true;
#CONSOLE_ENABLE = yes         # Console for debug
#COMMAND_ENABLE = no         # Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = yes            # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGB_MATRIX_ENABLE = no      # Enable keyboard RGB matrix (do not use together with RGBLIGHT_ENABLE)
RGBLIGHT_ENABLE = yes        # Enable keyboard RGB underglow
RGB_MATRIX_DRIVER = WS2812  # RGB matrix driver support
RGBLIGHT_DEFAULT_VAL = 50

AUDIO_ENABLE = no           # Audio output
SPLIT_KEYBOARD = yes
LTO_ENABLE = yes
VIA_ENABLE = yes

AUDIO_SUPPORTED = no
RGB_MATRIX_SUPPORTED = no
RGBLIGHT_SUPPORTED = yes

LAYOUTS = split_3x5_3
