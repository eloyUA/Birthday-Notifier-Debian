/**
 * PROGRAM: Birthday-Notifier
 * FILE: birthday-notifier.c
 * VERSION: 1.0.0
 * HISTORICAL: Created by Eloy Urriens Arpal on 1/13/2025
 * DESCRIPTION:
 *		This program saves names of people with their date of birth.
 		When the linux starts, it checks all birthdays and
 * 		if someone has a birthday today, a notify is sent.
 * 		This program be able to:
 * 			- Add (name) (date of birth)
 * 			- Remove (name)
 * 			- List of the names with years old
 * 			- Enable checking when Linux starts
 * 			- Disable checking when Linux starts
 * 			- Show status of checking
 * 			- Help
 * CC: BY SA
 */

#include "date.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <time.h>

#define VERSION "1.0"
#define MAX_NAME_LENGTH 64
#define PATH_HELP_FILE "/usr/share/doc/birthday-notifier/README"
#define PATH_CONFIG ".config/birthday-notifier"
#define PATH_DATA_FILE ".config/birthday-notifier/birthdays"
#define PATH_DATA_FILE_AUX ".config/birthday-notifier/birthdays-aux"
#define PATH_BIRTHDAY_NOTIFIER_DESKTOP ".config/autostart/birthday-notifier.desktop"
#define PATH_BIRTHDAY_NOTIFIER_BIN "/usr/bin/birthday-notifier"

/**
 * PATH_CONFIG: (/home/nameUsr + this path)
 * PATH_DATA_FILE:
 * 		The birthdays are saved in this file (/home/nameUsr/ + this path)
 * 		(this path = PATH_CONFIG + "birthday")
 * PATH_DATA_FILE_AUX:
 * 		This file is temporary (/home/nameUsr/ + this path)
 * 		(this path = PATH_CONFIG + "birthday-aux")
 * PATH_BIRTHDAY_NOTIFIER_DESKTOP:
 * 		When linux starts, this file executes birthday-notifier --check
 * 		(/home/nameUsr/ + this path)
 * PATH_BIRTHDAY_NOTIFIER_BIN: The path of birthday-notifier (bin)
 */

/**
 * FUNCTION: errorBirthdayNotifier
 * INPUT: Any error string.
 * REQUIREMENTS: None.
 * OUTPUT: The program prints the error and exits.
 */
void errorBirthdayNotifier(char *msgError) {
	char msg[512];
	snprintf(msg, sizeof(msg), "ERROR, in notifier.c: %s", msgError);
	perror(msg);
	exit(1);
}

/**
 * FUNCTION: readStringFromFile
 * INPUT:
 * 		A string, maxLength (the maximum length of the string),
 * 		until (the reader stop when reads this character) and the pointer to file.
 * REQUIREMENTS:
 * 		char string[maxLength] (The maxLength must be the maximum length of the string)
 * OUTPUT:
 * 		The string has everything read up to the variable until. The until isn't included.
 */
void readStringFromFile(char *string, char maxLength, char until, FILE *f) {
	char cont = 0;
	char c = fgetc(f);
	while (!feof(f) && cont < maxLength - 1 && c != EOF && c != until) {
		string[cont] = c;
		c = fgetc(f);
		cont++;
	}
	string[cont] = '\0';

	if (cont == maxLength - 1 && c != EOF && c != '\n')
		errorBirthdayNotifier("The variable maxLength of the string has to be greater");

	if (!feof(f) && c == EOF)
		errorBirthdayNotifier("Couldn't read birthday file");
}

/**
 * FUNCTION: readNameFromFile
 * INPUT: The name and the pointer to file.
 * REQUIREMENTS: char name[MAX_NAME_LENGTH] (The name must be reseved)
 * OUTPUT: The name read from f.
 */
void readNameFromFile(char *name, FILE *f) {
	readStringFromFile(name, MAX_NAME_LENGTH, ';', f);
}

/**
 * FUNCTION: readDateFromFile
 * INPUT: The date string and the pointer to file.
 * REQUIREMENTS: char date[MAX_DATE_LENGTH] (The date must be reseved)
 * OUTPUT: The date string read from f.
 */
void readDateFromFile(char *stringDate, FILE *f) {
	readStringFromFile(stringDate, MAX_DATE_LENGTH, '\n', f);
}

/**
 * OUTPUT: True (valid).
 */
bool validateName(char *name) {
	return strlen(name) <= MAX_NAME_LENGTH;
}

/**
 * FUNCTION: strcatPathUsrAndOtherPath
 * INPUT: A otherPath.
 * REQUIREMENTS: The finalPath and otherPath must be reserved.
 * OUTPUT: finalPath = path of the usr + "/" + otherPath
 */
void strcatPathUsrAndOtherPath(char *finalPath, char *otherPath) {
	const char *usrPath = getenv("HOME");
	if (usrPath == NULL)
		errorBirthdayNotifier("User path not found");
	strcpy(finalPath, usrPath);
	strcat(finalPath, "/");
	strcat(finalPath, otherPath);
}

void createDataFileIfIsNotCreated() {
	char path[256];
	char command[512];

	strcatPathUsrAndOtherPath(path, PATH_CONFIG);
	snprintf(command, sizeof(command), "test -f %s", path);
	if (system(command) != 0) {
		strcpy(command, "mkdir -p ");
		strcat(command, path);
		system(command);
	}

	strcatPathUsrAndOtherPath(path, PATH_DATA_FILE);
	snprintf(command, sizeof(command), "test -f %s", path);
	if (system(command) != 0) {
		strcatPathUsrAndOtherPath(path, PATH_DATA_FILE);
		strcpy(command, "touch ");
		strcat(command, path);
		system(command);
	}
}

/**
 * FUNCTION: isInBirthdayFile
 * INPUT: The name searched.
 * REQUIREMENTS: None
 * OUTPUT: True <=> The name searched is in the file
 */
bool isInBirthdayFile(char *nameSearched) {
	char path_data_file[512];
	strcatPathUsrAndOtherPath(path_data_file, PATH_DATA_FILE);

	FILE *f = fopen(path_data_file, "r");
	if (f == NULL) {
		createDataFileIfIsNotCreated();
		f = fopen(path_data_file, "r");
		if (f == NULL)
			errorBirthdayNotifier("Cannot open birthday file");
	}

	char name[MAX_NAME_LENGTH];
	char stringDate[MAX_DATE_LENGTH];

	readNameFromFile(name, f);
	readDateFromFile(stringDate, f);
	while (!feof(f) && strcmp(name, nameSearched) != 0) {
		readNameFromFile(name, f);
		readDateFromFile(stringDate, f);
	}
	fclose(f);

	return strcmp(name, nameSearched) == 0;
}

/**
 * FUNCTION: addBirthday
 * INPUT: The command arguments (argv[2]: Name and argv[3]: Date)
 * REQUIREMENTS: The function isCommandAddBirthday must be return true.
 * MODIFIES: In the birthday file the birthday is added.
 */
void addBirthday(char **argv) {
	if (!validateName(argv[2]))
		errorBirthdayNotifier("The name isn't valid");

	if (isInBirthdayFile(argv[2]))
		errorBirthdayNotifier("There is a person with same name");

	Date date;
	char stringDate[MAX_DATE_LENGTH];
	char path_data_file[512];
	FILE *f;

	date = newDate(argv[3]);
	formatDateToStringDate(stringDate, date);
	strcatPathUsrAndOtherPath(path_data_file, PATH_DATA_FILE);
	f = fopen(path_data_file, "a");
	if (f == NULL) {
		createDataFileIfIsNotCreated();
		f = fopen(path_data_file, "r");
		if (f == NULL)
			errorBirthdayNotifier("Cannot open birthday file");
	}

	fwrite(argv[2], sizeof(char), strlen(argv[2]), f);
	fputc(';', f);
	fwrite(stringDate, sizeof(char), strlen(stringDate), f);
	fputc('\n', f);
	fclose(f);
}

bool isCommandAddBirthday(int argc, char **argv) {
	return argc == 4 && (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--add") == 0);
}

/**
 * FUNCTION: removeBirthday
 * INPUT: The command arguments (argv[2]: Name)
 * REQUIREMENTS: The function isCommandRemoveBirthday must be return true.
 * MODIFIES: In the birthday file the birthday is deleted.
 */
void removeBirthday(char **argv) {
	char path_data_file[512];
	char path_data_file_aux[512];

	strcatPathUsrAndOtherPath(path_data_file, PATH_DATA_FILE);
	strcatPathUsrAndOtherPath(path_data_file_aux, PATH_DATA_FILE_AUX);

	FILE *fr = fopen(path_data_file, "r");
	if (fr == NULL) {
		createDataFileIfIsNotCreated();
		fr = fopen(path_data_file, "r");
		if (fr == NULL)
			errorBirthdayNotifier("Cannot open birthday file");
	}

	FILE *fw = fopen(path_data_file_aux, "w");
	if (fw == NULL)
		errorBirthdayNotifier("Cannot open birthday file");

	bool deleted;
	char name[MAX_NAME_LENGTH], nameToDelete[MAX_NAME_LENGTH];
	char stringDate[MAX_DATE_LENGTH];

	deleted = false;
	strcpy(nameToDelete, argv[2]);
	readNameFromFile(name, fr);
	while (!feof(fr)) {
		readDateFromFile(stringDate, fr);

		if (strcmp(nameToDelete, name) == 0) {
			deleted = true;
		}
		else {
			fwrite(name, sizeof(char), strlen(name), fw);
			fputc(';', fw);
			fwrite(stringDate, sizeof(char), strlen(stringDate), fw);
			fputc('\n', fw);
		}
		readNameFromFile(name, fr);
	}
	fclose(fr);
	fclose(fw);

	char command[128];
	if (deleted) {
		strcpy(command, "mv ");
		strcat(command, path_data_file_aux);
		strcat(command, " ");
		strcat(command, path_data_file);
		system(command);

		printf("The person was deleted.\n");
	}
	else {
		strcpy(command, "rm ");
		strcat(command, path_data_file_aux);
		system(command);
		printf("The person wasn't delted because their name isn't in the birthdays file.\n");
	}
}

bool isCommandRemoveBirthday(int argc, char **argv) {
	return argc == 3 && (strcmp(argv[1], "-r") == 0 ||
						strcmp(argv[1], "--remove") == 0);
}

/**
 * FUNCTION: showListBirthdays
 * OUTPUT: Show all birthdays.
 */
void showListBirthdays() {
	char path_data_file[512];
	strcatPathUsrAndOtherPath(path_data_file, PATH_DATA_FILE);

	FILE *f = fopen(path_data_file, "r");
	if (f == NULL) {
		createDataFileIfIsNotCreated();
		f = fopen(path_data_file, "r");
		if (f == NULL)
			errorBirthdayNotifier("Cannot open birthday file");
	}

	char name[MAX_NAME_LENGTH];
	Date date, dateNow;
	char stringDate[MAX_DATE_LENGTH];
	int cont;

	cont = 0;
	dateNow = getDateNow();
	readNameFromFile(name, f);
	if (feof(f)) {
		printf("There aren't birthdays.\n");
	}
	else {
		readDateFromFile(stringDate, f);
		while (!feof(f)) {
			date = newDate(stringDate);
			printf("%d.%s (%s) (%d age)\n", cont + 1, name,
					stringDate, calculateDifferenceInYears(date, dateNow));

			cont = cont + 1;
			readNameFromFile(name, f);
			readDateFromFile(stringDate, f);
		}
	}

	fclose(f);
}

bool isCommandShowListBirthdays(int argc, char **argv) {
	return argc == 2 && (strcmp(argv[1], "-l") == 0 ||
						strcmp(argv[1], "--list") == 0);
}

/**
 * FUNCTION: enableNotifier
 * OUTPUT:
 * 		The notifier is enabled so when linux init,
 * 		the notifier search in birthday file.
 * 		If there are birthdays are shown.
 */
void enableNotifier() {
	char txt[512];
	char command[550];
	char path_birthday_notifier_desktop[512];

	strcatPathUsrAndOtherPath(
		path_birthday_notifier_desktop,
		PATH_BIRTHDAY_NOTIFIER_DESKTOP);

	strcpy(txt, "[Desktop Entry]\n");
	strcat(txt, "Type=Application\n");
	strcat(txt, "Name=Simple-Birthday-Notifier\n");
	strcat(txt, "Exec=");
	strcat(txt, PATH_BIRTHDAY_NOTIFIER_BIN);
	strcat(txt, " --check\n");
	strcat(txt, "Terminal=false");

	strcpy(command, "echo \"");
	strcat(command, txt);
	strcat(command, "\"");
	strcat(command, " > ");
	strcat(command, path_birthday_notifier_desktop);

	system(command);
	printf("The notifier had been enabled.\n");
}

bool isCommandEnableNotifier(int argc, char **argv) {
	return argc == 2 && (strcmp(argv[1], "-e") == 0 ||
						strcmp(argv[1], "--enable") == 0);
}

/**
 * FUNCTION: disableNotifier
 * OUTPUT:
 * 		The notifier is disabled so when linux init,
 * 		the notifier don't search in birthday file.
 */
void disableNotifier() {
	char command[128];
	char path_birthday_notifier_desktop[512];

	strcatPathUsrAndOtherPath(
		path_birthday_notifier_desktop,
		PATH_BIRTHDAY_NOTIFIER_DESKTOP);

	strcpy(command, "rm ");
	strcat(command, path_birthday_notifier_desktop);
	system(command);

	printf("The notifier had been disabled.\n");
}

bool isCommandDisableNotifier(int argc, char **argv) {
	return argc == 2 && (strcmp(argv[1], "-d") == 0 ||
						strcmp(argv[1], "--disable") == 0);
}

/**
 * FUNCTION: showStatusNotifier
 * OUTPUT: 
 * 		The status of the notifier is shown.
 * 		For example, if the notifier is enabled, in the display is shown "enabled".
 */
void showStatusNotifier() {
	char command[512];
	char path_birthday_notifier_desktop[512];

	strcatPathUsrAndOtherPath(
		path_birthday_notifier_desktop,
		PATH_BIRTHDAY_NOTIFIER_DESKTOP);

	snprintf(command, sizeof(command), "test -f %s", path_birthday_notifier_desktop);
	if (system(command) == 0) {
		printf("The notifier is enabled.\n");
	}
	else {
		printf("The notifier is disabled.\n");
	}
}

bool isCommandShowStatusNotifier(int argc, char **argv) {
	return argc == 2 && (strcmp(argv[1], "-s") == 0 ||
						strcmp(argv[1], "--status") == 0);
}

/**
 * FUNCTION: checkBirthdays
 * REQUIREMENTS: Maximum 16 birthdays in one day.
 * OUTPUT:
 * 		The notifier search in the birthday file.
 * 		If there are some birthday todoy is shown.
 */
void checkBirthdays() {
	char path_data_file[512];

	strcatPathUsrAndOtherPath(path_data_file, PATH_DATA_FILE);
	FILE *f = fopen(path_data_file, "r");
	if (f == NULL) {
		createDataFileIfIsNotCreated();
		f = fopen(path_data_file, "r");
		if (f == NULL)
			errorBirthdayNotifier("Cannot open birthday file");
	}

	Date date, dateNow;
	char name[MAX_NAME_LENGTH];
	char stringDate[MAX_DATE_LENGTH];
	char names[16][MAX_NAME_LENGTH];
	char ages[16];
	char cont;

	cont = 0;
	dateNow = getDateNow();
	readNameFromFile(name, f);
	while (!feof(f) && cont < 16) {
		readDateFromFile(stringDate, f);
		date = newDate(stringDate);
		if (getMonth(date) == getMonth(dateNow) && getDay(date) == getDay(dateNow)) {
			strcpy(names[cont], name);
			ages[cont] = calculateDifferenceInYears(date, dateNow);
			cont++;
		}

		readNameFromFile(name, f);
	}
	fclose(f);

	if (cont > 0) {
		char aux[3];
		char command[16 * (MAX_NAME_LENGTH + 32)];
		strcpy(command, "notify-send ");
		strcat(command, "\"Birthday-Notifier\" \"");
		for (char i = 0; i < cont; i++) {
			strcat(command, names[i]);
			strcat(command, "'s birthdays is today (");
			sprintf(aux, "%hhd", ages[i]);
			strcat(command, aux);
			strcat(command, " years old).\n");
		}
		strcat(command, "\"");
		printf("%s", command);
		sleep(3);
		system(command);
	}
}

bool isCommandCheckBirthdays(int argc, char **argv) {
	return argc == 2 && (strcmp(argv[1], "-c") == 0 ||
						strcmp(argv[1], "--check") == 0);
}

/**
 * FUNCTION: helpOption
 * OUTPUT: Print the help file
 */
void helpOption() {
	FILE *f = fopen(PATH_HELP_FILE, "r");
	if (f == NULL)
		errorBirthdayNotifier("The help file doesn't exist.");
	char c = fgetc(f);
	while (!feof(f)) {
		printf("%c", c);
		c = fgetc(f);
	}
	fclose(f);
}

bool isCommandHelp(int argc, char **argv) {
	return argc == 2 && (strcmp(argv[1], "-h") == 0 ||
						strcmp(argv[1], "--help") == 0);
}

void showVersion() {
	printf("Version: %s\n", VERSION);
}

bool isCommandShowVersion(int argc, char **argv) {
	return argc == 2 && (strcmp(argv[1], "-v") == 0 || 
						strcmp(argv[1], "--version") == 0);
}

int main(int argc, char **argv) {
	if (isCommandAddBirthday(argc, argv)) {
		addBirthday(argv);
	}
	else if (isCommandRemoveBirthday(argc, argv)) {
		removeBirthday(argv);
	}
	else if (isCommandShowListBirthdays(argc, argv)) {
		showListBirthdays();
	}
	else if (isCommandEnableNotifier(argc, argv)) {
		enableNotifier();
	}
	else if (isCommandDisableNotifier(argc, argv)) {
		disableNotifier();
	}
	else if (isCommandShowStatusNotifier(argc, argv)) {
		showStatusNotifier();
	}
	else if (isCommandCheckBirthdays(argc, argv)) {
		checkBirthdays();
	}
	else if (isCommandHelp(argc, argv)) {
		helpOption();
	}
	else if (isCommandShowVersion(argc, argv)) {
		showVersion();
	}
	else {
		printf("Error, wrong command.\n");
	}

	return 0;
}