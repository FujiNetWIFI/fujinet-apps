CFLAGS += -I ../fn_network

contiki-webbrowser.apple2 \
contiki-webbrowser.apple2enh: $(wildcard ../fn_network/fujinet-network-apple2_*.lib)

contiki-webbrowser.atari: $(wildcard ../fn_network/fujinet-network-atari_*.lib)
