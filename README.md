# BrighterDarker
A simple Qt GUI that adjusts the display brightness for Linux desktop using ddcutil.<br />
Note:<br />
1. The code has been tested under Fedora 36. Other distros will probably work.<br />
2. You may need to change the Qt version in CMakeLists.txt to the one your are using.<br />
3. ddcutil requires sudo. You may need to adjust your system settings to avoid entering password. One method could be: 
`sudo visudo` in terminal, comment out `%wheel        ALL=(ALL)       ALL` and comment in `%wheel  ALL=(ALL)       NOPASSWD: ALL`.

