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

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_painter.hpp"
#include "ui_menu.hpp"
#include "ui_navigation.hpp"
#include "ui_receiver.hpp"
#include "ui_textentry.hpp"
#include "freqman.hpp"
#include "rtc_time.hpp"

namespace ui {
	
class FreqManBaseView : public View {
public:
	FreqManBaseView(
		NavigationView& nav
	);

	void focus() override;
	
	std::string title() const override { return "Freq. manager"; };
	
protected:
	using option_t = std::pair<std::string, int32_t>;
	using options_t = std::vector<option_t>;
	
	NavigationView& nav_;
	freqman_error error_ { NO_ERROR };
	options_t categories { };
	std::function<void(int32_t category_id)> on_change_category { nullptr };
	std::function<void(void)> on_select_frequency { nullptr };
	std::function<void(bool)> on_refresh_widgets { nullptr };
	std::vector<std::string> file_list { };
	int32_t current_category_id { 0 };
	
	bool populate_categories();
	void change_category(int32_t category_id);
	void refresh_list();
	
	freqman_db database { };
	
	Labels label_category {
		{ { 0, 4 }, "Category:", Color::light_grey() }
	};
	
	OptionsField options_category {
		{ 9 * 8, 4 },
		8,
		{ }
	};
	
	MenuView menu_view {
		{ 0, 3 * 8, 240, 23 * 8 },
		true
	};
	Text text_empty {
		{ 7 * 8, 12 * 8, 16 * 8, 16 },
		"Empty category !",
	};
	
	Button button_exit {
		{ 20 * 8, 34 * 8, 10 * 8, 4 * 8 },
		"Exit"
	};
};

class FrequencySaveView : public FreqManBaseView {
public:
	FrequencySaveView(NavigationView& nav, const rf::Frequency value);
	~FrequencySaveView();

private:
	std::string desc_buffer { };
	rtc::RTC datetime { };
	rf::Frequency value_ { };
	std::string str_timestamp { };
	
	void on_save_name();
	void on_save_timestamp();
	void on_tick_second();
	
	SignalToken signal_token_tick_second { };
	
	BigFrequency big_display {
		{ 4, 2 * 16, 28 * 8, 32 },
		0
	};
	
	Text text_save {
		{ 4 * 8, 15 * 8, 8 * 8, 16 },
		"Save as:",
	};
	Button button_save_name {
		{ 4 * 8, 18 * 8, 12 * 8, 32 },
		"Name (set)"
	};
	Button button_save_timestamp {
		{ 4 * 8, 23 * 8, 12 * 8, 32 },
		"Timestamp:"
	};
	Text text_timestamp {
		{ 17 * 8, 24 * 8, 11 * 8, 16 },
		"MM/DD HH:MM",
	};
};

class FrequencyLoadView : public FreqManBaseView {
public:
	std::function<void(rf::Frequency)> on_changed { };
	
	FrequencyLoadView(NavigationView& nav);

private:
	void refresh_widgets(const bool v);
};

class FrequencyManagerView : public FreqManBaseView {
public:
	FrequencyManagerView(NavigationView& nav);
	~FrequencyManagerView();

private:
	std::string desc_buffer { };
	
	void refresh_widgets(const bool v);
	void on_edit_freq(rf::Frequency f);
	void on_edit_desc(NavigationView& nav);
	void on_new_category(NavigationView& nav);
	void on_delete();

	Labels labels {
		{ { 4 * 8 + 4, 26 * 8 }, "Edit:", Color::light_grey() }
	};
	
	Button button_new_category {
		{ 18 * 8, 2, 12 * 8, 20 },
		"Create new"
	};

	Button button_edit_freq {
		{ 0 * 8, 29 * 8, 14 * 8, 32 },
		"Frequency"
	};
	Button button_edit_desc {
		{ 0 * 8, 34 * 8, 14 * 8, 32 },
		"Description"
	};
	
	Button button_delete {
		{ 18 * 8, 27 * 8, 12 * 8, 32 },
		"Delete"
	};
};

} /* namespace ui */
