#pragma once

#include <vector>

/**
 * SimplerGraph - Eine Klasse für gerichtete Graphen
 *
 * Verwendet eine Adjazenzmatrix (bool-Array) zur Darstellung des Graphen.
 * - Knoten werden durch Indizes 0, 1, ..., n-1 benannt
 * - Kante von i nach j wird durch Matrix[i][j] = true repräsentiert
 */
class SimplerGraph {
private:
    int anzahlKnoten;                          // Anzahl der Knoten (fest)
    std::vector<std::vector<bool>> adjazenzmatrix;  // 2D bool-Array für Kanten

public:
    /**
     * Konstruktor: Erstellt einen Graphen mit der angegebenen Knotenanzahl
     * @param n Anzahl der Knoten (fest, nicht veränderbar)
     */
    explicit SimplerGraph(int n);

    /**
     * Fügt eine gerichtete Kante von Knoten 'von' zu Knoten 'nach' hinzu
     * @param von Startknoten der Kante
     * @param nach Zielknoten der Kante
     * @return true wenn erfolgreich, false bei ungültigen Knoten
     */
    bool kanteHinzufuegen(int von, int nach);

    /**
     * Entfernt die gerichtete Kante von Knoten 'von' zu Knoten 'nach'
     * @param von Startknoten der Kante
     * @param nach Zielknoten der Kante
     * @return true wenn erfolgreich, false bei ungültigen Knoten
     */
    bool kanteEntfernen(int von, int nach);

    /**
     * Prüft, ob eine Kante zwischen zwei Knoten existiert
     * @param von Startknoten
     * @param nach Zielknoten
     * @return true wenn Kante existiert, false sonst
     * const - verändert den Graphen nicht
     */
    bool hatKante(int von, int nach) const;

    /**
     * Gibt die Anzahl der Knoten zurück
     * @return Anzahl der Knoten
     * const - verändert den Graphen nicht
     */
    int getAnzahlKnoten() const;

    /**
     * Gibt alle Knoten und Kanten des Graphen aus
     * const - verändert den Graphen nicht
     */
    void ausdrucken() const;

    /**
     *Führt eine Breite-First-Suche (BFS) ab einem Startknoten durch und gibt die besuchten Knoten in der Reihenfolge ihrer Entdeckung aus.
     *@param startKnoten Der Knoten, von dem die Suche gestartet wird.
     *@return Ein Vektor der besuchten Knoten in der Reihenfolge ihrer Entdeckung.
     *const - verändert den Graphen nicht
     */
    std::vector<int> BFS(int startKnoten) const;

    /**
     *Führt eine Tiefen-First-Suche (DFS) ab einem Startknoten durch und gibt die besuchten Knoten in der Reihenfolge ihrer Entdeckung aus.
     *@param startKnoten Der Knoten, von dem die Suche gestartet wird.
     *@return Ein Vektor der besuchten Knoten in der Reihenfolge ihrer Entdeckung.
     *const - verändert den Graphen nicht
     */
    std::vector<int> DFS(int startKnoten) const;
private:
    /**
     * Hilfsfunktion: Prüft ob ein Knoten gültig ist
     * @param knoten Der zu prüfende Knoten
     * @return true wenn gültig (0 <= knoten < anzahlKnoten)
     * const - verändert den Graphen nicht
     */
    bool istGueltigerKnoten(int knoten) const;

    /**
     * Rekursive Hilfsfunktion für DFS
     * @param knoten Der aktuell besuchte Knoten
     * @param besucht Vektor der bereits besuchten Knoten
     * @param besuchteKnoten Vektor der besuchten Knoten in der Reihenfolge
     */
    void DFS_rekursiv(int knoten, std::vector<bool>& besucht, std::vector<int>& besuchteKnoten) const;
};


