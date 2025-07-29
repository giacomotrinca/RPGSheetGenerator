# Makefile per RPGSheetGenerator
# D&D 5e Character Sheet Generator

# Configurazione del compilatore
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
DEBUG_FLAGS = -g -DDEBUG -O0
TARGET = dnd_generator
SOURCE_DIR = src
SOURCES = $(SOURCE_DIR)/main.cpp

# Configurazione delle librerie
PKG_CONFIG = pkg-config
LIBS = gtk4 libadwaita-1
CFLAGS = $(shell $(PKG_CONFIG) --cflags $(LIBS))
LDFLAGS = $(shell $(PKG_CONFIG) --libs $(LIBS))

# Target di default
all: setup $(TARGET)

# Setup iniziale (crea cartelle necessarie)
setup:
	@echo "📁 Creazione cartelle di progetto..."
	@mkdir -p save
	@echo "✅ Cartella 'save' creata"

# Compilazione release
$(TARGET): $(SOURCES)
	@echo "🔨 Compilazione in modalità release..."
	$(CXX) $(CXXFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS)
	@echo "✅ Compilazione completata: $(TARGET)"

# Compilazione debug
debug: $(SOURCES)
	@echo "🐛 Compilazione in modalità debug..."
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(CFLAGS) $(SOURCES) -o $(TARGET)_debug $(LDFLAGS)
	@echo "✅ Compilazione debug completata: $(TARGET)_debug"

# Esecuzione dell'applicazione
run: $(TARGET)
	@echo "🚀 Avvio dell'applicazione..."
	./$(TARGET)

# Esecuzione in modalità debug
run-debug: debug
	@echo "🐛 Avvio dell'applicazione in modalità debug..."
	./$(TARGET)_debug

# Installazione nell'ambiente locale
install: $(TARGET)
	@echo "📦 Installazione in /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "✅ Installazione completata"

# Disinstallazione
uninstall:
	@echo "🗑️  Disinstallazione..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "✅ Disinstallazione completata"

# Pulizia dei file compilati
clean:
	@echo "🧹 Pulizia dei file compilati..."
	rm -f $(TARGET) $(TARGET)_debug
	@echo "✅ Pulizia completata"

# Controllo delle dipendenze
check-deps:
	@echo "🔍 Controllo delle dipendenze..."
	@$(PKG_CONFIG) --exists $(LIBS) && echo "✅ Dipendenze GTK installate" || echo "❌ Dipendenze GTK mancanti"
	@echo "Versioni installate:"
	@$(PKG_CONFIG) --modversion $(LIBS) 2>/dev/null || echo "❌ Impossibile verificare le versioni"

# Informazioni sul progetto
info:
	@echo "📋 Informazioni del progetto:"
	@echo "   Nome: RPGSheetGenerator"
	@echo "   Target: $(TARGET)"
	@echo "   Sorgenti: $(SOURCES)"
	@echo "   Compilatore: $(CXX)"
	@echo "   Flags: $(CXXFLAGS)"
	@echo "   Librerie: $(LIBS)"

# Pacchettizzazione
package: clean $(TARGET)
	@echo "📦 Creazione del pacchetto..."
	mkdir -p dist/$(TARGET)
	cp $(TARGET) dist/$(TARGET)/
	cp README.md dist/$(TARGET)/
	cp LICENSE dist/$(TARGET)/
	tar -czf dist/$(TARGET).tar.gz -C dist $(TARGET)
	@echo "✅ Pacchetto creato: dist/$(TARGET).tar.gz"

# Target di aiuto
help:
	@echo "📚 Makefile per RPGSheetGenerator"
	@echo ""
	@echo "Target disponibili:"
	@echo "  all            Compila l'applicazione (default)"
	@echo "  debug          Compila in modalità debug"
	@echo "  run            Compila ed esegue l'applicazione"
	@echo "  run-debug      Compila ed esegue in modalità debug"
	@echo "  install        Installa in /usr/local/bin"
	@echo "  uninstall      Rimuove dall'installazione"
	@echo "  clean          Rimuove i file compilati"
	@echo "  check-deps     Verifica le dipendenze"
	@echo "  info           Mostra informazioni del progetto"
	@echo "  package        Crea un pacchetto distribuibile"
	@echo "  help           Mostra questo messaggio"
	@echo ""
	@echo "Esempi:"
	@echo "  make           # Compila l'applicazione"
	@echo "  make run       # Compila ed esegue"
	@echo "  make debug     # Compila in modalità debug"
	@echo "  make clean     # Pulisce i file compilati"

# Dichiarazione dei target che non creano file
.PHONY: all debug run run-debug install uninstall clean check-deps info package help
