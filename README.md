# Lumid

A simple Qt GUI that adjusts the display brightness for Linux desktop using [ddcutil](https://github.com/rockowitz/ddcutil).

## Screenshots

![UI](assets/focus.png) &nbsp; &nbsp; ![UI](assets/showAll.png)

## Build

1. Install ddcutil: `dnf install ddcutil`
2. Intall qt5 and qxt: `dnf install sudo dnf install qt5-qtbase-devel qt5-qtmultimedia-devel qt5-qtwebkit-devel qt5-qttools-devel libqxt-qt5-devel`
2. You may need to change the Qt version in CMakeLists.txt to the one you are using.
3. ddcutil requires sudo. You may need to adjust your system settings to avoid entering password. One method could be:
`sudo visudo` in terminal, comment out `%wheel        ALL=(ALL)       ALL` and comment in `%wheel  ALL=(ALL)       NOPASSWD: ALL`.

## Use

This app has only been tested on my own Fedora 37 desktop with GNOME 43.7. I wrote this because the other extensions/apps did not meet my personal requirement of simplicity, less lagging and customizable shortcuts when adjusting the brightness.
 >**Note: The default keyboard shortcuts for increasing and decreasing brightness are F6 and F5, respectively.**

## Known Issues

- Title bar color does not change along with system theme.

- Following errors may occur on **Wayland** due to its compatibility issues with GNOME:

  `QObject::connect: No such signal QPlatformNativeInterface::systemTrayWindowChanged(QScreen*)`

  `QxtGlobalShortcut failed to register: "F5"`

  `QxtGlobalShortcut failed to register: "F6"`
  
  You may also see "Night Light Unavailable", "This could be the result of the graphics driver being used, or the desktop being used remotely" in System Display Setting.

  **So please use Xorg instead! To switch to Xorg, please log out and select Xorg from the bottom-right corner of the login screen.**
  >Note: for the tray icon of lumid and other Apps to show up, you need to install 3rd-party gnome extensions like [Appindicator](https://extensions.gnome.org/extension/615/appindicator-support/). However, the new version the author mentioned in the comment section does not work with lumid properly so please stick to the current version on GNOME Extension for now.
