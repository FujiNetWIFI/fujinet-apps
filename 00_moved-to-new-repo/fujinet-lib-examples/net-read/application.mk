####################################
# REST_SERVER_ADDRESS
####################################
# If REST_SERVER_ADDRESS is already set on command line, these do nothing, otherwise they will work out the local IP address, ignoring 172 range which is WSL on Windows.
ifeq ($(detected_OS),Linux)
  REST_SERVER_ADDRESS = \"$(shell ip -4 addr show scope global | grep -oP '(?<=inet\s)\d+(\.\d+){3}')\"
else ifeq ($(detected_OS),$(filter $(detected_OS),MSYS MINGW Cygwin Windows))
  REST_SERVER_ADDRESS = \"$(shell ipconfig | grep -oP 'IPv4.*:\s*\K[\d.]+' | grep -v '^172\.')\"
endif

REST_SERVER_PORT = \"8080\"

CFLAGS += -DREST_SERVER_ADDRESS=$(REST_SERVER_ADDRESS) -DREST_SERVER_PORT=$(REST_SERVER_PORT)
ASFLAGS += -DREST_SERVER_ADDRESS=$(REST_SERVER_ADDRESS) -DREST_SERVER_PORT=$(REST_SERVER_PORT)
LDFLAGS += -DREST_SERVER_ADDRESS=$(REST_SERVER_ADDRESS) -DREST_SERVER_PORT=$(REST_SERVER_PORT)
