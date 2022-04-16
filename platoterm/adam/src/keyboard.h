/**
 * PLATOTERM for Coleco Adam
 *
 * @author Thomas Cherryhomes
 * @author Steve Peltz (protocol.c)
 * @license gpl v. 3, see COPYING for Details
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * Initialize Keyboard
 */
void keyboard_init(void);

/**
 * A simple key press feedback.
 */
void keyboard_click(void);

/**
 * keyboard_out - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void keyboard_out(unsigned char platoKey);

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void);

/**
 * keyboard_out_tty - keyboard output to serial I/O in TTY mode
 */
void keyboard_out_tty(char ch);

/**
 * keyboard_done - Shut down keyboard
 */
void keyboard_done(void);

#endif
