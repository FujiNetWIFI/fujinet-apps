/**
 * Show latest Mastodon post from OLDBYTES.SPACE
 *
 * @author   Thomas Cherryhomes
 * @email    thom dot cherryhomes at gmail dot com
 * @license  gpl v. 3
 */

#ifndef SHOW_POST_H
#define SHOW_POST_H

/**
 * @brief Show Post
 * @param display_name The name to display (must be 40 chars, or less)
 * @param created_at The created at timestamp to display (must be 40 chars, or less)
 * @param content The content to display, must be 800 chars or less)
 * @param replies_count # of replies to thread
 * @param reblogs_count # of boosts
 * @param favorites_count # of hearts
 */
void show_post(const char *display_name,
	       const char *created_at,
	       const char *content,
	       int replies_count,
	       int reblogs_count,
	       int favorites_count);

#endif /* SHOW_POST_H */
