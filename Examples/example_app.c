/**
 * @file example_app.c
 * @brief Contains example LED animation effects for the ICLED matrix.
 *
 * Provides a set of demonstration effects including Knight Rider, Starfield,
 * and Snake animation for 7x15 ICLED matrices.
 *
 * Created on: Apr 18, 2025
 * Author: MootSeeker
 */

#include "example_app.h"
#include "icled.h"
#include "main.h"

#include <stdlib.h>

/**
 * @def Snake movement
 * @brief Snake movement in all direction.
 */
#define DIR_HORIZONTAL  0
#define DIR_VERTICAL    1
#define DIR_DIAGONAL_RD 2  // Rechts-Unten Diagonal ↘
#define DIR_DIAGONAL_LU 3  // Links-Oben Diagonal ↖
#define DIR_MODE_COUNT  4

/**
 * @def MAX_STARS
 * @brief Maximum number of concurrent visible stars in the starfield effect.
 */
#define MAX_STARS 10



/**
 * @enum ICLED_EffectMode
 * @brief Available demo animation modes for the LED matrix.
 */
typedef enum
{
    EFFECT_SIMPLE 	 = 0,
    EFFECT_GLOW   	 = 1,
	EFFECT_STARFIELD = 2,
	EFFECT_SNAKE 	 = 3,
    EFFECT_COUNT
} ICLED_EffectMode;

/**
 * @brief Holds the currently selected LED effect mode.
 *
 * This global variable is used in the main loop and changed via button interrupt.
 */
volatile ICLED_EffectMode effectMode = EFFECT_SIMPLE;

/**
 * @struct Star
 * @brief Represents a temporary star in the starfield animation.
 *
 * Each star has a position (LED index), color (white or yellow),
 * and a lifetime (`age`) that counts down each frame.
 */
typedef struct
{
    uint16_t index;
    uint8_t  age;
    uint8_t  color; // 0 = white, 1 = yellow
} Star;


/**
 * @brief External interrupt callback for button press.
 *
 * This function is called by the HAL when an external interrupt occurs
 * (e.g., GPIO pin falling edge). It handles switching between predefined
 * LED effects by incrementing the global @ref effectMode variable.
 *
 * When the end of the effect list is reached, it wraps back to 0.
 *
 * @param GPIO_Pin The pin number that triggered the interrupt.
 *
 * @note This function currently responds only to @c S2_Pin.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == S2_Pin) // S2 Button
    {
        effectMode++;
        if (effectMode >= EFFECT_COUNT)
        {
            effectMode = 0;
        }
    }
}

/**
 * @brief Classic Knight Rider effect with red sweep and glow on top row.
 *
 * This animation mimics the iconic Knight Rider LED scanner:
 * - A red pixel moves left to right and back across the top row.
 * - Adjacent pixels glow with decreasing intensity to create a fading trail.
 * - The effect reverses direction when the end is reached.
 *
 * The center pixel is the brightest, while the side pixels use scaled brightness
 * to simulate a glowing sweep.
 *
 * @param brightness Brightness of the main pixel in percent (0–100).
 * @param delay Delay between frames in milliseconds.
 *
 * @note The top row is assumed to be mapped as `col * 7 + 0` for a 7x15 matrix.
 *       Call this function continuously in your main loop to maintain the effect.
 */
void ICLED_NightRideDemo(uint8_t brightness, uint16_t delay)
{
    static int8_t direction = 1;
    static int8_t col = 0;

    for (uint8_t c = 0; c < 15; c++)
    {
        ICLED_SetPixel(c * 7 + 0, 0, 0, 0);
    }

    ICLED_SetPixel(col * 7 + 0, (brightness * 255) / 100, 0, 0);

    if (col > 0)
        ICLED_SetPixel((col - 1) * 7 + 0, (brightness * 100) / 100, 0, 0);
    if (col > 1)
        ICLED_SetPixel((col - 2) * 7 + 0, (brightness * 40) / 100, 0, 0);
    if (col < 14)
        ICLED_SetPixel((col + 1) * 7 + 0, (brightness * 100) / 100, 0, 0);
    if (col < 13)
        ICLED_SetPixel((col + 2) * 7 + 0, (brightness * 40) / 100, 0, 0);

    ICLED_Show();
    HAL_Delay(delay);

    col += direction;
    if (col >= 14)
    {
        col = 14;
        direction = -1;
    }
    else if (col <= 0)
    {
        col = 0;
        direction = 1;
    }
}

/**
 * @brief Knight Rider effect with smooth fading colors (red → orange → yellow).
 *
 * This variant of the classic Knight Rider animation displays a moving light
 * on the top row of the matrix with a symmetrical color fade:
 * - Center pixel is yellowish (high red + green).
 * - Surrounding pixels fade to orange, then dark red.
 *
 * The direction reverses at each end of the matrix, creating a back-and-forth motion.
 *
 * @param brightness Brightness level of the center pixel (0–255).
 *                  Other pixels fade relative to this value.
 * @param delay Delay in milliseconds between animation steps.
 *
 * @note Designed for a matrix where the top row is mapped as `col * 7 + 0`.
 *       Call this function in a loop to maintain the animation.
 */
void ICLED_KnightRiderColorFade(uint8_t brightness, uint16_t delay)
{
    static int8_t direction = 1;
    static int8_t col = 0;

    for (uint8_t c = 0; c < 15; c++)
    {
        ICLED_SetPixel(c * 7 + 0, 0, 0, 0);
    }

    if (col > 2)
        ICLED_SetPixel((col - 3) * 7 + 0, (brightness * 64) / 255, 0, 0);
    if (col < 12)
        ICLED_SetPixel((col + 3) * 7 + 0, (brightness * 64) / 255, 0, 0);
    if (col > 1)
        ICLED_SetPixel((col - 2) * 7 + 0, (brightness * 150) / 255, (brightness * 50) / 255, 0);
    if (col < 13)
        ICLED_SetPixel((col + 2) * 7 + 0, (brightness * 150) / 255, (brightness * 50) / 255, 0);
    if (col > 0)
        ICLED_SetPixel((col - 1) * 7 + 0, (brightness * 230) / 255, (brightness * 150) / 255, 0);
    if (col < 14)
        ICLED_SetPixel((col + 1) * 7 + 0, (brightness * 230) / 255, (brightness * 150) / 255, 0);

    ICLED_SetPixel(col * 7 + 0, (brightness * 200) / 255, brightness, 0);
    ICLED_Show();
    HAL_Delay(delay);

    col += direction;
    if (col >= 14)
    {
        col = 14;
        direction = -1;
    }
    else if (col <= 0)
    {
        col = 0;
        direction = 1;
    }
}

/**
 * @brief Starfield animation with blinking stars over a cyan background.
 *
 * This effect simulates a dynamic night sky by rendering randomly blinking
 * white and yellow stars on top of a cyan-colored background. Stars appear
 * at random positions and disappear after a few frames, creating a twinkling effect.
 *
 * Behavior:
 * - Stars appear randomly (~15% chance per frame).
 * - Each star has a randomized lifespan (4–8 frames).
 * - Stars fade out and reset to cyan when their lifetime ends.
 * - The effect uses a static array of stars to limit the number of concurrent blinks.
 *
 * @param brightness The intensity of the stars and background (0–255).
 * @param delay Delay in milliseconds between animation frames.
 *
 * @note Call this function repeatedly (e.g., from the main loop).
 */
void ICLED_StarfieldEffect(uint8_t brightness, uint16_t delay)
{
    static Star stars[MAX_STARS] = {0};
    static uint8_t init = 0;

    if (!init)
    {
        for (uint16_t i = 0; i < ICLED_LED_COUNT; ++i)
            ICLED_SetPixel(i, 0, brightness / 2, brightness);
        ICLED_Show();
        init = 1;
    }

    for (int i = 0; i < MAX_STARS; ++i)
    {
        if (stars[i].age == 0 && (rand() % 100) < 15)
        {
            stars[i].index = rand() % ICLED_LED_COUNT;
            stars[i].age = 4 + rand() % 5;
            stars[i].color = rand() % 2;
        }
    }

    for (int i = 0; i < MAX_STARS; ++i)
    {
        if (stars[i].age > 0)
        {
            if (stars[i].color == 0)
            {
                ICLED_SetPixel(stars[i].index, brightness, brightness, brightness);
            }
            else
            {
                ICLED_SetPixel(stars[i].index, brightness, brightness, 0);
            }

            stars[i].age--;
            if (stars[i].age == 0)
            {
                ICLED_SetPixel(stars[i].index, 0, brightness / 2, brightness);
            }
        }
    }

    ICLED_Show();
    HAL_Delay(delay);
}

/**
 * @brief Animated snake pattern across the LED matrix.
 *
 * This effect displays a moving snake-like trail across a 7x15 LED matrix. The snake:
 * - Moves in alternating directions: horizontally, vertically, and diagonally.
 * - Changes length dynamically over time.
 * - Fades from head (bright green) to tail (darker green).
 *
 * The direction mode cycles automatically every ~160 frames:
 * - DIR_HORIZONTAL: Left to right, row-wise Z-pattern.
 * - DIR_VERTICAL: Top to bottom, column-wise S-pattern.
 * - DIR_DIAGONAL_RD: Diagonal down-right ↘.
 * - DIR_DIAGONAL_LU: Diagonal up-left ↖.
 *
 * This function is intended to be called repeatedly (e.g., from a main loop).
 *
 * @param brightness Brightness of the snake's head (0–255).
 * @param delay Delay between animation frames in milliseconds.
 *
 * @note The snake automatically resets when reaching the end of the matrix.
 */
void ICLED_SnakePattern(uint8_t brightness, uint16_t delay)
{
    static uint16_t tick = 0;
    static uint16_t headIndex = 0;
    static uint8_t snakeLength = 8;
    static uint8_t growing = 1;
    static uint8_t mode = DIR_HORIZONTAL;

    const uint8_t rows = 7;
    const uint8_t cols = 15;
    const uint16_t total = rows * cols;

    for( uint16_t i = 0; i < total; i++ )
    {
        ICLED_SetPixel(i, 0, 0, 0);
    }

    for( uint8_t s = 0; s < snakeLength; s++ )
    {
        int16_t step = headIndex - s;
        if (step < 0 || step >= total) continue;

        uint8_t index = 0;
        switch (mode)
        {
            case DIR_HORIZONTAL:
                index = ((step % cols) * rows) + (step / cols);
                break;
            case DIR_VERTICAL:
                index = (step / rows) * rows + (step % rows);
                break;
            case DIR_DIAGONAL_RD:
                index = (step % cols) * rows + (step % rows);
                break;
            case DIR_DIAGONAL_LU:
                index = (cols - 1 - (step % cols)) * rows + (rows - 1 - (step % rows));
                break;
        }

        uint8_t level = brightness - (s * (brightness / snakeLength));
        ICLED_SetPixel(index, 0, level, 0);
    }

    ICLED_Show();
    HAL_Delay(delay);

    tick++;
    headIndex++;

    if (tick % 160 == 0)
    {
        headIndex = 0;
        mode = (mode + 1) % DIR_MODE_COUNT;
    }

    if (tick % 25 == 0)
    {
        if (growing)
        {
            snakeLength++;
            if (snakeLength > 15) growing = 0;
        }
        else
        {
            if (snakeLength > 4) snakeLength--;
            else growing = 1;
        }
    }

    if (headIndex > total + snakeLength)
    {
        headIndex = 0;
    }
}

/**
 * @brief Executes the currently selected LED effect.
 *
 * This function checks the global variable @ref effectMode and calls
 * the corresponding animation function (e.g., Knight Rider, Starfield, Snake).
 * It should be placed inside the main loop and is intended to run continuously.
 *
 * Effects are changed externally via button interrupt (see HAL_GPIO_EXTI_Callback).
 *
 * - EFFECT_SIMPLE: Red sweep effect.
 * - EFFECT_GLOW: Color-fading Knight Rider.
 * - EFFECT_STARFIELD: Random blinking stars.
 * - EFFECT_SNAKE: Dynamic snake animation across matrix.
 *
 * @return void
 */
void example_app_run(void)
{
    switch (effectMode)
    {
        case EFFECT_SIMPLE:
            ICLED_NightRideDemo(40, 80);
            break;
        case EFFECT_GLOW:
            ICLED_KnightRiderColorFade(40, 100);
            break;
        case EFFECT_STARFIELD:
            ICLED_StarfieldEffect(20, 100);
            break;
        case EFFECT_SNAKE:
            ICLED_SnakePattern(40, 60);
            break;
        default:
            ICLED_Clear();
            HAL_Delay(100);
            break;
    }
}
