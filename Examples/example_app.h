/**
 * @file example_app.h
 * @brief Public interface for example LED effects using ICLED matrix.
 *
 * Contains function prototypes for various LED demo animations such as
 * Knight Rider, Starfield, and Snake effects.
 *
 * Created on: Apr 18, 2025
 * Author: MootSeeker
 */

#ifndef EXAMPLE_APP_H
#define EXAMPLE_APP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Runs the currently selected demo effect.
 *
 * This function should be called in the main loop to continuously render
 * the selected LED animation.
 */
void example_app_run(void);

/**
 * @brief Classic Knight Rider effect with red sweep and glow.
 *
 * @param brightness Brightness from 0 to 100 (%).
 * @param delay Delay between steps in milliseconds.
 */
void ICLED_NightRideDemo(uint8_t brightness, uint16_t delay);

/**
 * @brief Knight Rider effect with a fading color glow (red → orange → yellow).
 *
 * @param brightness Brightness from 0 to 100 (%).
 * @param delay Delay between steps in milliseconds.
 */
void ICLED_KnightRiderColorFade(uint8_t brightness, uint16_t delay);

/**
 * @brief Starfield animation with white and yellow stars blinking over cyan.
 *
 * @param brightness Global brightness level (0–255).
 * @param delay Delay between frames in milliseconds.
 */
void ICLED_StarfieldEffect(uint8_t brightness, uint16_t delay);

/**
 * @brief Animated snake movement across the matrix.
 *
 * The snake moves in different directions and changes its length over time.
 *
 * @param brightness Brightness of the snake's head.
 * @param delay Delay between steps in milliseconds.
 */
void ICLED_SnakePattern(uint8_t brightness, uint16_t delay);

#ifdef __cplusplus
}
#endif

#endif /* EXAMPLE_APP_H */
