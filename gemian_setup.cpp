/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <iostream>
#include <filesystem>

#include "para_variables.h"

static const char *const devBlockPara = "/dev/block/platform/bootdevice/by-name/para";
static const char *const defaultKeyboardFile = "/etc/default/keyboard";
static const std::string_view etcGemianDirectory("/etc/gemian");
static const char *const machineIdResetFile = "/etc/gemian/machine-id-reset";
static const char *const sshHostResetFile = "/etc/gemian/ssh-host-reset";
static const std::string_view etcMachineIdFile("/etc/machine-id");
static const std::string_view dbusMachineIdFile("/var/lib/dbus/machine-id");

void createFile(const std::string &path, std::string_view content) {
    std::ofstream stream(path);
    stream << content;
    stream.close();
}

int main() {
    ParaVariables paraVariables;

    std::ifstream paraStream(devBlockPara, std::ios::binary);
    if (!paraStream.is_open()) {
        std::cout << "Failed to open file: " << devBlockPara << std::endl;
    }

    if (paraVariables.ReadFromStream(paraStream) != ParaVarErrorNone) {
        std::cout << "Failed to read file: " << devBlockPara << std::endl;
    }
    paraStream.close();

    std::string hwKeyboard(paraVariables["keyboard_layout"]);
    std::cout << "HWKeyboard: " << hwKeyboard << "\n";

    if (hwKeyboard.length() > 0) {
        std::ifstream keyboardDefault(defaultKeyboardFile);
        if (!keyboardDefault.is_open()) {
            std::cout << "Failed to open file: " << devBlockPara << std::endl;
        }
        auto found=false;
        while(!keyboardDefault.eof()) {
            std::string toCheck;
            keyboardDefault >> toCheck;
            if (toCheck.find("XKBLAYOUT=") != std::string::npos && toCheck.find(hwKeyboard) != std::string::npos) {
                found = true;
            }
        }
        if (!found) {
            auto set_keymap = "localectl set-x11-keymap " + hwKeyboard;
            system(set_keymap.c_str());
        }
    }

    if (!std::filesystem::exists(machineIdResetFile)) {
        std::filesystem::remove(etcMachineIdFile);
        std::filesystem::remove(dbusMachineIdFile);
        system("dbus-uuidgen --ensure=/etc/machine-id");
        system("dbus-uuidgen --ensure");
        std::filesystem::create_directory(etcGemianDirectory);
        createFile(machineIdResetFile,"yes");
    }

    if (!std::filesystem::exists(sshHostResetFile)) {
        for(auto& p: std::filesystem::directory_iterator("/etc/ssh")) {
            if (p.path().string().find("ssh_host_") != std::string::npos) {
                std::filesystem::remove(p);
            }
        }
        system("dpkg-reconfigure openssh-server");
        std::filesystem::create_directory(etcGemianDirectory);
        createFile(sshHostResetFile,"yes");
    }

    return 0;
}
