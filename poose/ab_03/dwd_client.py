"""
REST-Client für die Crowdwettermeldungen des DWD.

Aufgabe:
1) Aktuelle Crowdwettermeldungen abrufen
2) Häufigkeit der Kategorien in `highestSeverities` ausgeben
3) Mitgelieferte Bild-Links der Melder:innen aus den Meldungen extrahieren und ausgeben

Hinweis:
- Der DWD dokumentiert den Crowd-Endpunkt als JSON-Datei.
- Die Struktur kann sich ändern, deshalb ist das Parsing bewusst defensiv geschrieben.

Prpmpt:
 Schreiben Sie einen REST-Client für die API des deutschen Wetterdienst https://dwd.api.bund.dev/.

Der REST-Client soll die aktuellen Crowdwettermeldungen auswerten. Dabei soll auf der Konsole

ausgedrucken werden

• mit welcher Häufigkeit die Kategorien der highestSeverities auftreten und

• welche Links zu mitgelieferten Bildern von Melder:innen angegeben wurden.

Bitte nutzte dazu python und die request libary. der code soll gut dokumentiert und nachvollziehbar sein

ChatGPT 5.4 Thinking mini

"""

from __future__ import annotations

import re
from collections import Counter
from typing import Any, Iterable
import requests
from requests.adapters import HTTPAdapter
from urllib3.util.retry import Retry


# Dokumentierter Crowd-Endpunkt laut DWD-API/OpenAPI-Repo
CROWD_URL = (
    "https://s3.eu-central-1.amazonaws.com/"
    "app-prod-static.warnwetter.de/v16/crowd_meldungen_overview_v2.json"
)

# URL-Erkennung für beliebige String-Felder
URL_RE = re.compile(r"https?://[^\s'\"<>]+", re.IGNORECASE)

# Dateiendungen, die sehr wahrscheinlich auf Bilder hindeuten
IMAGE_EXT_RE = re.compile(r"\.(?:png|jpe?g|gif|webp|bmp|tiff?)(?:\?.*)?$", re.IGNORECASE)

# Feldnamen-Hinweise: falls in verschachtelten Objekten solche Namen auftauchen,
# sind darin enthaltene URLs besonders wahrscheinlich Bild- oder Medienlinks.
URL_FIELD_HINTS = ("url", "urls", "bild", "image", "foto", "photo", "media")


def build_session() -> requests.Session:
    """
    Baut eine requests.Session mit Retries auf.

    Das ist nützlich, falls die API kurzfristig mit 5xx antwortet oder Verbindungen
    sporadisch fehlschlagen.
    """
    session = requests.Session()

    retries = Retry(
        total=3,
        connect=3,
        read=3,
        status=3,
        backoff_factor=0.5,
        status_forcelist=(429, 500, 502, 503, 504),
        allowed_methods=frozenset({"GET"}),
        raise_on_status=False,
    )

    adapter = HTTPAdapter(max_retries=retries)
    session.mount("https://", adapter)
    session.mount("http://", adapter)

    # Ein sprechender User-Agent ist bei APIs oft hilfreich.
    session.headers.update(
        {
            "User-Agent": "DWD-CrowdClient/1.0 (+https://example.local)",
            "Accept": "application/json",
        }
    )
    return session


def fetch_crowd_data(session: requests.Session) -> dict[str, Any]:
    """
    Ruft die Crowdwettermeldungen ab und gibt das JSON als Python-Objekt zurück.
    """
    response = session.get(CROWD_URL, timeout=20)
    response.raise_for_status()

    data = response.json()
    if not isinstance(data, dict):
        raise ValueError("Unerwartetes JSON-Format: Root-Element ist kein Objekt.")
    return data


def count_highest_severities(payload: dict[str, Any]) -> Counter[str]:
    """
    Zählt die Kategorien aus `highestSeverities`.

    Laut Schema enthält jedes Element mindestens:
    - category
    - auspraegung
    """
    counter: Counter[str] = Counter()
    severities = payload.get("highestSeverities", [])

    if not isinstance(severities, list):
        return counter

    for item in severities:
        if not isinstance(item, dict):
            continue
        category = str(item.get("category") or "unbekannt").strip() or "unbekannt"
        counter[category] += 1

    return counter


def _walk(value: Any, path: str = "root") -> Iterable[tuple[str, Any]]:
    """
    Rekursiver Walker über beliebig verschachtelte JSON-Strukturen.

    Liefert (Pfad, Wert)-Paare für alle Blattwerte.
    """
    if isinstance(value, dict):
        for key, subvalue in value.items():
            yield from _walk(subvalue, f"{path}.{key}")
    elif isinstance(value, list):
        for index, subvalue in enumerate(value):
            yield from _walk(subvalue, f"{path}[{index}]")
    else:
        yield path, value


def extract_image_links_from_meldung(meldung: dict[str, Any]) -> list[str]:
    """
    Extrahiert URL-Strings aus einer einzelnen Meldung.

    Strategie:
    - Rekursiv alle Stringwerte durchsuchen
    - URLs bevorzugen, wenn der Pfad nach URL/Bild/Media aussieht
    - zusätzlich URLs mit typischer Bild-Endung aufnehmen

    Das ist bewusst defensiv, weil das DWD-Schema für `zusatzAttribute`
    keine festen Felder für Bilder dokumentiert.
    """
    links: set[str] = set()

    for path, value in _walk(meldung):
        if not isinstance(value, str):
            continue

        # Ein String kann mehrere URLs enthalten.
        candidates = URL_RE.findall(value)
        if not candidates and value.startswith("http"):
            candidates = [value]

        path_lower = path.lower()

        for url in candidates:
            cleaned = url.rstrip(").,;]}>\"'")

            looks_like_image = bool(IMAGE_EXT_RE.search(cleaned))
            path_suggests_media = any(hint in path_lower for hint in URL_FIELD_HINTS)

            if looks_like_image or path_suggests_media:
                links.add(cleaned)

    return sorted(links)


def print_results(payload: dict[str, Any]) -> None:
    """
    Gibt die gewünschte Auswertung auf der Konsole aus.
    """
    print("=== Häufigkeit der Kategorien in highestSeverities ===")
    severity_counts = count_highest_severities(payload)

    if not severity_counts:
        print("Keine highestSeverities gefunden.")
    else:
        for category, count in severity_counts.most_common():
            print(f"{category}: {count}")

    print("\n=== Mitgelieferte Bild-Links aus den Meldungen ===")
    meldungen = payload.get("meldungen", [])

    if not isinstance(meldungen, list) or not meldungen:
        print("Keine Meldungen gefunden.")
        return

    all_links: set[str] = set()

    for meldung in meldungen:
        if not isinstance(meldung, dict):
            continue

        meldung_id = meldung.get("meldungId", "?")
        place = meldung.get("place", "unbekannter Ort")
        links = extract_image_links_from_meldung(meldung)

        if not links:
            continue

        print(f"\nMeldung {meldung_id} ({place}):")
        for link in links:
            print(f"  - {link}")
            all_links.add(link)

    if not all_links:
        print("Keine Bild-Links gefunden.")


def main() -> None:
    session = build_session()

    try:
        payload = fetch_crowd_data(session)
        print_results(payload)
    except requests.HTTPError as exc:
        print(f"HTTP-Fehler beim Abrufen der DWD-Daten: {exc}")
    except requests.RequestException as exc:
        print(f"Netzwerkfehler beim Abrufen der DWD-Daten: {exc}")
    except ValueError as exc:
        print(f"Datenfehler: {exc}")


if __name__ == "__main__":
    main()