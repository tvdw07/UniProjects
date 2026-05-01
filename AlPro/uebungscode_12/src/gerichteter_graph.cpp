#include <simpler_gerichteter_graph/gerichteter_graph.hpp>
#include <queue>
#include <stack>

#include <iostream>

namespace SimplerGerichteterGraph {
  // Naive Hashfunktion für Knoten
  size_t KnotenHash::operator()(const Knoten& k) const {
    return k.get_index_t();
  }

  // Naive Hashfunktion für Kanten (Hash(Start) XOR Hash(Ziel))
  size_t KantenHash::operator()(const Kante& k) const {
    return k.get_startknoten().get_index_t() ^ k.get_zielknoten().get_index_t();
  }

  GerichteterGraph::GerichteterGraph()
  {}

  void GerichteterGraph::knoten_einfuegen(const Knoten& k) {
    knotenmenge.emplace(k);
  }

  void GerichteterGraph::knoten_einfuegen(const index_t& idx) {
    knoten_einfuegen(Knoten(idx));
  }

  void GerichteterGraph::knoten_entfernen(const Knoten& k) {
    knotenmenge.erase(k);
    // Entferne alle Kanten, die k enthalten

    // Entferne alle auslaufenden Kanten
    if (auslaufendekanten.count(k) > 0) {
      for (const auto& e : auslaufendekanten[k]) {
        kantenmenge.erase(e);
      }
      auslaufendekanten.erase(k);
    }

    // Entferne alle einlaufenden Kanten
    if (einlaufendekanten.count(k) > 0) {
      for (const auto& e : einlaufendekanten[k]) {
        kantenmenge.erase(e);
      }
      einlaufendekanten.erase(k);
    }
  }

  void GerichteterGraph::knoten_entfernen(const index_t& idx) {
    knoten_entfernen(Knoten(idx));
  }

  bool GerichteterGraph::existiert_knoten(const Knoten& k) const {
    return knotenmenge.count(k) > 0;
  }

  bool GerichteterGraph::existiert_knoten(const index_t& idx) const {
    return existiert_knoten(Knoten(idx));
  }

  const Knotenmenge& GerichteterGraph::get_knotenmenge() const {
    return knotenmenge;
  }

  void GerichteterGraph::kante_einfuegen(const Kante& k) {
    const auto& startknoten = k.get_startknoten();
    const auto& zielknoten = k.get_zielknoten();
    knoten_einfuegen(startknoten);
    knoten_einfuegen(zielknoten);

    // Wir wollen keine Schlaufen
    if(not (startknoten == zielknoten)) {
      kantenmenge.emplace(k);
      auslaufendekanten[startknoten].emplace(k);
      einlaufendekanten[zielknoten].emplace(k);
    }
  }

  void GerichteterGraph::kante_einfuegen(const index_t& start, const index_t& ziel) {
    kante_einfuegen(Kante(start, ziel));
  }

  void GerichteterGraph::kante_entfernen(const Kante& k) {
    kantenmenge.erase(k);
    auslaufendekanten[k.get_startknoten()].erase(k);
    einlaufendekanten[k.get_zielknoten()].erase(k);
  }

  void GerichteterGraph::kante_entfernen(const index_t& start, const index_t& ziel) {
    kante_entfernen(Kante(start, ziel));
  }

  bool GerichteterGraph::existiert_kante(const Kante& k) const {
    return kantenmenge.count(k) > 0;
  }

  bool GerichteterGraph::existiert_kante(const index_t& start, const index_t& ziel) const {
    return existiert_kante(Kante(start, ziel));
  }

  Kantenmenge GerichteterGraph::get_auslaufende(const Knoten& start) const {
    if (auslaufendekanten.count(start) > 0) {
      return auslaufendekanten.at(start);
    } else {
      return Kantenmenge();
    }
  }

  Kantenmenge GerichteterGraph::get_auslaufende(const index_t& start) const {
    return get_auslaufende(Knoten(start));
  }

  Kantenmenge GerichteterGraph::get_einlaufende(const Knoten& start) const {
    if (einlaufendekanten.count(start) > 0) {
      return einlaufendekanten.at(start);
    } else {
      return Kantenmenge();
    }
  }

  Kantenmenge GerichteterGraph::get_einlaufende(const index_t& start) const {
    return get_einlaufende(Knoten(start));
  }


  void GerichteterGraph::drucke() const {
    std::cout << "Knoten: ";
    for (const auto& k : knotenmenge) {
      std::cout << k.get_string() << " ";
    }
    std::cout << std::endl;


    std::cout << "Kanten: ";
    for (const auto& k : kantenmenge) {
      std::cout << k.get_string() << " ";
    }
    std::cout << std::endl;

  }

  std::vector<Knoten> GerichteterGraph::breitensuche(const Knoten& start) const {
    std::vector<Knoten> reihenfolge;
    if (!existiert_knoten(start)) {
      return reihenfolge;
    }

    std::queue<Knoten> offene_knoten;
    Knotenmenge besucht;
    offene_knoten.push(start);
    besucht.emplace(start);

    while (!offene_knoten.empty()) {
      const auto aktueller = offene_knoten.front();
      offene_knoten.pop();
      reihenfolge.push_back(aktueller);

      for (const auto& kante : get_auslaufende(aktueller)) {
        const auto& nachbar = kante.get_zielknoten();
        if (besucht.emplace(nachbar).second) {
          offene_knoten.push(nachbar);
        }
      }
    }

    return reihenfolge;
  }

  std::vector<Knoten> GerichteterGraph::tiefensuche(const Knoten& start) const {
    std::vector<Knoten> reihenfolge;
    if (!existiert_knoten(start)) {
      return reihenfolge;
    }

    std::stack<Knoten> offene_knoten;
    Knotenmenge besucht;
    offene_knoten.push(start);
    besucht.emplace(start);

    while (!offene_knoten.empty()) {
      const auto aktueller = offene_knoten.top();
      offene_knoten.pop();
      reihenfolge.push_back(aktueller);

      for (const auto& kante : get_auslaufende(aktueller)) {
        const auto& nachbar = kante.get_zielknoten();
        if (besucht.emplace(nachbar).second) {
          offene_knoten.push(nachbar);
        }
      }
    }

    return reihenfolge;
  }

}
