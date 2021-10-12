cmake_minimum_required (VERSION 3.8)
add_executable (Win10_Internet_RepairTool "Win10_Internet_RepairTool.cpp" "Win10_Internet_RepairTool.h")
set_target_properties(Win10_Internet_RepairTool PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
