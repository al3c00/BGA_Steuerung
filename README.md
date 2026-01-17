# BGA_Steuerung

## Übersicht:

Dieses Repository enthält Platine-Daten und Software zur Steuerung und Überwachung der Substratflüsse einer landwirtschaftlichen Biogasanlage. Angesteuert werden elektrische Motoren (Pumpen und Mischmotoren) mitels Schützrelais sowie pneumatisch gesteuerte Schieber mit einem 24V 5/2 Pneumatikventil.

## Funktion:

## Schnittstellen der Steuerungsplatine:

+ 24 Digitale Inputs für NPN-Sensoren (Signal aktiv wenn auf Masse gezogen)
+ 6 Analoge Inputs (Stromschleife 4-20mA)
+ 2 Analoge Inputs (0-5v)
+ 16 Digitale Outputs
+ UART, SPI, I2C, drahtlose Kommunikationsmöglichkeiten des Raspberry Pi 4
  
## Steuerung der Anlage:
### Sequenzen für Steuerung:
Das Programm lädt Sequenzen, die zu einem bestimmten Zeitpunkt abgespielt werden sollen. Der Benutzer ist frei in der Programmierung der Sequenzen. Diese werden in einer .txt-Datei gespeichert und bei allfälligem Neustart des Programms wieder aufgerufen. Jede Funktion wird in einem eigenen Thread ausgeführt
Es können folgende Funktionen verwendet werden:
|Funktionsname|Beschrieb|Parameter_int1|Parameter_int2|Parameter_string1|Parameter_string2|
|---|---|---|---|---|---|
|WAIT|Stoppt die Ausführung der Sequenz indem der Thread mittels std::this_thread::sleep_for angehalten wird|Wartezeit in Millisekunden (ms)|-|-|-|
|JUMP_TO|Springt zu einer bestimmten Position in der Sequenz und fährt dort weiter|Zielposition (0 bis n)|-|-|-|
|PROGRESS_IF_1|Die Ausführung der Sequenz wird angehalten bis eine bestimmte Bedingung erfüllt ist an **einem** Input erfüllt ist|int_Zustand, der zu erfüllen ist|-|Input, der abgefragt wird|-|
|PROGRESS_IF_2|Die Ausführung der Sequenz wird angehalten bis eine bestimmte Bedingung erfüllt ist an **zwei** Inputs erfüllt ist (siehe HW_Con::getDoubleInputState|int_Zustand, der zu erfüllen ist|-|Input, der abgefragt wird|Input, der abgefragt wird|
|GET_DIGITAL_INPUT|Der aktuelle Zustand eines digitalen Inputs wird abgefragt|-|-|Input, der abgefragt wird|-|
|GET_DOUBLE_DIGITAL_INPUT|Die Funktion "getDoubleInputState" wird verwendet, um die Sensorwerte eines mechanischen Bauteils, dass Stellung An und Aus überwacht (nicht nur eine von beiden) zu überprüfen|-|-|Input1, der abgefragt wird|Input2, der abgefragt wird|
|GET_ANALOG_INPUT|Der aktuelle Zustand eines analogen Inputs wird abgefragt|-|-|Input, der abgefragt wird|-|
|SET_DIGITAL_OUTPUT|Der Zustand eines digitalen Outputs wird unabhängig von seinem aktuellen Zustand gesetzt|Zustand, der gesetzt werden soll (0/1, eigentlich Bool)|-|Output, der gesetzt werden soll|-|
|SWITCH_DIGITAL_OUTPUT|Der Zustand eines digitalen Outputs wird abhängig von seinem aktuellen Zustand ins Gegenteil gesetzt|-|-|Output, der gesetzt werden soll|-|
|NOT_DEFINED|Wird verwendet, um Fehler abzufangen und die struct zu konstruieren

Die Funktionen in der .txt-Datei nutzen folgende Syntax:
|Objekt|Beschrieb|
|#Sequencename|Name der Sequenz. Dieser wird im Quellcode für die Zuordnung an verschiedensten Stellen benutzt.| 
|{|Markiert den Beginn der Sequenz|
|Funktionsname(Parameter1, ev. Parameter2)|Siehe die oben beschriebenen Funktionen oder im Quellcode Sequencehandler.h|
|}Markiert das Ender der Sequenz|
Alle Sequenzen werden in derselben Datei gespeichert.


## Software:
### Allgemein:
Das Programm läuft auf einem Raspberry Pi 4, welcher mittel I2C mit den IO-Expandern auf der Platine kommuniziert. Die Kommunikation übernimmt eine Klasseninstanz von HW_Con. Die grafische Darstellung übernimmt die Klasse GUI, für z.B.unterschiedliche Ebenen können mehrere Instanzen erzeugt werden. Für das Rendering wird SDL2 genutzt. Es wird auf eine Abstrahierung des Quellcodes geachtet, viele Positionen der Elemente auf dem Display können in der dazugehörigen .txt-Datei angepasst und in der Software während Laufzeit neu geladen werden.

### GUI:
Es existiert mindestens eine Instaz der Klasse GUI. Damit können Bilder geladen und angezeigt und Text ausgegeben werden. Alle grafischen Elemente werden in einer std::map mit einem string-key referenziert. Es lassen sich vordefinierte Positionen in einem .txt-Dokument speichern und laden.

### HW_Con:
Von dieser Klasse darf nur eine Instanz erzeugt werden. Mittels I2C kommuniziert der PI mit den PCF8574 auf der Platine. Die einzelnen Inputs/Outputs sind mit ihren 7-Bit I2C Adressen der Chips sowie dem Port mit string-Namen in einer Map referenziert. *Beispiel: D_Out_0 --> dec35,p0.* Für jeden Input/Output existiert eine struct "IO_Object". Die Inputs werden im Main-Loop regelmässig neu abgefragt. Bei den Outputs wird der **Soll-Zustand** in dieser struct gespeichert und übermittelt. Der Ist-Zustand kann nicht abgefragt werden.

### Sequence_Handler:
Von dieser Klasse darf nur eine Instanz erzeugt werden. Sie lädt, speichert und organisert alle Sequenzen und überwacht deren Ausführung und gegebenenfalls Bearbeitung.


## Weiteres
Verbesserungspotenzial:
+ GND Pins bei I2C und UART Terminals vergessen
+ Sicherheit der digitalen Outputs verbessern (z.B.: blockieren der Stromversorgung aller Relais bis zum Zeitpunkt der explizit gewünschten Freigabe
+ Schöneres Layout der Platine
+ Aufbauposition des Raspberry optimieren
+ Pin Header besser zu Montageposition des RPI anpassen
+ Diverses mehr
