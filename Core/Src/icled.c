/**
 * @file icled.c
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
 */

#include "icled.h"

#include "main.h"
#include "tim.h"

#include <string.h>

/**
 * @brief DMA PWM buffer that stores the bit-expanded signal for all LEDs.
 * Each bit is translated into a PWM compare value (ICLED_PWM_0 or ICLED_PWM_1),
 * followed by reset slots for latch timing.
 */
static uint16_t pwm_buffer[ICLED_BUFFER_SIZE];

/**
 * @brief GRB pixel data buffer for all LEDs.
 * Each entry holds 3 bytes: [0] = G, [1] = R, [2] = B.
 */
static uint8_t led_data[ICLED_LED_COUNT][3];

/**
 * @brief Initializes the ICLED module.
 *
 * This function clears the entire matrix and starts PWM DMA transfer.
 */
void ICLED_Init( void )
{
    ICLED_Clear( );
    HAL_TIM_PWM_Start_DMA( &htim1, TIM_CHANNEL_1, ( uint32_t* )pwm_buffer, ICLED_BUFFER_SIZE );
}

/**
 * @brief Sets the color of a single LED.
 *
 * @param index The LED index (0 to ICLED_LED_COUNT - 1).
 * @param r Red intensity (0–255).
 * @param g Green intensity (0–255).
 * @param b Blue intensity (0–255).
 *
 * The values are stored in GRB order.
 */
void ICLED_SetPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
	/* Check if index is in range */
    if (index >= ICLED_LED_COUNT) return;

    /* Parse data */
    led_data[index][0] = g; // GRB!
    led_data[index][1] = r;
    led_data[index][2] = b;
}

/**
 * @brief Clears all LEDs by setting their color to black.
 *
 * Also calls ICLED_Show() to immediately update the matrix.
 */
void ICLED_Clear( void )
{
    for( uint16_t i = 0; i < ICLED_LED_COUNT; i++ )
    {
        ICLED_SetPixel( i, 0, 0, 0 );
    }

    ICLED_Show( );
}

/**
 * @brief Updates the LED strip with current pixel values.
 *
 * This function converts the GRB data buffer into a PWM-compatible
 * bitstream, appends reset slots, and restarts the DMA PWM transfer.
 */
void ICLED_Show( void )
{
    uint32_t pos = 0;

    // Convert each byte (G, R, B) into 8 PWM bits (MSB first)
    for( uint16_t i = 0; i < ICLED_LED_COUNT; i++ )
    {
        for( uint8_t c = 0; c < 3; c++ )
        {
            uint8_t val = led_data[i][c];
            for( int8_t bit = 7; bit >= 0; bit-- )
            {
                pwm_buffer[pos++] = (val & (1 << bit)) ? ICLED_PWM_1 : ICLED_PWM_0;
            }
        }
    }

    // Add latch timing (reset pulse)
    for( uint32_t i = 0; i < ICLED_RESET_SLOTS; i++ )
    {
        pwm_buffer[pos++] = 0;
    }

    // Restart DMA transmission with new data
    HAL_TIM_PWM_Stop_DMA( &htim1, TIM_CHANNEL_1 );
    HAL_TIM_PWM_Start_DMA( &htim1, TIM_CHANNEL_1, ( uint32_t* )pwm_buffer, ICLED_BUFFER_SIZE );
}
