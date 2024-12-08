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

#include QMK_KEYBOARD_H
#include <stdio.h>
//#include "layers.h"
//#include "tap_dance.h"

enum layers {
    _CLMK,
    _NUM,
    _SYM,
    _CTRL,
    _MOUSE,
};


typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
} td_state_t;



typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Tap dance enums
enum {
    LSFT_CTRL,
};

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

void td_lsft_ctrl_finished(tap_dance_state_t *state, void *user_data);
void td_lsft_ctrl_reset(tap_dance_state_t *state, void *user_data);

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    // Return single tap or hold
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    // Return double tap or hold
    } else if (state->count == 2) {
        if (!state->pressed) return TD_DOUBLE_TAP;
        else return TD_DOUBLE_HOLD;
    }
    else return TD_UNKNOWN;
}

/// LSFT, CAPS_WORD, _CTRL ///
static td_tap_t td_lsft_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void td_lsft_ctrl_finished(tap_dance_state_t *state, void *user_data) {
    td_lsft_state.state = cur_dance(state);
    switch (td_lsft_state.state) {
        case TD_SINGLE_TAP:
            set_oneshot_mods(MOD_BIT(KC_LSFT));
            break;
        case TD_SINGLE_HOLD:
            layer_on(_CTRL);
            break;
        case TD_DOUBLE_TAP:
            caps_word_on();
            break;
        default:
            break;
    }
}

void td_lsft_ctrl_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_lsft_state.state) {
        case TD_SINGLE_TAP:
            clear_oneshot_mods();
            break;
        case TD_SINGLE_HOLD:
            layer_off(_CTRL);
            break;
        default:
            break;
    }

    // Reset stored tap dance state
    td_lsft_state.state = TD_NONE;
}


/// Tap Dance Definitions ///
tap_dance_action_t  tap_dance_actions[] = {
    [LSFT_CTRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_lsft_ctrl_finished, td_lsft_ctrl_reset),
};




const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


/**
 * Layer: Colemak Mod-DH (Matrix) - MacOS
 * -----------------------------------------------------------------
 *   Q     W     F     P     B           J     L     U     Y     '
 * -----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
 *   A     R     S     T     G           M     N     E     I     O
 * -----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
 *   Z     X     C     D     V           K     H     ,     .     ;
 * -----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
 *            (MOUSE) LSFT  GUI         ENT   SPC  BSPC
 *                   (CTRL)
 * -----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
 */

    [_CLMK] = LAYOUT_split_3x5_3(
        KC_Q, KC_W, KC_F, KC_P, KC_B,   KC_J, KC_L, KC_U, KC_Y, KC_QUOT,
        KC_A, KC_R, KC_S, KC_T, KC_G,   KC_M, KC_N, KC_E, KC_I, KC_O,
        KC_Z, KC_X, KC_C, KC_D, KC_V,   KC_K, KC_H, KC_COMM, KC_DOT, KC_SCLN,
        TG(_MOUSE), TD(LSFT_CTRL), KC_LGUI,   KC_ENT, KC_SPC, KC_BSPC

    ),

// /**
//  * Layer: Numbers
//  * ----------------------------------------------------------------------------
//  *    (      7      8      9      )             #      -      :      +      '
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *    [      4      5      6      ]                    <      =      >      /
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *    $      1      2      3                    {      }      ,      .      ;
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *                         0   (CLMK)          ENT    SPC   BSPC 
//  * ------+------+------+------+------+------+------+------+------+------+------
//  */
//     [_NUM] = LAYOUT_split_3x6_3(
//   //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//        KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//       KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//       KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                           KC_LGUI, _______,  KC_SPC,     KC_ENT,   MO(3), KC_RALT
//                                       //`--------------------------'  `--------------------------'
//   ),


// /**
//  * Layer: CTRL shortcuts
//  * ----------------------------------------------------------------------------
//  *   ESC                       CTRL-B                      CTRL-U         CTRL-ALT-DEL
//  * ------+------+------+------+------+------+------+------+------+------+------
//  * CTRL-A CTRL-R                                                  CTRL-I CTRL-O
//  * ------+------+------+------+------+------+------+------+------+------+------
//  * CTRL-Z CTRL-X CTRL-C CTRL-D CTRL-V
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *                                                   
//  * ----------------------------------------------------------------------------
//  */
//     [_CTRL] = LAYOUT_split_3x6_3(
//   //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//        KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//       KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//       KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
//   //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                           KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
//                                       //`--------------------------'  `--------------------------'
//   ),

// /**
//  * Layer: Symbols
//  * ----------------------------------------------------------------------------
//  *          &      *     (                             [      ]       |     `
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *    ~     $      %     ^                             {      }
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *          !      @     #                                    _      \     /
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *                        )    (CLMK)          ENT    SPC   BSPC
//  * ----------------------------------------------------------------------------


// /**
//  * Layer: Functions
//  * ----------------------------------------------------------------------------
//  *  *SS*    F7     F8     F9    F12                  MUTE   VOLD   VOLUP
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *          F4     F5     F6    F11                  LEFT   DOWN    UP   RIGHT
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *          F1     F2     F3    F10                  HOME    END 
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *                       LSFT  (CLMK)         Layer  OPT   BSPC
//  * ----------------------------------------------------------------------------
//  */
//     [3] = LAYOUT_split_3x6_3(
//   //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//       QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//       RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//       RM_NEXT, RM_HUED, RM_SATD, RM_VALD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                           KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
//                                       //`--------------------------'  `--------------------------'
//   )

// /**
//  * Layer: Mouse
//  * ----------------------------------------------------------------------------
//  *                                                   WH_L   WH_D   WH_U   WH_R
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *                                                   MS_L   MS_D   MS_U   MS_R
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *                                                  LCLICK RCLICK MCLICK
//  * ------+------+------+------+------+------+------+------+------+------+------
//  *                             (CLMK)
//  * ----------------------------------------------------------------------------
//  */

// /**
//  * Layer: Keyboard - QMK / RGB
//  * ----------------------------------------------------------------------------
//  * ------+------+------+------+------+------+------+------+------+------+------
//  * ------+------+------+------+------+------+------+------+------+------+------
//  * ------+------+------+------+------+------+------+------+------+------+------
//  * ----------------------------------------------------------------------------
//  */

// /**
//  * Layer:
//  * ----------------------------------------------------------------------------
//  * ------+------+------+------+------+------+------+------+------+------+------
//  * ------+------+------+------+------+------+------+------+------+------+------
//  * ------+------+------+------+------+------+------+------+------+------+------
//  * ----------------------------------------------------------------------------
//  */
};

// #ifdef ENCODER_MAP_ENABLE
// const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
//   [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
//   [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
//   [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
//   [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
// };
// #endif





