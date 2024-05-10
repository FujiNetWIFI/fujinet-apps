#ifndef SCREENS_H
#define SCREENS_H

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


#endif /*SCREENS_H*/