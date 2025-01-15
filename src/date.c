/**
 * MODULE: Date
 * FILE: date.c
 * VERSION: 1.0.0
 * HISTORICAL: Created by Eloy Urriens Arpal on 1/12/2025, version 1.0.0
 * DESCRIPTION: This module works with dates in format day/month/year.
 * CC: BY SA
 */

#include "date.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * FUNCTION: errorDate
 * INPUT: Any error string.
 * REQUIREMENTS: None.
 * OUTPUT: The program prints the error and exits.
 */
void errorDate(char *msgError) {
	char msg[512];
	snprintf(msg, sizeof(msg), "ERROR, in date.c: %s", msgError);
	perror(msg);
	exit(1);
}

bool isTheFuture(char day, char month, short year) {
	Date dateNow = getDateNow();
	if (year > getYear(dateNow))
		return true;
	if (year < getYear(dateNow))
		return false;

	if (month > getMonth(dateNow))
		return true;
	if (month < getMonth(dateNow))
		return false;

	if (day > getDay(dateNow))
		return true;
	if (day <= getDay(dateNow))
		return false;
}

/**
 * FUNCTION: formatStringDateToNumberDate
 * INPUT: A string.
 * REQUIREMENTS: The string must be "day/month/year"
 * OUTPUT: The day, month and year
 */
void formatStingDateToNumberDate(char *stringDate, char *day, char *month, short *year) {
	char copyDate[MAX_DATE_LENGTH];
	char *token;

	strcpy(copyDate, stringDate);
	token = strtok(copyDate, "/");
	*day = (char) (atoi(token));

	token = strtok(NULL, "/");
	*month = (char) (atoi(token));

	token = strtok(NULL, "/");
	*year = (short) (atoi(token));

	if (*day == 0 || month == 0 || year == 0)
		errorDate("Cannot convert date string to numeric date");
}

/**
 * FUNCTION: validateDate
 * INPUT: Any string.
 * REQUIREMENTS: None.
 * OUTPUT: True <=> String is valid.
 */
bool validateDate(char *stringDate) {
	int len = strlen(stringDate);
	if (len > MAX_DATE_LENGTH || stringDate[0] == '/' || stringDate[len - 1] == '/')
		return false;

	char i = 0;
	char counter = 0;
	while (i < len - 1 && (stringDate[i] >= '0' && stringDate[i] <= '9' || stringDate[i] == '/' || counter <= 2)) {
		if (stringDate[i] == '/')
			counter++;
		i++;
	}

	if (!(stringDate[i] >= '0' && stringDate[i] <= '9' || stringDate[i] == '/') || counter != 2)
		return false;

	char day;
	char month;
	short year;

	formatStingDateToNumberDate(stringDate, &day, &month, &year);
	if (day <= 0 || day > 31 || month <= 0 || month > 12 || isTheFuture(day, month, year))
		return false;

	return true;
}

Date newDate(char *stringDate) {
	if (!validateDate(stringDate))
		errorDate("Date isn't valid.");

    Date date;
	formatStingDateToNumberDate(stringDate, &date.day, &date.month, &date.year);

	return date;
}

char getDay(Date date) {
    return date.day;
}

char getMonth(Date date) {
    return date.month;
}

short getYear(Date date) {
    return date.year;
}

bool equalsDate(Date date1, Date date2) {
	return date1.day == date2.day && date1.month == date2.month && date1.year == date2.year;
}

void formatDateToStringDate(char stringDate[MAX_DATE_LENGTH], Date date) {
	char charDay[MAX_DATE_LENGTH];
	char charMonth[MAX_DATE_LENGTH];
	char charYear[MAX_DATE_LENGTH];

	sprintf(charDay, "%hhd", date.day);
	sprintf(charMonth, "%hhd", date.month);
	sprintf(charYear, "%hd", date.year);

	strcpy(stringDate, charDay);
	strcat(stringDate, "/");
	strcat(stringDate, charMonth);
	strcat(stringDate, "/");
	strcat(stringDate, charYear);
}

Date getDateNow() {
    Date date;
	time_t t = time(NULL);
	struct tm *timeNow = localtime(&t);

	date.year = (short) (timeNow->tm_year + 1900);
	date.month = (char) (timeNow->tm_mon + 1);
	date.day = (char) (timeNow->tm_mday);

    return date;
}

int calculateDifferenceInYears(Date date1, Date date2) {
	int years;

	years = getYear(date2) - getYear(date1);
	if (getMonth(date1) > getMonth(date2) || (getMonth(date1) == getMonth(date2) && getDay(date1) > getDay(date2)))
		years--;
    return years;
}
