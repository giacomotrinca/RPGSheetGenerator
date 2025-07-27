# RPGSheetGenerator

Un generatore di schede per personaggi di Dungeons & Dragons 5ª Edizione, sviluppato in C++ con GTK4 e libadwaita per un'interfaccia grafica moderna e pulita.

## Caratteristiche

- **Creazione Guidata del Personaggio**: Un'interfaccia a più passaggi per guidarti nella creazione del tuo personaggio.
- **Selezione Completa**: Scegli Razza, Classe, Background e Sottorazza (dove applicabile).
- **Generazione Statistiche Flessibile**:
    - **Point Buy**: Assegna i punti caratteristica con un budget di 27 punti, con calcolo dei costi in tempo reale.
    - **Tira i Dadi (4d6 drop lowest)**: Lancia i dadi per generare i punteggi e assegnali alle caratteristiche con un'intuitiva interfaccia Drag-and-Drop.
- **Interfaccia Moderna**: Costruito con libadwaita per un look & feel che si integra perfettamente con l'ambiente desktop GNOME.
- **Multipiattaforma**: Essendo basato su GTK, ha il potenziale per funzionare su Linux, Windows e macOS.

## Prerequisiti

Per compilare ed eseguire il progetto, avrai bisogno delle librerie di sviluppo per GTK4 e libadwaita.

Su una distribuzione basata su **Debian/Ubuntu**, puoi installarle con:
```bash
sudo apt-get install libgtk-4-dev libadwaita-1-dev
```

Su una distribuzione basata su **Fedora**:
```bash
sudo dnf install gtk4-devel libadwaita-devel
```

Avrai anche bisogno di un compilatore C++ (`g++`) e di `pkg-config`, solitamente inclusi nel pacchetto `build-essential` o `development-tools`.

## Come Compilare ed Eseguire

1.  **Clona il repository:**
    ```bash
    git clone https://github.com/giacomotrinca/RPGSheetGenerator.git
    ```
2.  **Entra nella directory del progetto:**
    ```bash
    cd RPGSheetGenerator
    ```
3.  **Rendi eseguibile lo script di avvio:**
    ```bash
    chmod +x run.sh
    ```
4.  **Esegui lo script:**
    Lo script si occuperà di compilare il codice sorgente ed avviare l'applicazione.
    ```bash
    ./run.sh
    ```


