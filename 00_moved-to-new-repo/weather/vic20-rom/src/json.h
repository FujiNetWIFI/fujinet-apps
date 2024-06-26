/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose json functions
 */

#ifndef JSON_H
#define JSON_H

void json_init(void);
void json_parse(void);
void json_query(char *s);
void json_done(void);

#endif /* JSON_H */
