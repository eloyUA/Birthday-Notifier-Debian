#ifndef _DATE_H
#define _DATE_H

/**
 * MODULE: Date
 * FILE: date.h
 * VERSION: 1.0.0
 * HISTORICAL: Created by Eloy Urriens Arpal on 1/12/2025, version 1.0.0
 * DESCRIPTION: This module works with dates in format day/month/year.
 * CC: BY SA
 */

#include <stdbool.h>

#define MAX_DATE_LENGTH 13 // "dd/mm/yyyy"
typedef struct {
    char day;
    char month;
    short year;
} Date;

/**
 * FUNCTION: newDate
 * INPUT: A date (day/month/year)
 * REQUIREMENTS: None, any string.
 * OUTPUT:
 *      A Date if string is valid.
 *      If string isn't valid, the program exit with error.
 */
Date newDate(char *);

/**
 * FUNCTION: getDay
 * INPUT: A date.
 * REQUIREMENTS: None.
 * OUTPUT: A number [1..31]
 */
char getDay(Date date);

/**
 * FUNCTION: getMonth
 * INPUT: A date.
 * REQUIREMENTS: None.
 * OUTPUT: A number [1..12]
 */
char getMonth(Date date);

/**
 * FUNCTION: getYear
 * INPUT: A date.
 * REQUIREMENTS: None.
 * OUTPUT: The year.
 */
short getYear(Date date);

/**
 * FUNCTION: equalsDate
 * INPUT: Two dates.
 * REQUIREMENTS: None.
 * OUTPUT: True <=> The dates are equals.
 */
bool equalsDate(Date, Date);

/**
 * FUNCTION: formatDateToStringDate
 * INPUT: A string and date.
 * REQUIREMENTS: char string[MAX_DATE_LENGTH] (The string already must be reserved)
 * OUTPUT: In the string, the date in format day/month/year
 */
void formatDateToStringDate(char *, Date);

/**
 * FUNCTION: getDateNow
 * INPUT: None
 * REQUIREMENTS: None.
 * OUTPUT: Today's date.
 */
Date getDateNow();

/**
 * FUNCTION: calculateDifferenceInYears
 * INPUT: date1 and date2
 * REQUIREMENTS: date1 <= date2
 * OUTPUT:
 *      The difference in years. Examples:
 *          Example 1:
 *              date1 = 12/12/2005
 *              date2 = 13/12/2025
 *              difference = 20 years
 *          Example 2:
 *              date1 = 12/12/2005
 *              date2 = 12/12/2025
 *              difference = 20 years
 *          Example 3:
 *              date1 = 12/12/2005
 *              date2 = 11/12/2025
 *              difference = 19 years
 */
int calculateDifferenceInYears(Date, Date);

#endif
