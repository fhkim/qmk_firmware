/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
Modified by fhkim
*/

#include QMK_KEYBOARD_H
#include <stdio.h>



// TODO: Add Tap Dancing, to make GUI tap twice is ENT
// TODO: Add COLEMAK DH Layer
// TODO: consider a mouse nav layer


#define _COLEMAK_DH 0
#define _QWERTY 1
#define _NUM 2
#define _SYM 3
#define _FN 4
#define _MOUSE 5
#define _QK 6


// Tap Dance declarations
enum {
    TD_SFT_NUM,
    TD_FN_MOUSE,
    TD_SPC_SYM,
    TD_GUI_CTRL,
};

// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Function associated with all tap dances
td_state_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void td_sft_num_finished(qk_tap_dance_state_t *state, void *user_data);
void td_sft_num_reset(qk_tap_dance_state_t *state, void *user_data);
void td_fn_mouse_finished(qk_tap_dance_state_t *state, void *user_data);
void td_fn_mouse_reset(qk_tap_dance_state_t *state, void *user_data);
void td_spc_sym_finished(qk_tap_dance_state_t *state, void *user_data);
void td_spc_sym_reset(qk_tap_dance_state_t *state, void *user_data);
void td_gui_ctrl_finished(qk_tap_dance_state_t *state, void *user_data);
void td_gui_ctrl_reset(qk_tap_dance_state_t *state, void *user_data);


// Determine the current tap dance state
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    // Return single tap or hold
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    // Return double tap or hold
    } else if (state->count == 2) {
        if (!state->pressed) return TD_DOUBLE_TAP;
        else return TD_DOUBLE_HOLD;
    // Return triple tap or hold
    } else if (state->count == 3) {
        if (!state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    }
    else return TD_UNKNOWN;
}



/******* FOR TD_SFT_NUM *******/
// Initialize tap structure
static td_tap_t td_sft_num_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// One Tap/Hold = SHIFT, Two Taps = MO(_NUM)
void td_sft_num_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_sft_num_state.state = cur_dance(state);
    switch (td_sft_num_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_LSFT);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LSFT);
            break;
        case TD_DOUBLE_HOLD:
            layer_on(_NUM);
            break;
        default:
            break;
    }
}

// Deactivate held keys
void td_sft_num_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_sft_num_state.state) {
        case TD_SINGLE_HOLD:
            unregister_code(KC_LSFT);
            break;
        case TD_DOUBLE_HOLD:
            layer_off(_NUM);
            break;
        default:
            break;
    }

    // Reset stored tap dance state
    td_sft_num_state.state = TD_NONE;
}
/******* END FOR TD_SFT_NUM *******/

/******* FOR TD_FN_MOUSE *******/
// Initialize tap structure
static td_tap_t td_fn_mouse_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// One Tap/Hold = MO(_FN), Two Taps = MO(_MOUSE)
void td_fn_mouse_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_fn_mouse_state.state = cur_dance(state);
    switch (td_fn_mouse_state.state) {
        case TD_SINGLE_HOLD:
            layer_on(_FN);
            break;
        case TD_DOUBLE_HOLD:
            layer_on(_MOUSE);
            break;
        default:
            break;
    }
}

void td_fn_mouse_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_fn_mouse_state.state) {
        case TD_SINGLE_HOLD:
            layer_off(_FN);
            break;
        case TD_DOUBLE_HOLD:
            layer_off(_MOUSE);
            break;
        default:
            break;
    }

    // Reset stored tap dance state
    td_fn_mouse_state.state = TD_NONE;
}
/******* END FOR TD_FN_MOUSE *******/

/******* FOR TD_SPC_SYM *******/
// Initialize tap structure
static td_tap_t td_spc_sym_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Choose between activation space key and Symbol layer
void td_spc_sym_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_spc_sym_state.state = cur_dance(state);
    switch (td_spc_sym_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_SPC);
            break;
        case TD_SINGLE_HOLD:
            tap_code(KC_SPC); // TripleHold: comment
            // layer_on(_SYM);
            break;
        case TD_DOUBLE_HOLD:
            // tap_code(KC_SPC);
            layer_on(_SYM);
            break;
        case TD_TRIPLE_HOLD: // TripleHold: comment block
            tap_code(KC_SPC);
            layer_on(_SYM);
            break;
        default:
            break;
    }
}

// Deactivate held keys
void td_spc_sym_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_spc_sym_state.state) {
        case TD_SINGLE_HOLD:
            unregister_code(KC_SPC); // TripleHold: comment
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_HOLD: // TripleHold: comment
            layer_off(_SYM);
            break;
        default:
            break;
    }

    // Reset stored tap dance state
    td_spc_sym_state.state = TD_NONE;
}
/******* END FOR TD_SPC_SYM *******/

/******* FOR TD_GUI_CTRL *******/
static td_tap_t td_gui_ctrl_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Choose GUI or CTRL
void td_gui_ctrl_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_gui_ctrl_state.state = cur_dance(state);
    switch (td_gui_ctrl_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_LGUI);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LGUI);
            break;
        case TD_DOUBLE_HOLD:
            register_code(KC_LCTL);
            break;
        default:
            break;
    }
}

// Deactivate held keys
void td_gui_ctrl_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_gui_ctrl_state.state) {
        case TD_SINGLE_HOLD:
            unregister_code(KC_LGUI);
            break;
        case TD_DOUBLE_HOLD:
            unregister_code(KC_LCTL);
            break;
        default:
            break;
    }

    // Reset stored tap dance state
    td_gui_ctrl_state.state = TD_NONE;
}
/******* END FOR TD_GUI_CTRL *******/


// TAPPING_TERM_PER_KEY specifications
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Increase time to register double tap
        case TD(TD_SFT_NUM):
        case TD(TD_FN_MOUSE):
            return 490;
        default:
            return TAPPING_TERM;
    }
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for GUI, twice for activating _NUM layer
    [TD_SFT_NUM] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_sft_num_finished, td_sft_num_reset),
    [TD_FN_MOUSE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fn_mouse_finished, td_fn_mouse_reset),
    [TD_SPC_SYM] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_spc_sym_finished, td_spc_sym_reset),
    [TD_GUI_CTRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_gui_ctrl_finished, td_gui_ctrl_reset),
};



// Layout definition
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


/* Layer : Colemak Mod-DH (Matrix)
-----------------------------------------------------------------------------------------------------------------------------------
    TAB     Q   W   F   P   B               J   L   U   Y   ;   (QWERTY)
    ------+---+---+---+++++---|           |---+++++---+---+---+------------------------------------ DF_DFDDFAd
     `      A   R   S   T   G               M   N   E   I   O   '
    ------+---+---+---+++++---|           |---+++++---+---+---+------------------------------------  TEST_ESER
    LCTRL   Z   X   C   D   V               K   H   ,   .   /   ESC
    --------------------------|           |------------------------------------------------------------ arst = 
        MO(Fkey)  GUI  LSFT/MO(Num)    ENT  SPC/MO(Sym)  BSPC
-------------------------------------------------------------------------------------------------------------------------------------*/

  [_COLEMAK_DH] = LAYOUT_split_3x6_3(
  //,---------------------------------------------------------------.                          ,---------------------------------------------------------------.
      KC_GRV,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                                 KC_J,   KC_L,    KC_U,    KC_Y,   KC_SCLN,  DF(_QWERTY),
  //|--------+--------+--------+--------+--------+------------------|                          |-----------------+--------+--------+--------+--------+---------|
      KC_TAB,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                                 KC_M,   KC_N,    KC_E,    KC_I,   KC_O,     KC_QUOT,
  //|--------+--------+--------+--------+--------+------------------|                          |-----------------+--------+--------+--------+--------+---------|
      KC_LCTL,   KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                                                 KC_K,   KC_H,   KC_COMM,  KC_DOT, KC_SLSH, KC_ESC,
  //|--------+--------+--------+--------+--------+--------+------------------|  |--------------+-----------------+--------+--------+--------+--------+---------|
                                         TD(TD_FN_MOUSE), TD(TD_SFT_NUM), TD(TD_GUI_CTRL)  ,   LT(_FN, KC_ENT)  , TD(TD_SPC_SYM) ,  KC_BSPC
                                      //`------------------------------------'  `-----------------------------------'

  ),

/* Layer : QWERTY
-----------------------------------------------------------------------------------------------------------------------------------
    TAB     Q   W   E   R   T               Y   U   I   O   P   (COLEMAK)
    ------+---+---+---+++++---|           |---+++++---+---+---+------------------------------------
     `      A   S   D   F   G               H   J   K   L   ;   '
    ------+---+---+---+++++---|           |---+++++---+---+---+------------------------------------
    LCTRL   Z   X   C   V   B               N   M   ,   .   /   ESC
    --------------------------|           |------------------------------------------------------------
        MO(Fkey)  GUI  LSFT/MO(Num)    ENT  SPC/MO(Sym)  BSPC 
-------------------------------------------------------------------------------------------------------------------------------------*/

  [_QWERTY] = LAYOUT_split_3x6_3(
  //,---------------------------------------------------------------.                          ,---------------------------------------------------------------.
      KC_GRV,    KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,                                                  KC_Y,   KC_U,    KC_I,    KC_O,   KC_P,    DF(_COLEMAK_DH),
  //|--------+--------+--------+--------+--------+------------------|                          |-----------------+--------+--------+--------+--------+---------|
      KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,   KC_G,                                                  KC_H,   KC_J,    KC_K,    KC_L,   KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+------------------|                          |-----------------+--------+--------+--------+--------+---------|
      KC_LCTL,   KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,                                                  KC_N,   KC_M,   KC_COMM,  KC_DOT, KC_SLSH, KC_ESC,
  //|--------+--------+--------+--------+--------+--------+------------------|  |--------------+-----------------+--------+--------+--------+--------+---------|
                                        TD(TD_FN_MOUSE),  TD(TD_SFT_NUM),  TD(TD_GUI_CTRL),      LT(_FN, KC_ENT), TD(TD_SPC_SYM), KC_BSPC
                                      //`------------------------------------'  `-----------------------------------'

  ),

/* Layer : Number, Arrow Keys
-----------------------------------------------------------------------------------------------------------------------------------
             1      2     3    4    5               6   7   8   9   0   BSPC
    ------+------+-----+----+++++++----|          |---+++++---+---+---+------------------------------------
    LALT    LEFT  DOWN   UP  RIGHT                  -   4   5   6   +   =
    ------+------+-----+----+++++++----|          |---+++++---+---+---+------------------------------------
    LSFT                                                1   2   3   /   ESC
    -----------------------------------|          |------------------------------------------------------------
                            _MO(1)_   LSFT     ENT   0   .
-------------------------------------------------------------------------------------------------------------------------------------*/

  [_NUM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,   KC_0,   KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, KC_SPC, KC_LPRN,  KC_RPRN, XXXXXXX,                      KC_MINS,  KC_4,    KC_5,    KC_6,   KC_PLUS, KC_EQL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, XXXXXXX,                      KC_COMM,  KC_1,    KC_2,    KC_3,   KC_SLSH, KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, _______, _______,    KC_DOT,   KC_0,   KC_BSPC
                                      //`--------------------------'  `--------------------------'
  ),

/* Layer : Symbols
-----------------------------------------------------------------------------------------------------------------------------------
                $   {   }   ^              #   *   |   &        BSPC
    ------+---+---+---+++++---|          |---+++++---+---+---+------------------------------------
    `      !   @   (   )                   -   =   +   %   ;    ~  
    ------+---+---+---+++++---|          |---+++++---+---+---+------------------------------------
    LSFT            [   ]                  \   _   <   >   /    
    --------------------------|          |------------------------------------------------------------
                   ENT  LSFT   LSFT          _MO(Sym)_
-------------------------------------------------------------------------------------------------------------------------------------*/

  [_SYM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                     ,-----------------------------------------------------.
     KC_TILD, XXXXXXX,  KC_DLR, KC_LCBR,  KC_RCBR,  KC_CIRC,                       KC_HASH, KC_ASTR, KC_PIPE, KC_AMPR, XXXXXXX,  KC_BSPC,
  //|--------+--------+--------+--------+---------+--------|                    |--------+--------+--------+--------+--------+--------|
     KC_GRV,  KC_EXLM,  KC_AT,  KC_LPRN,  KC_RPRN,  XXXXXXX,                       KC_MINS,  KC_EQL, KC_PLUS, KC_PERC, KC_SCLN,  KC_QUOT,
  //|--------+--------+--------+--------+---------+--------|                    |--------+--------+--------+--------+--------+--------|
     KC_LSFT, XXXXXXX, XXXXXXX, KC_LBRC,  KC_RBRC, XXXXXXX,                      KC_BSLS, KC_UNDS, KC_LT, KC_GT, KC_SLSH,  XXXXXXX,
  //|--------+--------+--------+--------+---------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LSFT, KC_SPC ,  KC_ENT,     XXXXXXX, _______, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),


/* Layer : F keys and media controls, other
-----------------------------------------------------------------------------------------------------------------------------------
           F1   F2   F3   F4   F5                  Mute   VOL_DOWN   VOL_UP    CTRL+ALT+DEL    
    -----+----+----+----++++++----|          |----+++++++----------+---------+--------------+------------------------------------
           F6   F7   F8   F9   F10                 LEFT     DOWN       UP         RIGHT           
    -----+----+----+----++++++----|          |----+++++++----------+---------+---------------+------------------------------------
          F11  F12                                 HOME     END       DEL   
    ------------------------------|          |------------------------------------------------------------
                _MO(Fkey)_               MO(Qmk)  
-------------------------------------------------------------------------------------------------------------------------------------*/

  [_FN] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,----------------------------------------------------------.
      XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU,  XXXXXXX, C(A(KC_DEL)),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+---------+------------|
      XXXXXXX, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                      XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,    KC_RIGHT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+---------+------------|
      KC_LSFT, KC_F11,  KC_F12, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_HOME, KC_END,  KC_DEL,   XXXXXXX,  XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+---------+------------|
                                         MO(_QK), XXXXXXX, _______,     MO(_QK), XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

/* Layer : Mouse and navigation
-----------------------------------------------------------------------------------------------------------------------------------
           
    -----+----+----+----++++++----|          |----+++++++----------+---------+--------------+------------------------------------
                            
    -----+----+----+----++++++----|          |----+++++++----------+---------+---------------+------------------------------------
                                                           
    ------------------------------|          |------------------------------------------------------------
                _MO(Mouse)_               MO(Qmk)  
-------------------------------------------------------------------------------------------------------------------------------------*/

  [_MOUSE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,------------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+---------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+---------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_BTN1, KC_BTN2, KC_BTN3, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+---------+--------|
                                         _______, XXXXXXX, XXXXXXX,     MO(_QK), KC_BTN1, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),


/* Layer : QMK and fun features?
-----------------------------------------------------------------------------------------------------------------------------------
     QK_BOOT                                                      QK_BOOT
    ---------+---+---+---+---+---|          |---+---+---+---+---+------------------------------------
                                     
    ------+---+---+---+---+---|          |---+---+---+---+---+-----------------------------------
                                        
    --------------------------|          |------------------------------------------------------------
                
-------------------------------------------------------------------------------------------------------------------------------------*/

  [_QK] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                         _______, XXXXXXX, XXXXXXX,     _______, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  } else {
    return OLED_ROTATION_270;
  }
  return rotation;
}

// #define L_BASE 0
// #define L_LOWER 2
// #define L_RAISE 4
// #define L_ADJUST 8

/**
 * Render the highest active layer in OLED screen
 */
void oled_render_layer_state(void) {
    oled_write_ln_P(PSTR("Layer"), false);
    switch (get_highest_layer(layer_state|default_layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("QWERT"), false);
            break;
        case _COLEMAK_DH:
            oled_write_ln_P(PSTR("CLMK"), false);
            break;
        case _NUM:
            oled_write_ln_P(PSTR("Num"), false);
            break;
        case _SYM:
            oled_write_ln_P(PSTR("Sym"), false);
            break;
        case _FN:
            oled_write_ln_P(PSTR("Fn"), false);
            break;
        case _MOUSE:
            oled_write_ln_P(PSTR("M.Nav"), false);
            break;
        case _QK:
            oled_write_ln_P(PSTR("QK"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
    } else {
        oled_render_logo();
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
  }
  return true;
}
#endif // OLED_ENABLE
