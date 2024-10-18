/*
 * vibecheck_rgb_sequences.h
 *
 *  Created on: Sep 30, 2024
 *      Author: johnt
 */

#ifndef INC_VIBECHECK_RGB_SEQUENCES_H_
#define INC_VIBECHECK_RGB_SEQUENCES_H_

#include "vibecheck_rgb.h"

static const uint32_t top_sequence_len = 4;
static const uint32_t top_sequence_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color top_sequence_colors[] = {
		{.r = 0, .g = 0, .b = 255},
		{.r = 0, .g = 85, .b = 255},
		{.r = 0, .g = 170, .b = 255},
		{.r = 0, .g = 255, .b = 255},
		{.r = 85, .g = 255, .b = 170},
		{.r = 170, .g = 255, .b = 85},
		{.r = 255, .g = 255, .b = 0},
		{.r = 255, .g = 170, .b = 0},
		{.r = 255, .g = 85, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 255},
		{.r = 0, .g = 85, .b = 255},
		{.r = 0, .g = 170, .b = 255},
		{.r = 0, .g = 255, .b = 255},
		{.r = 85, .g = 255, .b = 170},
		{.r = 170, .g = 255, .b = 85},
		{.r = 255, .g = 255, .b = 0},
		{.r = 255, .g = 170, .b = 0},
		{.r = 255, .g = 85, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};

static const uint32_t connect_sequence_len = 18;
static const uint32_t connect_sequence_times[] = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
static const VibeCheckRGB_Color connect_sequence_colors[] = {
		{.r = 0, .g = 0, .b = 255},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 85, .b = 255},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 170, .b = 255},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 255, .b = 255},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 85, .g = 255, .b = 170},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 170, .g = 255, .b = 85},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 255, .g = 255, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 255, .g = 170, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 255, .g = 85, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 255, .g = 170, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 255, .g = 255, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 170, .g = 255, .b = 85},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 85, .g = 255, .b = 170},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 255, .b = 255},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 170, .b = 255},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 85, .b = 255},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 255},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};


static const uint32_t led_shell_success_len = 4;
static const uint32_t led_shell_success_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color led_shell_success_colors[] = {
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},
		{.r = 0, .g = 100, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};


static const uint32_t led_shell_failure_len = 4;
static const uint32_t led_shell_failure_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color led_shell_failure_colors[] = {
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},
		{.r = 100, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};



static const uint32_t led_sensor0_connected_len = 4;
static const uint32_t led_sensor0_connected_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color led_sensor0_connected_colors[] = {
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};


static const uint32_t led_sensor0_disconnected_len = 4;
static const uint32_t led_sensor0_disconnected_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color led_sensor0_disconnected_colors[] = {
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};


static const uint32_t led_sensor1_connected_len = 4;
static const uint32_t led_sensor1_connected_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color led_sensor1_connected_colors[] = {
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};


static const uint32_t led_sensor1_disconnected_len = 4;
static const uint32_t led_sensor1_disconnected_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color led_sensor1_disconnected_colors[] = {
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};




static const uint32_t led_sensor2_connected_len = 4;
static const uint32_t led_sensor2_connected_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color led_sensor2_connected_colors[] = {
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},
		{.r = 0, .g = 25, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};


static const uint32_t led_sensor2_disconnected_len = 4;
static const uint32_t led_sensor2_disconnected_times[] = {100, 100, 100, 100};
static const VibeCheckRGB_Color led_sensor2_disconnected_colors[] = {
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},
		{.r = 25, .g = 0, .b = 0},

		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
		{.r = 0, .g = 0, .b = 0},
};


static const uint32_t led_sensor_connected_len[] = {led_sensor0_connected_len, led_sensor1_connected_len, led_sensor2_connected_len};
static const uint32_t* led_sensor_connected_times[] = {led_sensor0_connected_times, led_sensor1_connected_times, led_sensor2_connected_times};
static const VibeCheckRGB_Color* led_sensor_connected_colors[] = {led_sensor0_connected_colors, led_sensor1_connected_colors, led_sensor2_connected_colors};


static const uint32_t led_sensor_disconnected_len[] = {led_sensor0_disconnected_len, led_sensor1_disconnected_len, led_sensor2_disconnected_len};
static const uint32_t* led_sensor_disconnected_times[] = {led_sensor0_disconnected_times, led_sensor1_disconnected_times, led_sensor2_disconnected_times};
static const VibeCheckRGB_Color* led_sensor_disconnected_colors[] = {led_sensor0_disconnected_colors, led_sensor1_disconnected_colors, led_sensor2_disconnected_colors};

#endif /* INC_VIBECHECK_RGB_SEQUENCES_H_ */
