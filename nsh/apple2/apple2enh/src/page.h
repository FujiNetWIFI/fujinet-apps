/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Page display functions
 */

#ifndef PAGE_H
#define PAGE_H

/**
 * @brief Re-usable text fragments
 */
extern const char *helpText;
extern const char *selectText;
extern const char *returnText;
extern const char *selectDeviceSubheader;

/**
 * @brief Help text
 */
extern const char *helpText;

/**
 * @brief display horizontal rule at specified line
 * @param y Desired horizontal line
 */
void page_line(unsigned char y);

/**
 * @brief Display standard page
 * @param header    Header text to display
 * @param subheader Subheader text to display
 * @param footer    Footer text to display
 */
void page(const char *header, const char *subheader, const char *footer);

#endif /* PAGE_H */
