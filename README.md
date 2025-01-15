# Birthday-Notifier-Debian
Birthday notifier for Debian based distributions
## Table of Contents
1. [General information](#general-information)
2. [Installation](#installation)

### 1. General information
The birthday notifier can add birthdays of the list (name and date of birth), remove birthdays of the list, show the list and enable or disable the notifies.

### 2. Installation
#### 2.1 Intallation with debian package
In releases there are some file, the debian package is birthday-notifier.deb and the other files are extra information about .deb and the source code.
First, you dowload birthday-notifier.deb and then you install debian package with dpkg:
```
dpkg -i birthday-notifier.deb
```

If you haven't installed the dependences, you have install them with apt (and then write the previus). For example, if you haven't libnotify-bin:
```sudo apt install libnotify-bin```
Finally, the birthday-notifier will installed. You can verify write:

```birthday-notifier --version```

#### 2.2 Installation with debian pakage (creating .deb from source code)
First, you clone this repository:
```git clone https://github.com/eloyUA/Birthday-Notifier-Debian```
Now you 
