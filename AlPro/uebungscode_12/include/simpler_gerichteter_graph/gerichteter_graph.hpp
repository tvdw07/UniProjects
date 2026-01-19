#pragma once

#include <simpler_gerichteter_graph/knoten.hpp>
#include <simpler_gerichteter_graph/kante.hpp>

#include <cstddef>
#include <vector>
#include <unordered_set>
#include <unordered_map>


namespace SimplerGerichteterGraph {

// Knotenmenge mit hashender Menge
class KnotenHash {
public:
  size_t operator() (const Knoten& k) const;
};

using Knotenmenge = std::unordered_set<Knoten, KnotenHash>;


// Kantenmenge mit hashender Menge
class KantenHash {
public:
  size_t operator() (const Kante& k) const;
};

using Kantenmenge = std::unordered_set<Kante, KantenHash>;

// Pro Knoten: Menge der auslaufenden / einlaufenden KantenHash
using AuslaufendeKanten = std::unordered_map<Knoten, Kantenmenge, KnotenHash>;
using EinlaufendeKanten = std::unordered_map<Knoten, Kantenmenge, KnotenHash>;


// Klasse Gerichteter Graph
class GerichteterGraph {
public:
  // Konstruktur
  GerichteterGraph();

  // Existierenden Knoten hinzufügen
  void knoten_einfuegen(const Knoten& k);

  // Knoten erstellen und hinzufügen
  void knoten_einfuegen(const index_t& idx);

  // Knoten entfernen (durch Angabe eines existierenden Knoten)
  void knoten_entfernen(const Knoten& k);

  // Knoten entfernen (durch Angabe eines Index)
  void knoten_entfernen(const index_t& idx);

  // Fragt an ob existierender Knoten enthalten ist
  bool existiert_knoten(const Knoten& k) const;

  // Fragt an ob Knoten zu gegebenem Index enthalten ist
  bool existiert_knoten(const index_t& k) const;

  // Gibt Referenz auf enthaltene Knotenmenge zurück
  const Knotenmenge& get_knotenmenge() const;

  // Existierende Kante hinzufügen
  void kante_einfuegen(const Kante& k);

  // Kante erstellen und hinzufügen
  void kante_einfuegen(const index_t& start, const index_t& ziel);

  // Kante entfernen (durch Angabe einer existierenden Kante)
  void kante_entfernen(const Kante& k);

  // Knoten entfernen (durch Angabe eines Indexpaars)
  void kante_entfernen(const index_t& start, const index_t& ziel);

  // Fragt an ob existierende Kante enthalten ist
  bool existiert_kante(const Kante& k) const;

  // Fragt an ob Kante zu gegebenem Indexpaar enthalten ist
  bool existiert_kante(const index_t& start, const index_t& ziel) const;

  // Gibt Referenz auf enthaltene auslaufende Kantenmenge zurück
  Kantenmenge get_auslaufende(const Knoten& start) const;

  // Gibt Referenz auf enthaltene auslaufende Kantenmenge zurück
  Kantenmenge get_auslaufende(const index_t& start) const;

  // Gibt Referenz auf enthaltene einlaufende Kantenmenge zurück
  Kantenmenge get_einlaufende(const Knoten& start) const;

  // Gibt Referenz auf enthaltene einlaufende Kantenmenge zurück
  Kantenmenge get_einlaufende(const index_t& start) const;

  // Druckt Graphen aus
  void drucke() const;

  // Breitensuche
  std::vector<Knoten> breitensuche(const Knoten& start) const;

  // Tiefensuche
  std::vector<Knoten> tiefensuche(const Knoten& start) const;

private:
  Knotenmenge knotenmenge;
  Kantenmenge kantenmenge;
  AuslaufendeKanten auslaufendekanten;
  EinlaufendeKanten einlaufendekanten;
};

}
