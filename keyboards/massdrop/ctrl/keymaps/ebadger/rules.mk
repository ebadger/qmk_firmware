SRC += hid_lamparray.c
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
#RAW_ENABLE = yes             # Raw devicen
#EXTRAKEY_ENABLE = no
#NKRO_ENABLE = no
OPT_DEFS += -DHID_LAMPARRAY_ENABLE -DKEYBOARD_JPN
