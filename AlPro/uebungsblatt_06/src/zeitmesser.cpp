#include <zeitmesser.cpp>

#include <chrono>


Zeitmesser::Zeitmesser() {
}

void Zeitmesser::start() {
    start_zeitpunkt = std::chrono::high_resolution_clock::now();
}

void Zeitmesser::stop() {
    stop_zeitpunkt = std::chrono::high_resolution_clock::now();
}

int Zeitmesser::get_microseconds() {
    return std::chrono::duration_cast<std::chrono::microseconds>(stop_zeitpunkt - start_zeitpunkt).count();
}

int Zeitmesser::get_milliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(stop_zeitpunkt - start_zeitpunkt).count();
}

int Zeitmesser::get_seconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(stop_zeitpunkt - start_zeitpunkt).count();
}

int Zeitmesser::get_minutes() {
    return std::chrono::duration_cast<std::chrono::minutes>(stop_zeitpunkt - start_zeitpunkt).count();
}
