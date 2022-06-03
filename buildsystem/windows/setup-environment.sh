#!/bin/bash

# Start by setting up each of the dependency projects
sh ./buildsystem/common/setup-freetype.sh
sh ./buildsystem/common/setup-glfw.sh
sh ./buildsystem/common/setup-imgui.sh
sh ./buildsystem/common/setup-jsonxx.sh
sh ./buildsystem/common/setup-spdlog.sh
sh ./buildsystem/common/setup-crossguid.sh
sh ./buildsystem/common/setup-soil.sh