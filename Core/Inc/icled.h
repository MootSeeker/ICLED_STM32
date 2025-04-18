/**
 * @file icled.h
 * @author MootSeeker
 * @brief ICLED driver for controlling GRB LED matrices using PWM and DMA.
 *
 * This module provides functions to set pixel colors, flush the LED buffer,
 * and create visual effects on a matrix of ICLEDs (e.g., Würth eiSos).
 * It relies on a hardware timer and DMA to generate correct PWM signal timing.
 *
 * @copyright (c) 2025 MootSeeker
 * @license MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the \"Software\"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ICLED_H
#define ICLED_H

/**
 * @brief Include the standard headers.
 */
#include <stdbool.h>
#include <stdint.h>

/**
 * @def ICLED_LED_COUNT
 * @brief Number of LEDs in the matrix.
 */
#define ICLED_LED_COUNT     105

/**
 * @def ICLED_RESET_SLOTS
 * @brief Number of idle PWM slots to trigger the LED latch (>50µs).
 */
#define ICLED_RESET_SLOTS   200

/**
 * @def ICLED_BITS_PER_LED
 * @brief Number of bits per LED (8 bits for each GRB component).
 */
#define ICLED_BITS_PER_LED  24

/**
 * @def ICLED_TIMING_BITS
 * @brief Total number of data bits for all LEDs.
 */
#define ICLED_TIMING_BITS   (ICLED_LED_COUNT * ICLED_BITS_PER_LED)

/**
 * @def ICLED_BUFFER_SIZE
 * @brief Total size of the PWM buffer including reset (latch) padding.
 */
#define ICLED_BUFFER_SIZE   (ICLED_TIMING_BITS + ICLED_RESET_SLOTS)

/**
 * @def ICLED_PWM_0
 * @brief PWM compare value for logical '0' (approx. 32% duty with ARR=39).
 */
#define ICLED_PWM_0         13

/**
 * @def ICLED_PWM_1
 * @brief PWM compare value for logical '1' (approx. 64% duty with ARR=39).
 */
#define ICLED_PWM_1         26

// PWM-Duty-Zeitwerte
#define ICLED_PWM_0         13     // entspricht ca. 32% bei ARR = 39
#define ICLED_PWM_1         26     // entspricht ca. 64% bei ARR = 39

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the LED driver and clears all pixels.
 */
void ICLED_Init(void);

/**
 * @brief Sets the color of a single LED by index.
 *
 * @param index The LED index (0 to ICLED_LED_COUNT - 1).
 * @param r     Red component (0–255).
 * @param g     Green component (0–255).
 * @param b     Blue component (0–255).
 */
void ICLED_SetPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Transfers the current LED buffer to the LEDs using DMA.
 *
 * Must be called after modifying pixel colors.
 */
void ICLED_Show(void);

/**
 * @brief Turns off all LEDs.
 */
void ICLED_Clear(void);

#ifdef __cplusplus
}
#endif


#endif /* End: ICLED_H */
