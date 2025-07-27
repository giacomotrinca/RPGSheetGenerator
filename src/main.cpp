#include <adwaita.h>
#include <string.h> // Aggiunto per usare strcmp
#include <stdlib.h> // Aggiunto per rand()
#include <time.h>   // Aggiunto per time()

// Struct per i dati globali dell'applicazione.
typedef struct {
    GtkWindow *main_window;
    AdwNavigationView *nav_view;
    GtkDropDown *dropdown_razza;
    GtkDropDown *dropdown_classe;
    GtkDropDown *dropdown_background;
} AppData;

// Struct per i dati specifici della pagina delle statistiche.
typedef struct {
    // Contenitori per le modalità
    GtkWidget *point_buy_box;
    GtkWidget *rolling_box;

    // Point Buy
    GtkWidget *point_buy_label;
    AdwSpinRow *spin_rows_point_buy[6];

    // Rolling
    GtkFlowBox *rolls_flowbox;
    GtkEntry *stat_entries_rolling[6];
} StatsPageData;


// Dichiarazione anticipata delle funzioni.
static AdwNavigationPage* create_stats_page(AppData *data, const char* razza_scelta, const char* classe_scelta, const char* background_scelto);
static void on_back_clicked(GtkButton *button, gpointer user_data);
static void on_generate_clicked(GtkButton *button, gpointer user_data);
static void on_quit_clicked(GtkButton *button, gpointer user_data);
static void on_roll_dice_clicked(GtkButton *button, gpointer user_data);
static void on_point_buy_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data);
static void on_method_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data);
// Funzioni per il Drag-and-Drop
static GdkContentProvider* on_drag_prepare(GtkDragSource *source, double x, double y, gpointer user_data);
static gboolean on_stat_drop(GtkDropTarget *target, const GValue *value, double x, double y, gpointer user_data);
static GtkWidget* create_draggable_score_label(int score);


// Funzione di callback per il pulsante "Avanti".
static void on_avanti_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;

    GtkStringList *razze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_razza));
    guint razza_pos = gtk_drop_down_get_selected(data->dropdown_razza);
    const char *razza_scelta = gtk_string_list_get_string(razze_model, razza_pos);

    GtkStringList *classi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_classe));
    guint classe_pos = gtk_drop_down_get_selected(data->dropdown_classe);
    const char *classe_scelta = gtk_string_list_get_string(classi_model, classe_pos);

    GtkStringList *backgrounds_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_background));
    guint background_pos = gtk_drop_down_get_selected(data->dropdown_background);
    const char *background_scelto = gtk_string_list_get_string(backgrounds_model, background_pos);

    AdwNavigationPage *stats_page = create_stats_page(data, razza_scelta, classe_scelta, background_scelto);
    adw_navigation_view_push(data->nav_view, stats_page);
}

// Callback per il pulsante "Indietro" manuale.
static void on_back_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    adw_navigation_view_pop(data->nav_view);
}

// Callback per il pulsante "Genera Scheda".
static void on_generate_clicked(GtkButton *button, gpointer user_data) {
    g_print("Pulsante 'Genera Scheda' cliccato! Inizializzazione generazione file .tex...\n");
}

// Callback per il pulsante "Esci".
static void on_quit_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    gtk_window_destroy(data->main_window);
}

// Funzione che costruisce il contenuto della pagina delle statistiche.
static AdwNavigationPage* create_stats_page(AppData *data, const char* razza_scelta, const char* classe_scelta, const char* background_scelto) {
    AdwNavigationPage *content;
    GtkWidget *page_vbox, *stats_hbox, *summary_group, *right_vbox, *button_box, *back_button, *generate_button;
    const char* stats_names[] = {"FOR", "DES", "COS", "INT", "SAG", "CAR", NULL};

    StatsPageData *stats_data = g_new0(StatsPageData, 1);

    page_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(page_vbox, 24);
    gtk_widget_set_margin_end(page_vbox, 24);
    gtk_widget_set_margin_top(page_vbox, 24);
    gtk_widget_set_margin_bottom(page_vbox, 24);
    g_signal_connect(page_vbox, "destroy", G_CALLBACK(g_free), stats_data);

    stats_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);
    gtk_widget_set_halign(stats_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(stats_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(stats_hbox, TRUE);

    // --- Gruppo Sinistro (Riepilogo Scelte) ---
    summary_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(summary_group), "Riepilogo Personaggio");
    gtk_widget_set_size_request(summary_group, 280, -1);

    GtkWidget *row_razza = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_razza), "Razza");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_razza), razza_scelta);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_razza);

    GtkWidget *row_classe = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_classe), "Classe");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_classe), classe_scelta);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_classe);
    
    GtkWidget *row_background = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_background), "Background");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_background), background_scelto);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_background);
    
    GtkWidget *row_sottorazza = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_sottorazza), "Sottorazza");
    GtkDropDown *dropdown_sottorazza = GTK_DROP_DOWN(gtk_drop_down_new(NULL, NULL));
    adw_action_row_add_suffix(ADW_ACTION_ROW(row_sottorazza), GTK_WIDGET(dropdown_sottorazza));
    adw_action_row_set_activatable_widget(ADW_ACTION_ROW(row_sottorazza), GTK_WIDGET(dropdown_sottorazza));
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_sottorazza);

    if (strcmp(razza_scelta, "Elfo") == 0) {
        const char *sottorazze[] = {"Alto Elfo", "Elfo dei Boschi", "Drow (Elfo Oscuro)", NULL};
        gtk_drop_down_set_model(dropdown_sottorazza, G_LIST_MODEL(gtk_string_list_new(sottorazze)));
        gtk_widget_set_visible(row_sottorazza, TRUE);
    } else if (strcmp(razza_scelta, "Nano") == 0) {
        const char *sottorazze[] = {"Nano delle Colline", "Nano delle Montagne", NULL};
        gtk_drop_down_set_model(dropdown_sottorazza, G_LIST_MODEL(gtk_string_list_new(sottorazze)));
        gtk_widget_set_visible(row_sottorazza, TRUE);
    } else if (strcmp(razza_scelta, "Halfling") == 0) {
        const char *sottorazze[] = {"Piedelesto", "Tozzo", NULL};
        gtk_drop_down_set_model(dropdown_sottorazza, G_LIST_MODEL(gtk_string_list_new(sottorazze)));
        gtk_widget_set_visible(row_sottorazza, TRUE);
    } else if (strcmp(razza_scelta, "Gnomo") == 0) {
        const char *sottorazze[] = {"Gnomo delle Foreste", "Gnomo delle Rocce", NULL};
        gtk_drop_down_set_model(dropdown_sottorazza, G_LIST_MODEL(gtk_string_list_new(sottorazze)));
        gtk_widget_set_visible(row_sottorazza, TRUE);
    } else {
        gtk_widget_set_visible(row_sottorazza, FALSE);
    }
    if (gtk_widget_is_visible(row_sottorazza)) {
        gtk_drop_down_set_selected(dropdown_sottorazza, 0);
    }
    
    // --- Pannello Destro (Scelta Metodo Statistiche) ---
    right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

    GtkWidget *method_label = gtk_label_new("Metodo Punteggi");
    gtk_widget_set_halign(method_label, GTK_ALIGN_START);
    const char *methods[] = {"Point Buy", "Tira i Dadi", NULL};
    GtkWidget *method_dropdown = gtk_drop_down_new_from_strings(methods);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(method_dropdown), 0);
    g_signal_connect(method_dropdown, "notify::selected-item", G_CALLBACK(on_method_changed), stats_data);

    gtk_box_append(GTK_BOX(right_vbox), method_label);
    gtk_box_append(GTK_BOX(right_vbox), method_dropdown);


    // --- Contenitore per Point Buy ---
    stats_data->point_buy_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_box_append(GTK_BOX(right_vbox), stats_data->point_buy_box);
    
    stats_data->point_buy_label = gtk_label_new("Punti Rimanenti: 27");
    gtk_widget_add_css_class(stats_data->point_buy_label, "title-4");
    gtk_box_append(GTK_BOX(stats_data->point_buy_box), stats_data->point_buy_label);

    GtkWidget *pb_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(pb_grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(pb_grid), 12);
    for (int i = 0; stats_names[i] != NULL; ++i) {
        GtkWidget *label = gtk_label_new(stats_names[i]);
        stats_data->spin_rows_point_buy[i] = ADW_SPIN_ROW(adw_spin_row_new_with_range(8, 15, 1));
        adw_spin_row_set_value(stats_data->spin_rows_point_buy[i], 8);
        g_signal_connect(stats_data->spin_rows_point_buy[i], "notify::value", G_CALLBACK(on_point_buy_changed), stats_data);
        gtk_grid_attach(GTK_GRID(pb_grid), label, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(pb_grid), GTK_WIDGET(stats_data->spin_rows_point_buy[i]), 1, i, 1, 1);
    }
    gtk_box_append(GTK_BOX(stats_data->point_buy_box), pb_grid);


    // --- Contenitore per Tira i Dadi ---
    stats_data->rolling_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_box_append(GTK_BOX(right_vbox), stats_data->rolling_box);
    gtk_widget_set_visible(stats_data->rolling_box, FALSE);

    GtkWidget *roll_actions_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *roll_button = gtk_button_new_with_label("Lancia i Dadi");
    stats_data->rolls_flowbox = GTK_FLOW_BOX(gtk_flow_box_new());
    gtk_flow_box_set_max_children_per_line(stats_data->rolls_flowbox, 6);
    gtk_flow_box_set_selection_mode(stats_data->rolls_flowbox, GTK_SELECTION_NONE);
    g_signal_connect(roll_button, "clicked", G_CALLBACK(on_roll_dice_clicked), stats_data);
    gtk_box_append(GTK_BOX(roll_actions_box), roll_button);
    gtk_box_append(GTK_BOX(roll_actions_box), GTK_WIDGET(stats_data->rolls_flowbox));
    gtk_box_append(GTK_BOX(stats_data->rolling_box), roll_actions_box);

    GtkWidget *roll_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(roll_grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(roll_grid), 12);
    for (int i = 0; stats_names[i] != NULL; ++i) {
        GtkWidget *label = gtk_label_new(stats_names[i]);
        stats_data->stat_entries_rolling[i] = GTK_ENTRY(gtk_entry_new());
        gtk_editable_set_editable(GTK_EDITABLE(stats_data->stat_entries_rolling[i]), FALSE);
        gtk_entry_set_alignment(stats_data->stat_entries_rolling[i], 0.5);
        gtk_widget_add_css_class(GTK_WIDGET(stats_data->stat_entries_rolling[i]), "title-3");

        GtkDropTarget *target = gtk_drop_target_new(G_TYPE_STRING, GDK_ACTION_MOVE);
        g_signal_connect(target, "drop", G_CALLBACK(on_stat_drop), stats_data);
        gtk_widget_add_controller(GTK_WIDGET(stats_data->stat_entries_rolling[i]), GTK_EVENT_CONTROLLER(target));
        
        gtk_grid_attach(GTK_GRID(roll_grid), label, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(roll_grid), GTK_WIDGET(stats_data->stat_entries_rolling[i]), 1, i, 1, 1);
    }
    gtk_box_append(GTK_BOX(stats_data->rolling_box), roll_grid);

    
    gtk_box_append(GTK_BOX(stats_hbox), summary_group);
    gtk_box_append(GTK_BOX(stats_hbox), right_vbox);

    // --- Barra dei Pulsanti in basso ---
    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_halign(button_box, GTK_ALIGN_END);
    
    back_button = gtk_button_new_with_label("Indietro");
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_clicked), data);

    generate_button = gtk_button_new_with_label("Genera Scheda");
    gtk_widget_add_css_class(generate_button, "suggested-action");
    g_signal_connect(generate_button, "clicked", G_CALLBACK(on_generate_clicked), data);

    gtk_box_append(GTK_BOX(button_box), back_button);
    gtk_box_append(GTK_BOX(button_box), generate_button);

    gtk_box_append(GTK_BOX(page_vbox), stats_hbox);
    gtk_box_append(GTK_BOX(page_vbox), button_box);

    content = adw_navigation_page_new(page_vbox, "Statistiche");
    return content;
}

// Funzione che costruisce il contenuto della pagina delle selezioni.
static AdwNavigationPage* create_selections_page(AppData *data) {
    AdwNavigationPage *content;
    GtkWidget *content_box, *avanti_button;

    content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(content_box, 24);
    gtk_widget_set_margin_end(content_box, 24);
    gtk_widget_set_margin_top(content_box, 24);
    gtk_widget_set_margin_bottom(content_box, 24);

    GtkWidget* label_razza = gtk_label_new("Razza");
    gtk_widget_add_css_class(label_razza, "title-4");
    gtk_widget_set_halign(label_razza, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_razza);
    const char *razze[] = {"Umano", "Elfo", "Halfling", "Nano", "Dragonide", "Gnomo", "Mezzelfo", "Mezzorco", "Tiefling", NULL};
    data->dropdown_razza = GTK_DROP_DOWN(gtk_drop_down_new_from_strings(razze));
    gtk_drop_down_set_selected(data->dropdown_razza, 0);
    gtk_box_append(GTK_BOX(content_box), GTK_WIDGET(data->dropdown_razza));

    GtkWidget* label_classe = gtk_label_new("Classe");
    gtk_widget_add_css_class(label_classe, "title-4");
    gtk_widget_set_halign(label_classe, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_classe);
    const char *classi[] = {"Guerriero", "Barbaro", "Bardo", "Chierico", "Druido", "Ladro", "Mago", "Monaco", "Paladino", "Ranger", "Stregone", "Warlock", NULL};
    data->dropdown_classe = GTK_DROP_DOWN(gtk_drop_down_new_from_strings(classi));
    gtk_drop_down_set_selected(data->dropdown_classe, 0);
    gtk_box_append(GTK_BOX(content_box), GTK_WIDGET(data->dropdown_classe));

    GtkWidget* label_background = gtk_label_new("Background");
    gtk_widget_add_css_class(label_background, "title-4");
    gtk_widget_set_halign(label_background, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_background);
    const char *backgrounds[] = {"Accolito", "Artigiano di Gilda", "Ciarlatano", "Criminale", "Eremita", "Eroe Popolare", "Forestiero", "Intrattenitore", "Marinaio", "Monello", "Nobile", "Sapiente", "Soldato", NULL};
    data->dropdown_background = GTK_DROP_DOWN(gtk_drop_down_new_from_strings(backgrounds));
    gtk_drop_down_set_selected(data->dropdown_background, 0);
    gtk_box_append(GTK_BOX(content_box), GTK_WIDGET(data->dropdown_background));

    avanti_button = gtk_button_new_with_label("Avanti");
    gtk_widget_add_css_class(avanti_button, "suggested-action");
    gtk_widget_set_valign(avanti_button, GTK_ALIGN_END);
    gtk_widget_set_vexpand(avanti_button, TRUE);
    g_signal_connect(avanti_button, "clicked", G_CALLBACK(on_avanti_clicked), data);
    gtk_box_append(GTK_BOX(content_box), avanti_button);

    content = adw_navigation_page_new(content_box, "Crea Personaggio");
    return content;
}

// Funzione principale di attivazione dell'applicazione.
static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *toolbar_view;
    GtkWidget *header_bar;
    GtkWidget *quit_button;
    AppData *data = g_new(AppData, 1);

    window = adw_application_window_new(app);
    data->main_window = GTK_WINDOW(window);
    gtk_window_set_title(GTK_WINDOW(window), "Generatore Schede D&D");
    gtk_window_set_default_size(GTK_WINDOW(window), -1, -1); // Dimensione automatica

    toolbar_view = adw_toolbar_view_new();
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), toolbar_view);

    header_bar = gtk_header_bar_new();
    adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW(toolbar_view), header_bar);

    quit_button = gtk_button_new_from_icon_name("window-close-symbolic");
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_clicked), data);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), quit_button);

    data->nav_view = ADW_NAVIGATION_VIEW(adw_navigation_view_new());
    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(toolbar_view), GTK_WIDGET(data->nav_view));
    
    AdwNavigationPage *selections_page = create_selections_page(data);
    adw_navigation_view_push(data->nav_view, selections_page);

    g_signal_connect(window, "destroy", G_CALLBACK(g_free), data);

    gtk_window_present(GTK_WINDOW(window));
}

// --- Nuove Funzioni Helper ---

// NUOVA: Gestisce il cambio di metodo per la generazione delle statistiche
static void on_method_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    GtkDropDown *dropdown = GTK_DROP_DOWN(source_object);
    guint selected = gtk_drop_down_get_selected(dropdown);

    if (selected == 0) { // Point Buy
        gtk_widget_set_visible(stats_data->point_buy_box, TRUE);
        gtk_widget_set_visible(stats_data->rolling_box, FALSE);
    } else { // Tira i Dadi
        gtk_widget_set_visible(stats_data->point_buy_box, FALSE);
        gtk_widget_set_visible(stats_data->rolling_box, TRUE);
    }
}

// Logica per il Point Buy
static void on_point_buy_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    int costs[] = {0, 1, 2, 3, 4, 5, 7, 9}; // Costi per punteggi da 8 a 15
    int total_cost = 0;

    for (int i = 0; i < 6; i++) {
        int value = adw_spin_row_get_value(stats_data->spin_rows_point_buy[i]);
        total_cost += costs[value - 8];
    }
    
    int budget = 27 - total_cost;
    char buffer[50];
    sprintf(buffer, "Punti Rimanenti: %d", budget);
    gtk_label_set_text(GTK_LABEL(stats_data->point_buy_label), buffer);

    for (int i = 0; i < 6; i++) {
        GtkAdjustment *adj = adw_spin_row_get_adjustment(stats_data->spin_rows_point_buy[i]);
        if (budget <= 0) {
            gtk_adjustment_set_upper(adj, adw_spin_row_get_value(stats_data->spin_rows_point_buy[i]));
        } else {
            gtk_adjustment_set_upper(adj, 15);
        }
    }

    if (budget < 0) {
        gtk_widget_add_css_class(stats_data->point_buy_label, "error");
    } else {
        gtk_widget_remove_css_class(stats_data->point_buy_label, "error");
    }
}

// Logica per il lancio dei dadi
static void on_roll_dice_clicked(GtkButton *button, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(stats_data->rolls_flowbox));
    while (child != NULL) {
        GtkWidget* next_child = gtk_widget_get_next_sibling(child);
        gtk_flow_box_remove(stats_data->rolls_flowbox, child);
        child = next_child;
    }
    for (int i = 0; i < 6; i++) {
        gtk_editable_set_text(GTK_EDITABLE(stats_data->stat_entries_rolling[i]), "");
    }

    for (int i = 0; i < 6; i++) {
        int rolls[4];
        int sum = 0;
        int min = 7;

        for (int j = 0; j < 4; j++) {
            rolls[j] = (rand() % 6) + 1;
            sum += rolls[j];
            if (rolls[j] < min) {
                min = rolls[j];
            }
        }
        int score = sum - min;
        
        GtkWidget *label = create_draggable_score_label(score);
        gtk_flow_box_insert(stats_data->rolls_flowbox, label, -1);
    }
}

// NUOVA: Crea un'etichetta trascinabile per un punteggio
static GtkWidget* create_draggable_score_label(int score) {
    char buffer[10];
    sprintf(buffer, "%d", score);
    GtkWidget *label = gtk_label_new(buffer);
    gtk_widget_add_css_class(label, "title-2");
    gtk_widget_set_margin_start(label, 6);
    gtk_widget_set_margin_end(label, 6);

    GtkDragSource *source = gtk_drag_source_new();
    gtk_drag_source_set_actions(source, GDK_ACTION_MOVE);
    // CORREZIONE: Passa l'etichetta stessa come user_data per identificarla
    g_signal_connect(source, "prepare", G_CALLBACK(on_drag_prepare), label);
    gtk_widget_add_controller(label, GTK_EVENT_CONTROLLER(source));

    return label;
}

// NUOVA: Prepara i dati per il drag
static GdkContentProvider* on_drag_prepare(GtkDragSource *source, double x, double y, gpointer user_data) {
    GtkWidget *label = GTK_WIDGET(user_data);
    const char *text = gtk_label_get_text(GTK_LABEL(label));
    
    // CORREZIONE: Passa un payload "punteggio:indirizzo_widget" per identificarlo univocamente
    char payload[100];
    sprintf(payload, "%s:%p", text, (void*)label);

    GValue value = G_VALUE_INIT;
    g_value_init(&value, G_TYPE_STRING);
    g_value_set_string(&value, payload);

    return gdk_content_provider_new_for_value(&value);
}

// NUOVA: Gestisce il drop di un punteggio su una caratteristica
static gboolean on_stat_drop(GtkDropTarget *target, const GValue *value, double x, double y, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    GtkEntry *entry = GTK_ENTRY(gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(target)));
    
    // CORREZIONE: Estrae il punteggio e l'indirizzo del widget dal payload
    const char *payload = g_value_get_string(value);
    char payload_copy[100];
    strcpy(payload_copy, payload);

    char *new_score_str = strtok(payload_copy, ":");
    char *widget_addr_str = strtok(NULL, ":");

    if (!new_score_str || !widget_addr_str) return FALSE;

    // Gestisce lo scambio se la casella era già piena
    const char *old_score_str = gtk_editable_get_text(GTK_EDITABLE(entry));
    if (old_score_str && strlen(old_score_str) > 0) {
        // CORREZIONE: Cerca il widget disattivato e lo riattiva
        GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(stats_data->rolls_flowbox));
        while (child != NULL) {
            if (GTK_IS_LABEL(child) && !gtk_widget_get_sensitive(child)) {
                if (strcmp(gtk_label_get_text(GTK_LABEL(child)), old_score_str) == 0) {
                    gtk_widget_set_sensitive(child, TRUE);
                    gtk_widget_set_opacity(child, 1.0);
                    break;
                }
            }
            child = gtk_widget_get_next_sibling(child);
        }
    }

    // Imposta il nuovo punteggio
    gtk_editable_set_text(GTK_EDITABLE(entry), new_score_str);

    // CORREZIONE: Disattiva l'etichetta originale dalla lista usando il suo indirizzo
    GtkWidget *dragged_label = NULL;
    sscanf(widget_addr_str, "%p", (void**)&dragged_label);

    if (dragged_label) {
        gtk_widget_set_opacity(dragged_label, 0.4);
        gtk_widget_set_sensitive(dragged_label, FALSE);
    }

    return TRUE; // Drop accettato
}


int main(int argc, char **argv) {
    // Inizializza il seme per la generazione casuale
    srand(time(NULL));

    AdwApplication *app;
    int status;

    app = adw_application_new("com.esempio.dndgenerator", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
