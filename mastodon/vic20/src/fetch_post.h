/**
 * Show latest Mastodon post from OLDBYTES.SPACE
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes at gmail dot com
 * @license  gpl v. 3
 */

#ifndef FETCH_H
#define FETCH_H

/**
 * @brief Fetch Mastodon Post
 * @param display_name The name to display
 * @param created_at The created at timestamp to display
 * @param content The content to display
 */
void fetch_post(char *display_name,
		char *created_at,
		char *content);

#endif /* FETCH_H */
