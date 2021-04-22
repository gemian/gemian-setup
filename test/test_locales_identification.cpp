/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <catch2/catch_test_macros.hpp>
#include <cstdarg>
#include <iostream>
#include <string>
#include <map>
#include "locales_identification.h"

TEST_CASE("Test Locales", "[locales_identification]")
{
	std::string layoutSwissGerman("ch-de");
	int layoutIndex = LocalesIdentification::indexForKeyboardLayout(layoutSwissGerman);
	REQUIRE(layoutIndex == 25);
	std::string locale(LocalesIdentification::getDefaultLocale(layoutIndex));
	REQUIRE(locale == "de_CH.UTF-8");
	std::string africaJubaTz("Africa/Juba");
	auto tzIndex = LocalesIdentification::indexForArabicCountryTz(africaJubaTz);
	REQUIRE(tzIndex == 15);
	locale = LocalesIdentification::getArabicLocales(tzIndex);
	REQUIRE(locale == "ar_SS UTF-8");
}