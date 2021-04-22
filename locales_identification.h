/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

class LocalesIdentification {
public:
	static int indexForArabicCountryTz(std::string &tz);

	static int indexForKeyboardLayout(std::string &layout);

	static int getLayoutCodesCount();

	static const char *getDefaultLocale(int index);

	static const char *getArabicLocales(int index);
};

