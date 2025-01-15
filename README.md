# Birthday-Notifier-Debian
Birthday notifier for Debian based distributions
## Table of Contents
1. [General information](#general-information)
2. [Installation](#installation)

### General information
The birthday notifier can add birthdays of the list (name and date of birth), remove birthdays of the list, show the list and enable or disable the notifies.


### Installation
#### Intallation with debian package
In releases there are some file, the debian package is birthday-notifier.deb and the other files are extra information about .deb and the source code.
First, you dowload birthday-notifier.deb and then you install debian package with dpkg:
```
dpkg -i birthday-notifier.deb
```
If you haven't installed the dependences, you have install them with apt (and then write the previus). For example, if you haven't libnotify-bin:
```
sudo apt install libnotify-bin
```
Finally, the birthday-notifier will installed. You can verify it write:
```
birthday-notifier --version
```


#### Installation with debian package (creating .deb from source code)
First, you clone this repository and change of directory:
```
git clone https://github.com/eloyUA/Birthday-Notifier-Debian
cd Birthday-Notifier-Debian
```
Now, if you haven't debhelper, install it.
```
sudo apt install debhelper
```
Finally, you creat the debian package:
```
dpkg-buildpackage -us -uc
```
The debian package will into ../ (2.1 Install with debian package)


#### Installation manual
First, you clone this repository and change of directory:
```
git clone https://github.com/eloyUA/Birthday-Notifier-Debian
cd Birthday-Notifier-Debian
```
Then, if you haven't gcc, intall it.
```
sudo apt install gcc
```
You compile the project:
```
gcc -c src/date.c -o src/date.o
gcc src/birthday-notifier.c src/date.o -o src/birthday-notifier
```
Finally, you move src/birthday-notifier and src/README:
```
sudo mv src/birthday-notifier /usr/bin
sudo mkdir -p /usr/share/doc/birthday-notifier
sudo mv src/README /usr/share/doc/birthday-notifier
```
