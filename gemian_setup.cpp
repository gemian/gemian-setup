/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <iostream>
#include <filesystem>

#include "para_variables.h"
#include "locales_identification.h"

static const char *const devBlockPara = "/dev/disk/by-partlabel/para";
static const char *const defaultKeyboardFile = "/etc/default/keyboard";
static const std::string_view etcGemianDirectory("/etc/gemian");
static const char *const machineIdResetFile = "/etc/gemian/machine-id-reset";
static const char *const sshHostResetFile = "/etc/gemian/ssh-host-reset";
static const char *const timeZoneSetFile = "/etc/gemian/time-zone-set";
static const char *const localeSetFile = "/etc/gemian/locale-set";
static const std::string_view etcMachineIdFile("/etc/machine-id");
static const std::string_view dbusMachineIdFile("/var/lib/dbus/machine-id");
static const char *const operationPerformedContent = "done";

void createFile(const std::string &path, std::string_view content) {
	std::ofstream stream(path);
	stream << content;
	stream.close();
}

int main() {
	ParaVariables paraVariables;

	std::ifstream paraStream(devBlockPara, std::ios::binary);
	if (paraStream.is_open()) {
		if (paraVariables.ReadFromStream(paraStream) != ParaVarErrorNone) {
			std::cout << "Failed to read file: " << devBlockPara << std::endl;
		}
		paraStream.close();
	} else {
		std::cout << "Failed to open file: " << devBlockPara << std::endl;
	}

	std::string hwKeyboard(paraVariables["keyboard_layout"]);
	std::cout << "HW Keyboard: " << hwKeyboard << "\n";
	std::cout << "Time Zone: " << paraVariables["time_zone"] << "\n";

	if (hwKeyboard.length() > 0) {
		auto found = false;
		std::ifstream keyboardDefault(defaultKeyboardFile);
		if (keyboardDefault.is_open()) {
			while (!keyboardDefault.eof()) {
				std::string toCheck;
				keyboardDefault >> toCheck;
				if (toCheck.find("XKBLAYOUT=") != std::string::npos && toCheck.find(hwKeyboard) != std::string::npos) {
					found = true;
				}
			}
		} else {
			std::cout << "Failed to open file: " << defaultKeyboardFile << std::endl;
		}
		if (!found) {
			auto setKeymap = "localectl set-x11-keymap " + hwKeyboard;
			auto err = system(setKeymap.c_str());
			std::cout << setKeymap << " : " << err << "\n";
		}
	}

	if (!std::filesystem::exists(localeSetFile) && (paraVariables["keyboard_layout"].length() > 0)) {
		int layoutIndex = LocalesIdentification::indexForKeyboardLayout(paraVariables["keyboard_layout"]);
		if (layoutIndex >= 0 && (layoutIndex < LocalesIdentification::getLayoutCodesCount())) {
			std::string locale(LocalesIdentification::getDefaultLocale(layoutIndex));
			if (locale.find('*') != std::string::npos && paraVariables["time_zone"].length() > 0) {
				auto index = LocalesIdentification::indexForArabicCountryTz(paraVariables["time_zone"]);
				locale = LocalesIdentification::getArabicLocales(index);
			}
			if (paraVariables["language"].length() > 0 && paraVariables["language"] != "auto") {
			    locale = paraVariables["language"]+".UTF-8";
			}
			auto sedLocalGenCmd = "sed -i /etc/locale.gen -e 's/# " + locale + "/" + locale + "/'";
			auto err = system(sedLocalGenCmd.c_str());
			std::cout << sedLocalGenCmd << " : " << err << "\n";

			auto localeGenCmd = "locale-gen";
			err = system(localeGenCmd);
			std::cout << localeGenCmd << " : " << err << "\n";

			char buffer[128];
			std::string result;
			std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("localectl list-locales", "r"), pclose);
			if (!pipe) {
				std::cout << "localectl list-locales" << " : " << "popen() failed!" << "\n";
			}
			auto localeDotPos = locale.find('.');
			if (localeDotPos == std::string::npos) {
				localeDotPos = locale.find(' ');
			}
			while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
				result = buffer;
				auto resultDotPos = result.find('.');
				if (resultDotPos == std::string::npos) {
					resultDotPos = result.find(' ');
				}
				if (result.substr(0, resultDotPos) == locale.substr(0, localeDotPos)) {
					locale = result;
				}
			}

			auto localeSetCmd = "localectl set-locale " + locale + "";
			err = system(localeSetCmd.c_str());
			std::cout << localeSetCmd << " : " << err << "\n";

			createFile(localeSetFile, operationPerformedContent);
		}
	}

	if (!std::filesystem::exists(machineIdResetFile)) {
		std::filesystem::remove(etcMachineIdFile);
		std::filesystem::remove(dbusMachineIdFile);
		const char *idGen1Cmd = "dbus-uuidgen --ensure=/etc/machine-id";
		auto err = system(idGen1Cmd);
		std::cout << idGen1Cmd << " : " << err << "\n";
		const char *idGen2Cmd = "dbus-uuidgen --ensure";
		err = system(idGen2Cmd);
		std::cout << idGen2Cmd << " : " << err << "\n";
		std::filesystem::create_directory(etcGemianDirectory);
		createFile(machineIdResetFile, operationPerformedContent);
	}

	if (!std::filesystem::exists(sshHostResetFile)) {
		for (auto &p: std::filesystem::directory_iterator("/etc/ssh")) {
			if (p.path().string().find("ssh_host_") != std::string::npos) {
				std::filesystem::remove(p);
			}
		}
		const char *reConfigCmd = "dpkg-reconfigure openssh-server";
		auto err = system(reConfigCmd);
		std::cout << reConfigCmd << " : " << err << "\n";
		std::filesystem::create_directory(etcGemianDirectory);
		createFile(sshHostResetFile, operationPerformedContent);
	}

	if (!std::filesystem::exists(timeZoneSetFile) && (paraVariables["time_zone"].length() > 0)) {
		auto setTimeZone = "timedatectl set-timezone " + paraVariables["time_zone"];
		auto err = system(setTimeZone.c_str());
		std::cout << setTimeZone << " : " << err << "\n";
		createFile(timeZoneSetFile, operationPerformedContent);
	}

	return 0;
}
