# Aufgabe 3: Rekursive Fibonacci-Funktion

## b) Register und Stack bei Rekursion

Bei einem rekursiven Aufruf wird dieselbe Funktion mit neuen Argumenten gestartet. Der neue Aufruf verwendet dieselben Argumentregister wie sein Aufrufer und darf die caller-saved Register veraendern. Auf x86-64 mit der System-V-ABI gehoeren `rax`, `rcx`, `rdx`, `rsi`, `rdi` und `r8` bis `r11` dazu. Werte, die nach dem rekursiven Aufruf noch gebraucht werden, muessen daher vorher gesichert werden.

Die callee-saved Register `rbx`, `rbp` und `r12` bis `r15` muss der aufgerufene Code erhalten. Die Funktion `fibonacci` speichert `rbx` deshalb im Stackframe. Sie legt `n` dort ab, waehrend sie `f(n-1)` und `f(n-2)` berechnet. Das erste Teilergebnis `f(n-1)` kommt in einen lokalen Stackplatz, weil der zweite Aufruf `rax` ueberschreibt.

Vor einem `call` muss `rsp` gemaess der System-V-ABI 16-Byte-ausgerichtet sein. Die Funktion richtet den Stack nach dem Sichern von `rbp` und `rbx` mit acht Bytes lokalem Speicher passend aus.

`enter` richtet einen Stackframe ein. In der hier verwendeten einfachen Form entspricht es im Wesentlichen `push rbp` und `mov rbp, rsp`; es kann zusaetzlich lokalen Speicher reservieren. `leave` baut den Frame ab und entspricht `mov rsp, rbp` gefolgt von `pop rbp`. Die Loesung verwendet die ausformulierte Variante beim Aufbau und `leave` beim Abbau.

## c) Stack beim ersten rekursiven Aufruf fuer n = 3

Beim Aufruf `fibonacci(3)` wird zuerst der eigene Frame angelegt. Direkt vor dem ersten rekursiven `call fibonacci` ist `rdi` mit `2` geladen; `rsp` zeigt auf den reservierten lokalen Platz bei `rbp - 16`. Der `call` legt anschliessend die Ruecksprungadresse auf den Stack. Die Adressen wachsen in der Skizze nach oben:

```text
Hoehere Adressen
+----------------------------------+
| Ruecksprungadresse in main       |
+----------------------------------+
| Gespeichertes rbp von main       | <- rbp im Frame fibonacci(3)
+----------------------------------+
| Gespeichertes rbx = 3            | <- rbp - 8
+----------------------------------+
| Lokaler Platz / Ausrichtung      | <- rsp direkt vor dem call
+----------------------------------+
| Ruecksprungadresse nach call     | <- rsp im Eintritt von fibonacci(2)
+----------------------------------+
Niedrigere Adressen
```

Jeder weitere rekursive Aufruf legt einen eigenen Frame an und speichert sein `rbx`. So bleibt `n` fuer jeden Aufruf erhalten, obwohl alle Aufrufe dieselben Argumentregister verwenden. `rsp` bewegt sich mit jeder neuen Ruecksprungadresse und jedem Stackframe zu niedrigeren Adressen.
