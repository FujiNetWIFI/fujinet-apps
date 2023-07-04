/**
 * @brief card display routines
 */

#ifndef CARD_H
#define CARD_H

/**
 * @brief card numbers
 */
typedef enum _card
  {
    CARD_2,
    CARD_3,
    CARD_4,
    CARD_5,
    CARD_6,
    CARD_7,
    CARD_8,
    CARD_9,
    CARD_J,
    CARD_Q,
    CARD_K,
    CARD_A
  }
  Card;

/**
 * @brief card suits
 */
typedef enum _suit
  {
    SUIT_HEART,
    SUIT_DIAMOND,
    SUIT_CLUB,
    SUIT_SPADE
  }
  Suit;

/**
 * @brief display a single card at x,y
 * @param x Horizontal position (0-39)
 * @param y Vertical position (0-191)
 * @param c Card to display
 * @param s Suit to display
 */
void card(unsigned char x, unsigned char y, Card c, Suit);

#endif /* CARD_H */
