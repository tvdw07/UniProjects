//
// Created by Tim on 17.11.2025.
//
#pragma once

#include <vector>

/**
 * @class TowersOfHanoi
 * @brief Repräsentiert und löst das Spiel "Towers of Hanoi".
 *
 * Die Klasse verwaltet drei Türme (jeweils als std::vector<int>) und bietet
 * Methoden zum Aufsetzen, Prüfen, Ausgeben und Rekursiven Lösen des Spiels.
 */
class TowersOfHanoi {
private:
    /// Tower 1 (Quelle bei Start)
    std::vector<int> tower1;

    /// Tower 2 (Hilfsturm)
    std::vector<int> tower2;

    /// Tower 3 (Ziel bei Start)
    std::vector<int> tower3;

    /// Anzahl der Scheiben / Höhe der Türme
    int height{};

    /**
     * @brief Prüft, ob ein einzelner Turm in einem legalen Zustand ist.
     * @param arr Referenz auf den Turm als Vektor von Scheibengrößen.
     * @return true, falls keine größere Scheibe auf einer kleineren liegt.
     *
     * Diese Methode verändert keine Membervariablen.
     */
    bool is_legal(const std::vector<int> &arr) const;

    /**
     * @brief Gibt einen einzelnen Turm auf der Standardausgabe aus.
     * @param tower Referenz auf den Turm.
     * @param name Name des Turms (z.B. "Tower 1").
     *
     * Diese Methode verändert keine Membervariablen.
     */
    void print_tower(const std::vector<int> &tower, const char *name) const;

    /**
     * @brief Bewegt die oberste Scheibe von `from` nach `to`.
     * @param from Quellturm (wird verändert).
     * @param to Zielturm (wird verändert).
     * @return true, falls der Zug legal war; bei illegalem Zug wird der Zustand zurückgesetzt und false geliefert.
     */
    bool move_disk(std::vector<int> &from, std::vector<int> &to) const;

    /**
     * @brief Rekursive Hilfsmethode zum Lösen des Spiels.
     * @param source Quellturm.
     * @param target Zielturm.
     * @param helper Hilfsturm.
     * @param n Anzahl der zu bewegenden Scheiben.
     * @param print_steps Falls true, wird nach jedem Einzelzug der Spielzustand ausgegeben.
     * @return true im Erfolgsfall, false bei einem Fehler / illegalem Zug.
     */
    bool solve(std::vector<int> &source, std::vector<int> &target, std::vector<int> &helper,
               int n, bool print_steps);

public:
    /**
     * @brief Konstruktor mit Angabe der Turmhöhe.
     * @param tower_height Anzahl der Scheiben (>= 0).
     */
    explicit TowersOfHanoi(int tower_height);

    /**
     * @brief Setzt das Spiel mit gegebener Turmhöhe zurück / auf.
     * @param tower_height Neue Anzahl der Scheiben.
     */
    void setup_game(int tower_height);

    /**
     * @brief Prüft, ob der gesamte Spielzustand legal ist (alle Türme legal).
     * @return true, falls alle drei Türme legal sind.
     */
    bool is_game_state_legal() const;

    /**
     * @brief Gibt den aktuellen Spielzustand (alle drei Türme) aus.
     *
     * Diese Methode verändert keine Membervariablen.
     */
    void print_game_state() const;

    /**
     * @brief Startet das Lösen des Spiels (Quelle: Tower 1, Ziel: Tower 3).
     * @param print_steps Falls true, werden Zwischenzustände ausgegeben.
     * @return true, falls das Lösen erfolgreich war.
     */
    bool solve(bool print_steps = false);
};