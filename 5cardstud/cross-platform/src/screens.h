#ifndef SCREENS_H
#define SCREENS_H

#ifndef _CMOC_VERSION_
#include <stdint.h>
#endif

/// @brief Convenience function to reset screen and draw border
void resetScreenWithBorder();

/// @brief Shows information about the game
void showHelpScreen();

/// @brief Action called in Welcome Screen to check if a server name is stored in an app key
void welcomeActionVerifyServerDetails();

/// @brief Action called in Welcome Screen to verify player has a name
void welcomeActionVerifyPlayerName();

/// @brief Shows the Welcome Screen with Logo. Asks player's name
void showWelcomScreen();

/// @brief Action in Table Selection Screen, joins the selected server
void tableActionJoinServer();

/// @brief Shows a screen to select a table to join
void showTableSelectionScreen();

/// @brief Shows main game play screen (table and cards)
void showGameScreen();

/// @brief shows in-game menu
void showInGameMenuScreen();

/// @brief Allow the player to modify their name
void showPlayerNameScreen();

/// @brief Handles available key strokes for the defined input box (player name and chat). Returns true if user hits enter

bool inputFieldCycle(uint8_t x, uint8_t y, uint8_t max, uint8_t* buffer);

#endif /*SCREENS_H*/
