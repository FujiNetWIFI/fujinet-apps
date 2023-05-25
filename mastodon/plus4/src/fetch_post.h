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
 * @param replies_count # of replies to thread
 * @param reblogs_count # of boosts
 * @param favorites_count # of hearts
 */
void fetch_post(char *display_name,
		char *created_at,
		char *content,
		int *replies_count,
		int *reblogs_count,
		int *favorites_count);

#endif /* FETCH_H */
