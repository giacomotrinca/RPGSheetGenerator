# RPGSheetGenerator

<p align="center">
  <strong>🧙‍♂️ Generatore di Schede per Personaggi D&D 5e</strong>
</p>

<p align="center">
  Un'applicazione desktop moderna sviluppata in C++ con GTK4 e libadwaita per creare schede personaggio complete e professionali per Dungeons & Dragons 5ª Edizione.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C++-17-blue" alt="C++17">
  <img src="https://img.shields.io/badge/GTK-4.0-green" alt="GTK4">
  <img src="https://img.shields.io/badge/Platform-Linux-orange" alt="Linux">
  <img src="https://img.shields.io/badge/License-MIT-yellow" alt="License">
</p>

---

## 📖 Indice dei Contenuti

- [🎯 Introduzione](#-introduzione)
- [✨ Caratteristiche Principali](#-caratteristiche-principali)
- [📱 Screenshot](#-screenshot)
- [🛠️ Prerequisiti di Sistema](#️-prerequisiti-di-sistema)
- [⚙️ Installazione](#️-installazione)
- [🚀 Utilizzo](#-utilizzo)
- [🔧 Compilazione](#-compilazione)
- [📝 Funzionalità Dettagliate](#-funzionalità-dettagliate)
- [💾 Gestione dei Salvataggi](#-gestione-dei-salvataggi)
- [🔄 Changelog](#-changelog)
- [🤝 Contributi](#-contributi)
- [📄 Licenza](#-licenza)

---

## 🎯 Introduzione

**RPGSheetGenerator** è un'applicazione desktop nativa per Linux che semplifica e automatizza il processo di creazione dei personaggi per **Dungeons & Dragons 5ª Edizione**. 

L'applicazione offre un'interfaccia grafica moderna e intuitiva, costruita utilizzando le più recenti tecnologie **GTK4** e **libadwaita**, garantendo un'esperienza utente fluida e visivamente accattivante che si integra perfettamente con l'ambiente desktop GNOME.

### 🎲 Perché RPGSheetGenerator?

- **Automazione Completa**: Elimina i calcoli manuali e l'applicazione manuale delle regole
- **Interfaccia Professionale**: Design pulito e moderno senza elementi distraenti
- **Efficienza**: Creazione rapida di personaggi completi in pochi click
- **Accuratezza**: Implementazione fedele delle regole ufficiali D&D 5e
- **Portabilità**: Salvataggio e caricamento di personaggi in formato JSON

## ✨ Caratteristiche Principali

### 🧙‍♂️ Creazione Guidata
- **Processo Step-by-Step**: Interface guidata che accompagna l'utente in ogni fase
- **Validazione in Tempo Reale**: Controllo immediato della validità delle scelte
- **Anteprima Dinamica**: Visualizzazione istantanea dei modificatori e bonus

### 🎲 Metodi di Generazione Statistiche
- **Point Buy System**: Sistema ufficiale con 27 punti da distribuire
- **Standard Array**: Array fisso (15, 14, 13, 12, 10, 8)
- **Lancio Dadi**: Generazione casuale 4d6 scarta il più basso
- **Assegnazione Drag & Drop**: Riordino intuitivo delle statistiche

### 🏰 Database Completo
- **9 Razze Giocabili**: Umano, Elfo, Nano, Halfling, Dragonide, Tiefling, Gnomo, Mezzorco, Mezzelfo
- **12 Classi**: Tutte le classi del Player's Handbook
- **Backgrounds Diversificati**: Selezione completa di background con competenze
- **Bonus Razziali**: Applicazione automatica di tutti i modificatori razziali

### 🎯 Calcoli Automatici
- **Modificatori di Caratteristica**: Calcolo automatico da statistiche
- **Tiri Salvezza**: Competenze e bonus applicati automaticamente
- **Competenze**: Gestione completa delle skill proficiencies
- **Classe Armatura**: Calcolo base + bonus Destrezza
- **Punti Ferita**: Calcolo automatico basato su classe e Costituzione

### 🎨 Interfaccia Moderna
- **Design libadwaita**: Interfaccia nativa GNOME con look professionale
- **Layout Responsivo**: Ottimizzato per diverse risoluzioni
- **Schede Unificate**: Visualizzazione compatta di caratteristiche e competenze
- **Temi Adattivi**: Supporto automatico per tema chiaro/scuro

## 📱 Screenshot

```
[Qui andranno gli screenshot dell'applicazione]
```

## 🛠️ Prerequisiti di Sistema

### Sistemi Operativi Supportati
- **Linux** (distribuzione con GTK4)
- **Arch Linux** (raccomandato)
- **Ubuntu 22.04+**
- **Fedora 36+**

### Dipendenze Richieste
```bash
# Arch Linux
sudo pacman -S gtk4 libadwaita base-devel pkg-config

# Ubuntu/Debian
sudo apt-get install libgtk-4-dev libadwaita-1-dev build-essential pkg-config

# Fedora
sudo dnf install gtk4-devel libadwaita-devel gcc-c++ pkgconfig
```

## 💾 Gestione dei Salvataggi

### Formato di Salvataggio

I personaggi vengono salvati in formato **JSON** nella cartella `save/` con la seguente struttura:

```json
{
  "name": "Gray",
  "race": "Mezzelfo", 
  "subrace": "",
  "class": "Guerriero",
  "background": "Nobile",
  "gender": "Agender",
  "level": 1,
  "stats": [15, 13, 15, 9, 12, 12],
  "modifiers": [2, 1, 2, -1, 1, 1],
  "saving_throw_proficiencies": [true, false, true, false, false, false],
  "saving_throws": [4, 1, 4, -1, 1, 1],
  "skill_proficiencies": [false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, true],
  "proficiency_bonus": 2,
  "initiative": 1,
  "armor_class": 11,
  "hit_points": 10,
  "speed": 9
}
```

### Funzionalità di Salvataggio

- **Auto-save**: Salvataggio automatico durante la creazione
- **Caricamento**: Lista di personaggi esistenti all'avvio
- **Backup**: Backup automatico prima delle modifiche
- **Esportazione**: Possibilità di esportare in altri formati

## 🔄 Changelog

### Versione 2.0.0 (Corrente)
- ✅ **Interfaccia Rinnovata**: Layout unificato e professionale
- ✅ **Design Pulito**: Rimossi elementi emoji per look professionale  
- ✅ **Visualizzazione Migliorata**: Valori caratteristiche nel formato "14 +2" con box bianchi
- ✅ **Makefile Semplificato**: Rimosso codice di generazione immagini
- ✅ **Ottimizzazioni**: Performance migliorate e codice semplificato

### Versione 1.x.x (Precedente)
- ✅ **Funzionalità Base**: Creazione personaggi completa
- ✅ **Tre Metodi Generazione**: Point Buy, Standard Array, Roll Dice
- ✅ **Database Completo**: Tutte le razze e classi principali
- ✅ **Sistema Salvataggio**: Formato JSON con caricamento automatico

### Roadmap Futura
- 🔄 **Livelli Avanzati**: Supporto per personaggi multi-livello
- 🔄 **Sottoclassi**: Implementazione delle specializzazioni
- 🔄 **Equipaggiamento**: Sistema completo di gear e inventario
- 🔄 **Incantesimi**: Database completo di spell per classi magiche
- 🔄 **Esportazione PDF**: Generazione di schede stampabili

## 🛠️ Sviluppo

### Struttura del Progetto

```
RPGSheetGenerator/
├── src/
│   └── main.cpp          # Codice sorgente principale
├── save/                 # Cartella per salvataggi personaggi
│   ├── Gray.json        # Esempio di personaggio salvato
│   └── Shump.json
├── img/                  # Immagini e assets
├── Makefile             # Script di compilazione
├── README.md            # Documentazione
└── LICENSE              # Licenza MIT
```

### Architettura del Codice

- **GTK4/libadwaita**: Framework UI moderno e nativo
- **C++17**: Standard moderno con features avanzate
- **Pagine di Navigazione**: Flusso guidato step-by-step
- **JSON**: Serializzazione semplice e leggibile
- **Calcoli Automatici**: Logica D&D 5e implementata fedelmente

### Compilazione per Sviluppatori

```bash
# Clona il repository
git clone https://github.com/giacomotrinca/RPGSheetGenerator.git
cd RPGSheetGenerator

# Verifica dipendenze
make check-deps

# Compilazione debug
make debug

# Avvio in modalità debug
make run-debug

# Informazioni di compilazione
make info
```

### Contribuire al Progetto

1. **Fork** del repository
2. **Crea** un branch per la tua feature (`git checkout -b feature/AmazingFeature`)
3. **Commit** delle modifiche (`git commit -m 'Add some AmazingFeature'`)
4. **Push** al branch (`git push origin feature/AmazingFeature`)
5. **Apri** una Pull Request

### Linee Guida per Contributi

- **Code Style**: Segui lo stile C++ esistente
- **Documentazione**: Commenta le funzioni pubbliche
- **Testing**: Testa tutte le nuove funzionalità
- **Commit Messages**: Usa messaggi descrittivi e chiari

## 🐛 Segnalazione Bug

Se riscontri problemi o bug:

1. **Verifica** che sia un problema riproducibile
2. **Controlla** le [Issues esistenti](https://github.com/giacomotrinca/RPGSheetGenerator/issues)
3. **Crea** una nuova issue con:
   - Descrizione dettagliata del problema
   - Passi per riprodurre il bug
   - Informazioni di sistema (OS, versione GTK, etc.)
   - Screenshot se applicabili

## 📚 Risorse Aggiuntive

### Riferimenti D&D 5e
- [Player's Handbook](https://dnd.wizards.com/) - Regole ufficiali
- [D&D Beyond](https://www.dndbeyond.com/) - Strumenti ufficiali online
- [SRD 5.1](https://dnd.wizards.com/resources/systems-reference-document) - System Reference Document

### Documentazione Tecnica
- [GTK4 Documentation](https://docs.gtk.org/gtk4/) - Framework UI
- [libadwaita Documentation](https://gnome.pages.gitlab.gnome.org/libadwaita/) - Design system GNOME
- [C++ Reference](https://en.cppreference.com/) - Standard C++17

## 🤝 Contributi

I contributi sono benvenuti! Consulta il file [CONTRIBUTING.md](CONTRIBUTING.md) per le linee guida dettagliate.

### Contributori

- **[giacomotrinca](https://github.com/giacomotrinca)** - Creatore e maintainer principale

## 📄 Licenza

Questo progetto è rilasciato sotto licenza **MIT**. Vedi il file [LICENSE](LICENSE) per i dettagli completi.

```
MIT License

Copyright (c) 2025 Giacomo Trinca

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
