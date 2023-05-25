/**
 * key.h
 * Keyboard Translation Tables
 */

#include "../plato_key.h"

#ifndef KEY_H
#define KEY_H

#define MODIFIER_NONE        0x00
#define MODIFIER_SHIFT       0x01
#define MODIFIER_COMMO       0x02
#define MODIFIER_COMMO_SHIFT 0x03
#define MODIFIER_CTRL        0x04
#define MODIFIER_CTRL_SHIFT  0x05

/* 64 entries for each possible primary scancode */
/* each of these map to indexes in PTAT          */
/* Thereby mapping each C= key to a PLATO key.   */
/* anything above 0x7f is a special key, that    */
/* causes an ACCESS key to be sent, followed by  */
/* the processed key value in ACCESS_KEYS        */

static const uint8_t KEYBOARD_TO_PLATO[] = {
  PKEY_ERASE, /* INS/DEL */
  PKEY_NEXT, /* RETURN */
  PKEY_DIVIDE, /* CRSR <-> */
  PKEY_NOKEY, /* F7/F8 */
  PKEY_NOKEY, /* F1/F2 */
  PKEY_NOKEY, /* F3/F4 */
  PKEY_NOKEY, /* F5/F6 */
  PKEY_MULTIPLY, /* CRSR ^V */
  PKEY_3, /* 3 */
  PKEY_w, /* W */
  PKEY_a, /* A */
  PKEY_4, /* 4 */
  PKEY_z, /* Z */
  PKEY_s, /* S */
  PKEY_e, /* E */
  PKEY_NOKEY, /* UNUSED */
  PKEY_5, /* 5 */
  PKEY_r, /* R */
  PKEY_d, /* D */
  PKEY_6, /* 6 */
  PKEY_c, /* C */
  PKEY_f, /* F */
  PKEY_t, /* T */
  PKEY_x, /* X */
  PKEY_7, /* 7 */
  PKEY_y, /* Y */
  PKEY_g, /* G */
  PKEY_8, /* 8 */
  PKEY_b, /* B */
  PKEY_h, /* H */
  PKEY_u, /* U */
  PKEY_v, /* V */
  PKEY_9, /* 9 */
  PKEY_i, /* I */
  PKEY_j, /* J */
  PKEY_0, /* 0 */
  PKEY_m, /* M */
  PKEY_k, /* K */
  PKEY_o, /* O */
  PKEY_n, /* N */
  PKEY_PLUS, /* + */
  PKEY_p, /* P */
  PKEY_l, /* L */
  PKEY_MINUS, /* - */
  PKEY_PERIOD, /* . */
  PKEY_COLON, /* : */
  PKEY_AT, /* @ */ /* ACCESS */
  PKEY_COMMA, /* , */
  PKEY_NOKEY, /* STERLING */
  PKEY_ASTERISK, /* ASTERISK */
  PKEY_SEMICOLON, /* ; */
  PKEY_NOKEY, /* CLR/HOME */
  PKEY_NOKEY, /* UNUSED */
  PKEY_EQUALS, /* = */
  PKEY_TAB, /* UP ARROW */
  PKEY_SLASH, /* / */
  PKEY_1, /* 1 */
  PKEY_ASSIGN, /* LEFT ARROW */
  PKEY_NOKEY, /* UNUSED */
  PKEY_2, /* 2 */
  PKEY_SPACE, /* SPACE */
  PKEY_NOKEY, /* UNUSED */
  PKEY_q, /* Q */
  PKEY_STOP, /* RUN/STOP */
  PKEY_NOKEY, /* EMPTY KEY */
};

static const uint8_t KEYBOARD_TO_PLATO_SHIFT[] = {
  PKEY_ERASE1, /* INS/DEL */
  PKEY_NEXT1, /* RETURN */
  PKEY_INTERSECT, /* CRSR <-> */
  PKEY_NOKEY, /* F7/F8 */
  PKEY_NOKEY, /* F1/F2 */
  PKEY_NOKEY, /* F3/F4 */
  PKEY_NOKEY, /* F5/F6 */
  PKEY_UNION, /* CRSR ^V */
  PKEY_POUND, /* 3 */
  PKEY_W, /* W */
  PKEY_A, /* A */
  PKEY_DOLLAR, /* 4 */
  PKEY_Z, /* Z */
  PKEY_S, /* S */
  PKEY_E, /* E */
  PKEY_NOKEY, /* UNUSED */
  PKEY_PERCENT, /* 5 */
  PKEY_R, /* R */
  PKEY_D, /* D */
  PKEY_AMPERSAND, /* 6 */
  PKEY_C, /* C */
  PKEY_F, /* F */
  PKEY_T, /* T */
  PKEY_X, /* X */
  PKEY_APOSTROPHE, /* 7 */
  PKEY_Y, /* Y */
  PKEY_G, /* G */
  PKEY_PARENTHESIS_LEFT, /* 8 */
  PKEY_B, /* B */
  PKEY_H, /* H */
  PKEY_U, /* U */
  PKEY_V, /* V */
  PKEY_PARENTHESIS_RIGHT, /* 9 */
  PKEY_I, /* I */
  PKEY_J, /* J */
  PKEY_NOKEY, /* 0 */
  PKEY_M, /* M */
  PKEY_K, /* K */
  PKEY_O, /* O */
  PKEY_N, /* N */
  PKEY_MULTIPLY, /* + */ /* possible multiply */
  PKEY_P, /* P */
  PKEY_L, /* L */
  PKEY_DIVIDE, /* - */ /* possible divide */
  PKEY_GREATER_THAN, /* . */
  PKEY_BRACKET_LEFT, /* : */
  PKEY_AT, /* @ */
  PKEY_LESS_THAN, /* , */
  PKEY_NOKEY, /* STERLING */
  PKEY_ASTERISK, /* ASTERISK */
  PKEY_BRACKET_RIGHT, /* ] */
  PKEY_NOKEY, /* CLR/HOME */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* = */
  PKEY_PI, /* UP ARROW */ /* SHIFT access to PI */
  PKEY_QUESTION_MARK, /* / */
  PKEY_EXCLAMATION, /* 1 */
  PKEY_NOKEY, /* LEFT ARROW */
  PKEY_NOKEY, /* UNUSED */
  PKEY_QUOTE, /* 2 */
  PKEY_BACKSPACE, /* SPACE */
  PKEY_NOKEY, /* UNUSED */
  PKEY_Q, /* Q */
  PKEY_STOP1, /* RUN/STOP */
  PKEY_NOKEY, /* EMPTY KEY */
};

static const uint8_t KEYBOARD_TO_PLATO_COMMO[] = {
  PKEY_NOKEY, /* INS/DEL */
  PKEY_NOKEY, /* RETURN */
  PKEY_NOKEY, /* CRSR <-> */
  PKEY_NOKEY, /* F7/F8 */
  PKEY_NOKEY, /* F1/F2 */
  PKEY_NOKEY, /* F3/F4 */
  PKEY_NOKEY, /* F5/F6 */
  PKEY_NOKEY, /* CRSR ^V */
  PKEY_NOKEY, /* 3 */
  PKEY_NOKEY, /* W */
  PKEY_ANS, /* A */
  PKEY_NOKEY, /* 4 */
  PKEY_NOKEY, /* Z */
  PKEY_STOP, /* S */
  PKEY_EDIT, /* E */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* 5 */
  PKEY_ERASE, /* R */
  PKEY_DATA, /* D */
  PKEY_NOKEY, /* 6 */
  PKEY_COPY, /* C */
  PKEY_FONT, /* F */
  PKEY_TERM, /* T */
  PKEY_MULTIPLY, /* X */
  PKEY_NOKEY, /* 7 */
  PKEY_SUB, /* Y */
  PKEY_DIVIDE, /* G */
  PKEY_NOKEY, /* 8 */
  PKEY_BACK, /* B */
  PKEY_HELP, /* H */
  PKEY_NOKEY, /* U */
  PKEY_NOKEY, /* V */
  PKEY_NOKEY, /* 9 */
  PKEY_NOKEY, /* I */
  PKEY_NOKEY, /* J */
  PKEY_NOKEY, /* 0 */
  PKEY_MICRO, /* M */
  PKEY_NOKEY, /* K */
  PKEY_NOKEY, /* O */
  PKEY_NEXT, /* N */
  PKEY_SIGMA, /* + */ /* possible multiply */
  PKEY_SUPER, /* P */
  PKEY_LAB, /* L */
  PKEY_DELTA, /* - */ /* possible divide */
  PKEY_NOKEY, /* . */
  PKEY_NOKEY, /* : */
  PKEY_AT, /* @ */
  PKEY_NOKEY, /* , */
  PKEY_NOKEY, /* STERLING */
  PKEY_NOKEY, /* ASTERISK */
  PKEY_NOKEY, /* ] */
  PKEY_NOKEY, /* CLR/HOME */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* = */
  PKEY_NOKEY, /* UP ARROW */
  PKEY_NOKEY, /* / */
  PKEY_NOKEY, /* 1 */
  PKEY_NOKEY, /* LEFT ARROW */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* 2 */
  PKEY_NOKEY, /* SPACE */
  PKEY_NOKEY, /* UNUSED */
  PKEY_SQUARE, /* Q */
  PKEY_STOP, /* RUN/STOP */
  PKEY_NOKEY, /* EMPTY KEY */
};

static const uint8_t KEYBOARD_TO_PLATO_CS[] = {
  PKEY_NOKEY, /* INS/DEL */
  PKEY_NOKEY, /* RETURN */
  PKEY_NOKEY, /* CRSR <-> */
  PKEY_NOKEY, /* F7/F8 */
  PKEY_NOKEY, /* F1/F2 */
  PKEY_NOKEY, /* F3/F4 */
  PKEY_NOKEY, /* F5/F6 */
  PKEY_NOKEY, /* CRSR ^V */
  PKEY_NOKEY, /* 3 */
  PKEY_NOKEY, /* W */
  PKEY_TERM, /* A */
  PKEY_NOKEY, /* 4 */
  PKEY_NOKEY, /* Z */
  PKEY_STOP1, /* S */
  PKEY_EDIT1, /* E */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* 5 */
  PKEY_ERASE1, /* R */
  PKEY_DATA1, /* D */
  PKEY_NOKEY, /* 6 */
  PKEY_COPY1, /* C */
  PKEY_FONT, /* F */
  PKEY_TERM, /* T */
  PKEY_MULTIPLY, /* X */
  PKEY_NOKEY, /* 7 */
  PKEY_SUB1, /* Y */
  PKEY_INTERSECT, /* G */
  PKEY_NOKEY, /* 8 */
  PKEY_BACK1, /* B */
  PKEY_HELP1, /* H */
  PKEY_NOKEY, /* U */
  PKEY_NOKEY, /* V */
  PKEY_NOKEY, /* 9 */
  PKEY_NOKEY, /* I */
  PKEY_NOKEY, /* J */
  PKEY_NOKEY, /* 0 */
  PKEY_FONT, /* M */
  PKEY_NOKEY, /* K */
  PKEY_NOKEY, /* O */
  PKEY_NEXT1, /* N */
  PKEY_SIGMA, /* + */ /* possible multiply */
  PKEY_SUPER1, /* P */
  PKEY_LAB1, /* L */
  PKEY_DELTA, /* - */ /* possible divide */
  PKEY_NOKEY, /* . */
  PKEY_NOKEY, /* : */
  PKEY_AT, /* @ */
  PKEY_NOKEY, /* , */
  PKEY_NOKEY, /* STERLING */
  PKEY_NOKEY, /* ASTERISK */
  PKEY_NOKEY, /* ] */
  PKEY_NOKEY, /* CLR/HOME */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* = */
  PKEY_NOKEY, /* UP ARROW */
  PKEY_NOKEY, /* / */
  PKEY_NOKEY, /* 1 */
  PKEY_NOKEY, /* LEFT ARROW */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* 2 */
  PKEY_NOKEY, /* SPACE */
  PKEY_NOKEY, /* UNUSED */
  PKEY_ACCESS, /* Q */
  PKEY_STOP1, /* RUN/STOP */
  PKEY_NOKEY, /* EMPTY KEY */
};

static const uint8_t KEYBOARD_TO_PLATO_CTRL[] = {
  PKEY_NOKEY, /* INS/DEL */
  PKEY_NOKEY, /* RETURN */
  PKEY_NOKEY, /* CRSR <-> */
  PKEY_NOKEY, /* F7/F8 */
  PKEY_NOKEY, /* F1/F2 */
  PKEY_NOKEY, /* F3/F4 */
  PKEY_NOKEY, /* F5/F6 */
  PKEY_NOKEY, /* CRSR ^V */
  PKEY_NOKEY, /* 3 */
  PKEY_CAPITAL_PI, /* W */
  PKEY_ALPHA, /* A */
  PKEY_NOKEY, /* 4 */
  PKEY_NOKEY, /* Z */
  PKEY_LOWERCASE_SIGMA, /* S */
  PKEY_LOWERCASE_AE, /* E */
  PKEY_NOKEY, /* UNUSED */
  PKEY_5, /* 5 */
  PKEY_RHO, /* R */
  PKEY_ACUTE_ACCENT, /* D */
  PKEY_6, /* 6 */
  PKEY_CEDILLA, /* C */
  PKEY_NOKEY, /* F */
  PKEY_THETA, /* T */
  PKEY_CIRCUMFLEX, /* X */
  PKEY_NOKEY, /* 7 */
  PKEY_LOWERCASE_O_WITH_DIARESIS, /* Y */
  PKEY_LOWERCASE_AE, /* G */
  PKEY_NOKEY, /* 8 */
  PKEY_BETA, /* B */
  PKEY_LOWERCASE_OE, /* H */
  PKEY_DIARESIS, /* U */
  PKEY_HACEK, /* V */
  PKEY_NOKEY, /* 9 */
  PKEY_NOKEY, /* I */
  PKEY_LOWERCASE_A_WITH_RING, /* J */
  PKEY_LEFT_EMBED, /* 0 */
  PKEY_MU, /* M */
  PKEY_LOWERCASE_A_WITH_DIAERESIS, /* K */
  PKEY_DEGREE, /* O */
  PKEY_TILDE, /* N */
  PKEY_AMPERSAND, /* + */ /* possible multiply */
  PKEY_PI, /* P */
  PKEY_LAMBDA, /* L */
  PKEY_NOKEY, /* - */ /* possible divide */
  PKEY_NOKEY, /* . */
  PKEY_LOWER_TILDE, /* : */
  PKEY_AT, /* @ */
  PKEY_DELIMITER, /* , */
  PKEY_NOKEY, /* STERLING */
  PKEY_NOKEY, /* ASTERISK */
  PKEY_NOKEY, /* ] */
  PKEY_NOKEY, /* CLR/HOME */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOT_EQUAL, /* = */
  PKEY_NOKEY, /* UP ARROW */
  PKEY_BACKSLASH, /* / */
  PKEY_RIGHT_EMBED, /* 1 */
  PKEY_LEFT_EMBED, /* LEFT ARROW */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* 2 */
  PKEY_NOKEY, /* SPACE */
  PKEY_NOKEY, /* UNUSED */
  PKEY_GRAVE, /* Q */
  PKEY_NOKEY, /* RUN/STOP */
  PKEY_NOKEY, /* EMPTY KEY */
};

static const uint8_t KEYBOARD_TO_PLATO_CTRL_SHIFT[] = {
  PKEY_NOKEY, /* INS/DEL */
  PKEY_NOKEY, /* RETURN */
  PKEY_NOKEY, /* CRSR <-> */
  PKEY_NOKEY, /* F7/F8 */
  PKEY_NOKEY, /* F1/F2 */
  PKEY_NOKEY, /* F3/F4 */
  PKEY_NOKEY, /* F5/F6 */
  PKEY_NOKEY, /* CRSR ^V */
  PKEY_NOKEY, /* 3 */
  PKEY_UP_ARROW, /* W */
  PKEY_LEFT_ARROW, /* A */
  PKEY_NOKEY, /* 4 */
  PKEY_NOKEY, /* Z */
  PKEY_NOKEY, /* S */
  PKEY_NOKEY, /* E */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* 5 */
  PKEY_NOKEY, /* R */
  PKEY_RIGHT_ARROW, /* D */
  PKEY_BIG_CROSS, /* 6 */
  PKEY_COPYRIGHT, /* C */
  PKEY_DIAMOND, /* F */
  PKEY_NOKEY, /* T */
  PKEY_DOWN_ARROW, /* X */
  PKEY_NOKEY, /* 7 */
  PKEY_UPPERCASE_O_WITH_DIARESIS, /* Y */
  PKEY_UPPERCASE_AE, /* G */
  PKEY_NOKEY, /* 8 */
  PKEY_NOKEY, /* B */
  PKEY_UPPERCASE_OE, /* H */
  PKEY_NOKEY, /* U */
  PKEY_NOKEY, /* V */
  PKEY_EQUIVALENT, /* 9 */
  PKEY_BAR, /* I */
  PKEY_UPPERCASE_A_WITH_RING, /* J */
  PKEY_NOKEY, /* 0 */
  PKEY_NOKEY, /* M */
  PKEY_UPPERCASE_A_WITH_DIAERESIS, /* K */
  PKEY_SQUARE, /* O */
  PKEY_NOKEY, /* N */
  PKEY_NOKEY, /* + */ /* possible multiply */
  PKEY_NOKEY, /* P */
  PKEY_NOKEY, /* L */
  PKEY_NOKEY, /* - */ /* possible divide */
  PKEY_GREATER_THAN_OR_EQUAL, /* . */
  PKEY_NOKEY, /* : */
  PKEY_NOKEY, /* @ */
  PKEY_LESS_THAN_OR_EQUAL, /* , */
  PKEY_NOKEY, /* STERLING */
  PKEY_NOKEY, /* ASTERISK */
  PKEY_NOKEY, /* ] */
  PKEY_NOKEY, /* CLR/HOME */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* = */
  PKEY_NOKEY, /* UP ARROW */
  PKEY_NOKEY, /* / */
  PKEY_NOKEY, /* 1 */
  PKEY_NOKEY, /* LEFT ARROW */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* 2 */
  PKEY_NOKEY, /* SPACE */
  PKEY_NOKEY, /* UNUSED */
  PKEY_NOKEY, /* Q */
  PKEY_NOKEY, /* RUN/STOP */
  PKEY_NOKEY, /* EMPTY KEY */
};

#endif /* KEY_H */
