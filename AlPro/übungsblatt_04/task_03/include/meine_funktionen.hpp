#pragma once
#ifndef __MEINE_FUNKTIONEN_HPP__
#define __MEINE_FUNKTIONEN_HPP__


// Gute Kommentare hinzufügen

/* Berechnet den Sinus von x
 *@param x Der Winkel in Radiant
 *@return Der Sinuswert von x
 */
double sinus(double x);

/* Berechnet den Sinus von x mit gegebener Toleranz
 *@param x Der Winkel in Radiant
 *@param toleranzwert Der akzeptable Fehlerbereich
 *@return Der Sinuswert von x
 */
double sinus(double x, double toleranzwert);

/* Berechnet den Kosinus von x
 *@param x Der Winkel in Radiant
 *@return Der Kosinuswert von x
 */
double kosinus(double x);

/* Berechnet den Kosinus von x mit gegebener Toleranz
 *@param x Der Winkel in Radiant
 *@param toleranzwert Der akzeptable Fehlerbereich
 *@return Der Kosinuswert von x
 */
double kosinus(double x, double toleranzwert);

/* Berechnet den Tangens von x
 *@param x Der Winkel in Radiant
 *@return Der Tangenswert von x
 */
double tangens(double x);

/* Berechnet den Tangens von x mit gegebener Toleranz
 *@param x Der Winkel in Radiant
 *@param toleranzwert Der akzeptable Fehlerbereich
 *@return Der Tangenswert von x
 */
double tangens(double x, double toleranzwert);

/* Berechnet den Kotangens von x
 *@param x Der Winkel in Radiant
 *@return Der Kotangenswert von x
 */
double kotangens(double x);

/* Berechnet den Kotangens von x mit gegebener Toleranz
 *@param x Der Winkel in Radiant
 *@param toleranzwert Der akzeptable Fehlerbereich
 *@return Der Kotangenswert von x
 */
double kotangens(double x, double toleranzwert);


#endif
