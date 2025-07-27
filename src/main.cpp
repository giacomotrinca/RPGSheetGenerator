#include <adwaita.h>

// NOTA: Per un look moderno, usiamo la libreria Libadwaita.
// Assicurati di averla installata (`sudo pacman -S libadwaita` su Arch).

// Questa funzione viene chiamata quando l'applicazione si avvia.
static void activate(GtkApplication* app, gpointer user_data) {
    // --- Widget Principali ---
    GtkWidget *window;
    GtkWidget *header_bar;
    GtkWidget *main_box;
    GtkWidget *content_box;
    GtkWidget *generate_button;

    // --- Widget per il Contenuto ---
    GtkWidget *label_razza;
    GtkWidget *dropdown_razza;
    GtkWidget *label_classe;
    GtkWidget *dropdown_classe;
    GtkWidget *label_background;
    GtkWidget *dropdown_background;

    // 1. Crea la finestra principale usando AdwApplicationWindow per lo stile moderno.
    window = adw_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Generatore Schede D&D");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, -1); // Larghezza 400, altezza automatica

    // 2. Crea la Header Bar (la barra del titolo moderna)
    header_bar = gtk_header_bar_new();

    // 3. Crea il pulsante "Genera" e aggiungilo alla Header Bar
    generate_button = gtk_button_new_with_label("Genera Scheda");
    gtk_widget_add_css_class(generate_button, "suggested-action"); // Stile per pulsante primario
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), generate_button);

    // 4. Crea un contenitore principale per l'intera finestra
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // CORREZIONE: Aggiunto il cast GTK_BOX()
    gtk_box_append(GTK_BOX(main_box), header_bar); // Aggiungi la header bar in alto

    // 5. Crea un box per il contenuto con margini e spaziatura
    content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(content_box, 12);
    gtk_widget_set_margin_end(content_box, 12);
    gtk_widget_set_margin_top(content_box, 12);
    gtk_widget_set_margin_bottom(content_box, 12);
    // CORREZIONE: Aggiunto il cast GTK_BOX()
    gtk_box_append(GTK_BOX(main_box), content_box); // Aggiungi il box del contenuto sotto la header

    // Imposta il main_box come contenuto della finestra
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), main_box);

    // --- Sezione Razza ---
    label_razza = gtk_label_new("Razza");
    gtk_widget_add_css_class(label_razza, "title-4"); // Stile titolo più piccolo
    gtk_widget_set_halign(label_razza, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_razza);

    const char *razze[] = {
        "Umano", "Elfo", "Halfling", "Nano", "Dragonide", 
        "Gnomo", "Mezzelfo", "Mezzorco", "Tiefling", NULL
    };
    dropdown_razza = gtk_drop_down_new_from_strings(razze);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown_razza), 0);
    gtk_box_append(GTK_BOX(content_box), dropdown_razza);

    // --- Sezione Classe ---
    label_classe = gtk_label_new("Classe");
    gtk_widget_add_css_class(label_classe, "title-4");
    gtk_widget_set_halign(label_classe, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_classe);

    const char *classi[] = {
        "Guerriero", "Barbaro", "Bardo", "Chierico", "Druido", 
        "Ladro", "Mago", "Monaco", "Paladino", "Ranger", 
        "Stregone", "Warlock", NULL
    };
    dropdown_classe = gtk_drop_down_new_from_strings(classi);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown_classe), 0);
    gtk_box_append(GTK_BOX(content_box), dropdown_classe);

    // --- Sezione Background ---
    label_background = gtk_label_new("Background");
    gtk_widget_add_css_class(label_background, "title-4");
    gtk_widget_set_halign(label_background, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_background);

    const char *backgrounds[] = {
        "Accolito", "Artigiano di Gilda", "Ciarlatano", "Criminale",
        "Eremita", "Eroe Popolare", "Forestiero", "Intrattenitore",
        "Marinaio", "Monello", "Nobile", "Sapiente", "Soldato", NULL
    };
    dropdown_background = gtk_drop_down_new_from_strings(backgrounds);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown_background), 0);
    gtk_box_append(GTK_BOX(content_box), dropdown_background);

    // Mostra la finestra e tutti i suoi widget figli.
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    AdwApplication *app;
    int status;

    // Usa AdwApplication invece di GtkApplication
    app = adw_application_new("com.esempio.dndgenerator", G_APPLICATION_DEFAULT_FLAGS);

    // Collega il segnale "activate" alla nostra funzione 'activate'.
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Avvia l'applicazione.
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Rilascia l'oggetto applicazione per liberare la memoria.
    g_object_unref(app);

    return status;
}