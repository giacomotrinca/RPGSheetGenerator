# RPGSheetGenerator

<p align="center">
  <strong>Un generatore di schede per personaggi di Dungeons & Dragons 5ª Edizione</strong>
</p>

<p align="center">
  Un'applicazione desktop moderna sviluppata in C++ con GTK4 e libadwaita per creare schede personaggio complete e professionali per D&D 5e.
</p>

---

## 📖 Indice dei Contenuti

- [Introduzione](#-introduzione)
- [Caratteristiche Principali](#-caratteristiche-principali)
- [Funzionalità Dettagliate](#-funzionalità-dettagliate)
  - [Selezione Razza e Classe](#selezione-razza-e-classe)
  - [Generazione delle Statistiche](#generazione-delle-statistiche)
  - [Gestione delle Competenze](#gestione-delle-competenze)
  - [Background del Personaggio](#background-del-personaggio)
- [Prerequisiti di Sistema](#-prerequisiti-di-sistema)
- [Installazione](#-installazione)
- [Utilizzo](#-utilizzo)
- [Sviluppo](#-sviluppo)
- [Contributi](#-contributi)
- [Licenza](#-licenza)

---

## 🎯 Introduzione

RPGSheetGenerator è un'applicazione desktop nativa per Linux che semplifica e automatizza il processo di creazione dei personaggi per Dungeons & Dragons 5ª Edizione. L'applicazione offre un'interfaccia grafica moderna e intuitiva, costruita utilizzando le più recenti tecnologie GTK4 e libadwaita, garantendo un'esperienza utente fluida e visivamente accattivante.

Il progetto nasce dalla necessità di avere uno strumento efficiente e user-friendly per la gestione dei personaggi D&D, eliminando i calcoli manuali e automatizzando l'applicazione delle regole del gioco.

## ✨ Caratteristiche Principali

- 🧙‍♂️ **Creazione Guidata Completa**: Processo step-by-step per la creazione del personaggio
- 🎲 **Tre Metodi di Generazione Statistiche**: Point Buy, Standard Array e lancio dadi
- 🏰 **Database Completo**: Tutte le razze, classi e background del Player's Handbook
- 🎯 **Calcoli Automatici**: Modificatori, tiri salvezza e competenze calcolati automaticamente
- 🎨 **Interfaccia Moderna**: Design pulito e responsivo con libadwaita
- ⚡ **Performance Ottimizzate**: Applicazione nativa C++ veloce e leggera
- 🐧 **Compatibilità Linux**: Ottimizzato per ambienti desktop GNOME

## 🔧 Funzionalità Dettagliate

### Selezione Razza e Classe

L'applicazione supporta tutte le razze e classi principali di D&D 5e:

**Razze Disponibili:**
- Umano (Normale e Variante)
- Elfo (Alto Elfo, Elfo dei Boschi, Drow)
- Halfling (Piedelesto, Tozzo)
- Nano (delle Montagne, delle Colline)
- Dragonide
- Gnomo (delle Rocce, delle Foreste)
- Mezzelfo
- Mezzorco
- Tiefling

**Classi Disponibili:**
- Barbaro, Bardo, Chierico, Druido
- Guerriero, Ladro, Mago, Monaco
- Paladino, Ranger, Stregone, Warlock

### Generazione delle Statistiche

#### Point Buy System
- Budget di 27 punti da distribuire
- Costi variabili basati sul valore (8-15 punti)
- Calcolo automatico dei costi rimanenti
- Validazione in tempo reale

#### Standard Array
- Valori predefiniti: 15, 14, 13, 12, 10, 8
- Assegnazione tramite dropdown intuitivi
- Prevenzione duplicati automatica

#### Lancio Dadi (4d6 drop lowest)
- Generazione casuale con il metodo classico
- Interfaccia drag-and-drop per l'assegnazione
- Possibilità di rigenerare i valori

### Gestione delle Competenze

- **Tiri Salvezza**: Assegnazione automatica basata sulla classe
- **Competenze Abilità**: Selezione guidata con limiti di classe
- **Competenze Background**: Applicazione automatica
- **Calcolo Modificatori**: Aggiornamento dinamico in base alle statistiche

### Background del Personaggio

Supporto completo per i background standard:
- Accolito, Criminale, Eremita, Eroe Popolare
- Nobile, Sapiente, Soldato, Monello
- Ogni background applica automaticamente le competenze appropriate

## 🛠️ Prerequisiti di Sistema

### Dipendenze Richieste

**Sistema Operativo**: Linux (testato su Ubuntu 20.04+, Fedora 35+)

**Librerie di Sviluppo**:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential pkg-config libgtk-4-dev libadwaita-1-dev

# Fedora/RHEL
sudo dnf install gcc-c++ pkg-config gtk4-devel libadwaita-devel

# Arch Linux
sudo pacman -S base-devel gtk4 libadwaita
```

**Tool di Build**:
- GCC 9.0+ o Clang 10.0+
- pkg-config
- make (opzionale)

## 🚀 Installazione

### Metodo 1: Compilazione Diretta

1. **Clona il repository:**
   ```bash
   git clone https://github.com/giacomotrinca/RPGSheetGenerator.git
   cd RPGSheetGenerator
   ```

2. **Compila ed esegui:**
   ```bash
   chmod +x run
   ./run
   ```

### Metodo 2: Compilazione Manuale

```bash
# Compila l'applicazione
g++ ./src/main.cpp -o dnd_generator $(pkg-config --cflags --libs gtk4 libadwaita-1)

# Esegui l'applicazione
./dnd_generator
```

### Metodo 3: Installazione Sistema (Opzionale)

```bash
# Copia il binario in una directory del PATH
sudo cp dnd_generator /usr/local/bin/

# Ora puoi eseguire da qualsiasi directory
dnd_generator
```

## 🎮 Utilizzo

### Avvio dell'Applicazione

1. **Lancio**: Esegui `./dnd_generator` o usa lo script `./run`
2. **Interfaccia**: L'applicazione si aprirà con una finestra guidata a schede

### Processo di Creazione Personaggio

1. **Scheda Iniziale**: Inserisci nome del personaggio e informazioni base
2. **Selezione Razza**: Scegli razza e sottorazza dal menu dropdown
3. **Selezione Classe**: Seleziona la classe del personaggio
4. **Generazione Statistiche**: Scegli il metodo preferito:
   - **Point Buy**: Distribuisci 27 punti tra le caratteristiche
   - **Standard Array**: Assegna i valori predefiniti
   - **Lancio Dadi**: Genera e assegna valori casuali
5. **Background**: Seleziona il background del personaggio
6. **Competenze**: Verifica e personalizza le competenze automatiche
7. **Finalizzazione**: Rivedi e salva la scheda completata

### Caratteristiche dell'Interfaccia

- **Navigazione Intuitiva**: Barra di progresso che mostra i passaggi completati
- **Validazione Real-time**: Controlli immediati su valori e selezioni
- **Calcoli Automatici**: Tutti i modificatori e bonus aggiornati istantaneamente
- **Anteprima Live**: Visualizzazione in tempo reale delle statistiche finali

## 🔨 Sviluppo

### Struttura del Progetto

```
RPGSheetGenerator/
├── src/
│   └── main.cpp          # Codice sorgente principale
├── README.md             # Documentazione
├── LICENSE               # Licenza del progetto
├── run                   # Script di build e avvio
└── dnd_generator         # Eseguibile compilato
```

### Tecnologie Utilizzate

- **Linguaggio**: C++17
- **GUI Framework**: GTK4
- **Design System**: libadwaita
- **Build System**: GCC con pkg-config
- **Gestione Memoria**: RAII e smart pointers

### Compilazione per Sviluppatori

```bash
# Debug build con simboli
g++ -g -DDEBUG ./src/main.cpp -o dnd_generator_debug $(pkg-config --cflags --libs gtk4 libadwaita-1)

# Release build ottimizzata
g++ -O3 -DNDEBUG ./src/main.cpp -o dnd_generator $(pkg-config --cflags --libs gtk4 libadwaita-1)

# Con warning estesi
g++ -Wall -Wextra -Wpedantic ./src/main.cpp -o dnd_generator $(pkg-config --cflags --libs gtk4 libadwaita-1)
```

### Estensioni Future

- [ ] Supporto per salvataggio/caricamento schede
- [ ] Export in PDF
- [ ] Supporto multi-lingua
- [ ] Integrazione con API D&D Beyond
- [ ] Supporto per contenuti aggiuntivi (Xanathar's, Tasha's, etc.)
- [ ] Modalità campagna per gestire multiple schede

## 🤝 Contributi

I contributi sono molto graditi! Ecco come puoi contribuire:

### Come Contribuire

1. **Fork** il repository
2. **Crea** un branch per la tua feature (`git checkout -b feature/AmazingFeature`)
3. **Commit** le tue modifiche (`git commit -m 'Add some AmazingFeature'`)
4. **Push** al branch (`git push origin feature/AmazingFeature`)
5. **Apri** una Pull Request

### Tipi di Contributi Benvenuti

- 🐛 **Bug Reports**: Segnala problemi o comportamenti inaspettati
- 💡 **Feature Requests**: Suggerisci nuove funzionalità
- 📝 **Miglioramenti Documentazione**: Aiuta a migliorare guide e README
- 🔧 **Codice**: Implementa nuove feature o correggi bug
- 🎨 **Design**: Migliora l'interfaccia utente e l'esperienza utente

### Linee Guida per il Codice

- Segui le convenzioni C++ moderne (C++17+)
- Utilizza nomi descrittivi per variabili e funzioni
- Commenta il codice complesso
- Testa le modifiche su diverse distribuzioni Linux
- Mantieni la compatibilità con GTK4 e libadwaita

### Reporting Bug

Quando riporti un bug, includi:
- Versione del sistema operativo
- Versioni di GTK4 e libadwaita
- Passi per riprodurre il problema
- Comportamento atteso vs comportamento attuale
- Screenshot se applicabili

## 📄 Licenza

Questo progetto è distribuito sotto licenza [MIT](LICENSE). Vedi il file `LICENSE` per i dettagli completi.

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
```

---

<p align="center">
  <strong>Creato con ❤️ per la comunità D&D</strong>
</p>

<p align="center">
  Se questo progetto ti è stato utile, considera di lasciare una ⭐ su GitHub!
</p>

