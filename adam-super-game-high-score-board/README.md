# adam-super-game-high-score-board

A hi-score leaderboard for Coleco Adam Super Games:

* Buck Rogers: Planet of Zoom
* Donkey Kong
* Donkey Kong Jr.
* Dragon's Lair
* Sub-Roc
* Zaxxon

The code here is divided into several sections:

* client/ The software that runs on the ADAM and synchronizes hi-score blocks with the server. Written in C.
* server/ The software running on server which gathers the hi-score blocks and reconciles hi-scores. Written in NodeJS.
* ui/ The user facing web site that displays the hi-scores gathered from hi-score blocks. Written in Emscripten C.

