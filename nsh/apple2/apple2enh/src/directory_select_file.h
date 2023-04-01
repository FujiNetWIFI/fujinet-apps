/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Directory Show
 */

#ifndef DIRECTORY_SELECT_FILE_H
#define DIRECTORY_SELECT_FILE_H

/**
 * @brief Reset directory view
 */
void directory_select_file_reset(void);

/**
 * @brief Navigate/select file
 * @param selected_page The currently visible page (e.g. for previous page [...])
 * @param num_entries The number of entries visible
 * @param is next_page visible?
 * @return the selected file in global variable 'url'
 */
void directory_select_file(unsigned char *selected_page, unsigned char num_entries, bool next_page);

#endif /* DIRECTORY_SELECT_FILE_H */
