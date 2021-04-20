/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "locales_identification.h"

static const int layoutCodesCount = 26;
const char *keyboard_layout_codes[] =
        {"ara", "at", "be", "cat", "hr", "cz",
         "dk", "gb", "us", "ie", "dvorak-us", "fr", "fi",
         "de", "gr", "hu", "it", "jp", "no", "pl",
         "pt", "ru", "sk", "es", "se", "ch-de", nullptr
        };
const char *default_locale[] = {
        "ar_*.UTF-8", "en_AU.UTF-8", "be_BY.UTF-8",
        "ca_ES.UTF-8", "hr_HR.UTF-8", "cs_CZ.UTF-8", "da_DK.UTF-8",
        "en_GB.UTF-8", "en_US.UTF-8", "en_IE.UTF-8", "en_US.UTF-8",
        "fr_FR.UTF-8", "fi_FI.UTF-8", "de_DE.UTF-8", "el_GR.UTF-8",
        "hu_HU.UTF-8", "it_IT.UTF-8", "ja_JP.UTF-8", "nn_NO.UTF-8",
        "pl_PL.UTF-8", "pt_PT.UTF-8", "ru_RU.UTF-8", "sk_SK.UTF-8",
        "es_ES.UTF-8", "sv_SE.UTF-8", "de_CH.UTF-8", nullptr
};
static const int arabicCountriesCount = 19;
const char *arabicCountriesTZ[]{
        "Asia/Dubai",
        "Asia/Bahrain",
        "Africa/Algiers",
        "Africa/Cairo",
        "Indian",
        "Asia/Baghdad",
        "Asia/Amman",
        "Asia/Kuwait",
        "Asia/Beirut",
        "Africa/Tripoli",
        "Africa/Casablanca",
        "Asia/Muscat",
        "Asia/Qatar",
        "Asia/Bahrain",
        "Africa/Khartoum",
        "Africa/Juba",
        "Asia/Damascus",
        "Africa/Tunis",
        "Asia/Aden",
        nullptr
};
const char *arabicLocales[]{
        "ar_AE.UTF-8",
        "ar_BH.UTF-8",
        "ar_DZ.UTF-8",
        "ar_EG.UTF-8",
        "ar_IN UTF-8",
        "ar_IQ.UTF-8",
        "ar_JO.UTF-8",
        "ar_KW.UTF-8",
        "ar_LB.UTF-8",
        "ar_LY.UTF-8",
        "ar_MA.UTF-8",
        "ar_OM.UTF-8",
        "ar_QA.UTF-8",
        "ar_SA.UTF-8",
        "ar_SD.UTF-8",
        "ar_SS UTF-8",
        "ar_SY.UTF-8",
        "ar_TN.UTF-8",
        "ar_YE.UTF-8",
        nullptr
};

int LocalesIdentification::indexForArabicCountryTz(std::string &tz) {
    for (int i = 0; i <= arabicCountriesCount; i++) {
        std::string tz_name(arabicCountriesTZ[i]);
        if (tz_name == tz) {
            return i;
        }
    }
    return 3; // Egypt
}

int LocalesIdentification::indexForKeyboardLayout(std::string &layout) {
    for (int i = 0; !layout.empty() && keyboard_layout_codes[i] != nullptr; i++) {
        if (layout == keyboard_layout_codes[i]) {
            return i;
        }
    }
    return 8; // USA
}

int LocalesIdentification::getLayoutCodesCount() {
    return layoutCodesCount;
}

const char *LocalesIdentification::getDefaultLocale(int index) {
    return default_locale[index];
}

const char *LocalesIdentification::getArabicLocales(int index) {
    return arabicLocales[index];
}
