# Birthday-Notifier-Debian
Birthday Notifier for Debian-based distributions.

## Table of Contents
1. [General Information](#general-information)
2. [Installation](#installation)

### General Information
The Birthday Notifier allows you to:
- Add birthdays to a list (name and date of birth).
- Remove birthdays from the list.
- Display the list of birthdays.
- Enable or disable notifications.

### Installation

#### Installation with Debian Package
In the [Releases](https://github.com/eloyUA/Birthday-Notifier-Debian/releases) section, you will find several files. The `.deb` package is named `birthday-notifier.deb`, while the other files provide additional information or source code.

1. Download the `birthday-notifier.deb` file.
2. Install the Debian package using `dpkg`:
   ```
   sudo dpkg -i birthday-notifier.deb
   ```
3. If dependencies are missing, install them using `apt` (and then retry the `dpkg` command). For example, if `libnotify-bin` is not installed:
   ```
   sudo apt install libnotify-bin
   ```
4. Verify the installation by checking the version:
   ```
   birthday-notifier --version
   ```

#### Installation by creating the debian package from source code
1. Clone the repository and navigate to the project directory:
   ```
   git clone https://github.com/eloyUA/Birthday-Notifier-Debian
   cd Birthday-Notifier-Debian
   ```
2. Ensure that `debhelper` is installed:
   ```
   sudo apt install debhelper
   ```
3. Build the debian package:
   ```
   dpkg-buildpackage -us -uc
   ```
4. The `.deb` package will be generated in the parent directory (`../`). Follow the steps in the [Installation with Debian Package](#installation-with-debian-package) section to install it.

#### Manual Installation
1. Clone the repository and navigate to the project directory:
   ```
   git clone https://github.com/eloyUA/Birthday-Notifier-Debian
   cd Birthday-Notifier-Debian
   ```
2. Ensure that `gcc` is installed:
   ```
   sudo apt install gcc
   ```
3. Compile the project:
   ```
   gcc -c src/date.c -o src/date.o
   gcc src/birthday-notifier.c src/date.o -o src/birthday-notifier
   ```
4. Move the compiled binary and documentation to their appropriate locations:
   ```
   sudo mv src/birthday-notifier /usr/bin
   sudo mkdir -p /usr/share/doc/birthday-notifier
   sudo mv src/README /usr/share/doc/birthday-notifier
   ```
