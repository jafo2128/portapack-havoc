/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
 * Copyright (C) 2016 Furrtek
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __UI_ABOUT_H__
#define __UI_ABOUT_H__

#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "ui_font_fixed_8x16.hpp"

#include <cstdint>

namespace ui {

class CreditsWidget : public Widget {
public:
	CreditsWidget(Rect parent_rect);
	
	void on_show() override;
	void on_hide() override;

	void paint(Painter&) override;
	
	void new_row(const std::array<Color, 240>& pixel_row);

private:
	void clear();
};

class AboutView : public View {
public:
	AboutView(NavigationView& nav);
	
	void focus() override;
	
	std::string title() const override { return "About"; };

private:
	void update();

	uint8_t credits_index { 0 };
	uint8_t render_line { 0 };
	Coord start_pos { 0 };
	uint8_t slow_down { 0 };
	int32_t timer { 0 };
	bool loop { false };
	
	std::string text { };

	enum flags {
		SECTION = 0,
		MEMBER = 1,
		MEMBER_LF = 0x41,
		TITLE = 2,
		TITLE_LF = 0x42,
		END = 0x80
	};
		
	typedef struct credits_t {
		size_t start_pos;
		std::string text;
		int32_t delay;
	} credits_t;
	
	const credits_t credits[19] = {	
		{ 60,		"PortaPack|HAVOC",				0 },
		{ 7 * 8,	"Git hash " GIT_REVISION,		16 },
		{ 9 * 8,	"Gurus  J. Boone",				0 },
		{ 16 * 8,	"M. Ossmann",					16 },
		{ 9 * 8,	"HAVOC  Furrtek",				16 },
		{ 5 * 8,	"POCSAG rx  T. Sailer",			0 },
		{ 16 * 8,	"E. Oenal",						16 },
		{ 2 * 8,	"RDS waveform  C. Jacquet", 	16 },
		{ 5 * 8,	"Xy. infos  cLx", 				16 },
		{ 0,		"OOK scan trick  Samy Kamkar",	16 },
		{ 5 * 8,	"World map  NASA", 				24 },
		{ 12 * 8,	"Thanks",						16 },
		{ 1 * 8,	"Rainer Matla     Keld Norman",	0 },
		{ 1 * 8,	" Giorgio C.         DC1RDB",	0 },
		{ 1 * 8,	" Sigmounte           Waax",	0 },
		{ 1 * 8,	" Windyoona         Channels",	0 },
		{ 1 * 8,	"   F4GEV",						24 },
		{ 12 * 8,	"MMXVII",						-1 }
	};
	
	CreditsWidget credits_display {
		{ 0, 16, 240, 240 }
	};

	Button button_ok {
		{ 72, 272, 96, 24 },
		"OK"
	};
	
	MessageHandlerRegistration message_handler_update {
		Message::ID::DisplayFrameSync,
		[this](const Message* const) {
			this->update();
		}
	};
	
};

} /* namespace ui */

#endif/*__UI_ABOUT_H__*/
