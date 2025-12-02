# BGA_Steuerung

## Übersicht:

Dieses Repository enthält Platine-Daten und Software zur Steuerung und Überwachung der Substratflüsse einer landwirtschaftlichen Biogasanlage. 

## Funktion:
### Biogasanlage allgemein:
Auf dem Landwirtschaftsbetrieb wird eine Biogasanalge des Types Haral-Energie geplant. Diese zeichnet sich durch den eckigen, unterirdischen Fermenter aus. Das gewonnene Gas wird in einem Blockheizkraftwerk verbrannt, wobei Wärme und im gekoppelten Generator Strom entsteht. Die Wärme wird für die Versorgung des Landwirtschaftsbetriebs genutzt, der Strom wird in das öffentliche Netz eingespiesen.
Die Anlage besteht aus einer Vorgrube (Mischen der Substrate), einem Fermenter (Methangaserzeugung) und einem Endlager. 
Im nachfolgenden Diagramm wird die Funktionsweise der Anlage schematisch dargestellt:
```mermaid
graph TD;
  Güllegrube Schweine -->Vorgrube;
  Güllegrube Rinder-->Vorgrube;
  Feststoffmischer-->Vorgrube;
  Vorgrube-->Fermenter;
  Fermenter-->Endlager;
  Endlager-->Probenahme;
  Endlager-->Vorgrube;
  Endlager-->Betrieb;
  Endlager-->Nachbarbetrieb;
  Endlager-->Fassbefüllung;
```
