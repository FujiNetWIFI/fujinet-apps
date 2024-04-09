-include makefiles/fujinet-lib.mk

# To add quotes INSIDE the version string, you need 3 backslashes. First 2 create a single backslash, 3rd is to escape the quotation
# Single quotes need no backslashes at all.
VERSION_STRING = "2024-04-09 \\\"GOLD_RUSH\\\""

# Then you have to add even more backslashes here. This was fun to work out.
CFLAGS += -DVERSION_STRING=\"$(VERSION_STRING)\"
