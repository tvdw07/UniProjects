#pragma once

#include <vector>
#include <chrono>


  // Klasse um Zeiten zu messen.
  //
  // Verwendung:
  // Die Memberfunktion start hält den aktuellen Zeitpunkt fest.
  // Die Memberfunktion stop hält einen zweiten, aktuellen Zeitpunkt fest.
  // Nachdem start und anschließend stopp aufgerufen wurde,
  // kann mit get_microseconds, ...  get_minutes, den Zeitraum zwischen start und stop
  // ausgeben.
  class Zeitmesser {
  public:
      Zeitmesser();
      void start();
      void stop();
      int get_microseconds();
      int get_milliseconds();
      int get_seconds();
      int get_minutes();
  private:
      // Unter der Haube wird std::chrono verwendet.
      // Hier werden die Typen alternativ benannt.
      using highres_clock = std::chrono::high_resolution_clock;
      using Zeitpunkt     = std::chrono::time_point<highres_clock>;
      using Dauer         = std::chrono::duration<int, std::ratio<1>>;

      // Start- und Stopzeitpunkt wird entsprechend gesichert.
      Zeitpunkt start_zeitpunkt;
      Zeitpunkt stop_zeitpunkt;
  };
