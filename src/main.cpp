#include <adwaita.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// --- LIBRERIA NOMI ---
// Helper per ottenere un nome casuale da un array.
static const char* get_random_from_array(const char* arr[], int size) {
    if (size == 0) return "";
    return arr[rand() % size];
}

// Nomi Umani
static const char* HUMAN_MALE_NAMES[] = {"Arthur", "Lancelot", "Gawain", "Percival", "Tristan", "Galahad", "Bors", "Kay", "Bedivere", "Hector"};
static const char* HUMAN_FEMALE_NAMES[] = {"Guinevere", "Morgana", "Elaine", "Lynette", "Enid", "Igraine", "Nimue", "Viviane", "Laudine", "Ragnelle"};
static const char* HUMAN_NEUTRAL_NAMES[] = {"Arden", "Blair", "Cassian", "Darcy", "Ellis", "Jules", "Kai", "Quinn", "Reese", "Skyler"};

// Nomi Elfici
static const char* ELF_MALE_NAMES[] = {"Aelar", "Eren", "Laelon", "Ilian", "Riardon", "Soveliss", "Theren", "Varis", "Zylas"};
static const char* ELF_FEMALE_NAMES[] = {"Aelin", "Elora", "Liara", "Mirelle", "Naivara", "Sariel", "Taela", "Valen", "Ysera"};
static const char* ELF_NEUTRAL_NAMES[] = {"Ariel", "Elaris", "Faelan", "Lian", "Nael", "Riel", "Shay", "Tael", "Zin"};

// Nomi Nani
static const char* DWARF_MALE_NAMES[] = {"Balin", "Borin", "Dain", "Fili", "Gimli", "Harkon", "Kili", "Thorin", "Varrick"};
static const char* DWARF_FEMALE_NAMES[] = {"Amber", "Bruna", "Diesa", "Eldis", "Freyda", "Gerda", "Helga", "Ingrid", "Sif"};
static const char* DWARF_NEUTRAL_NAMES[] = {"Bryn", "Dagnir", "Gorm", "Runa", "Storn", "Thane", "Val"};

// Funzione principale per ottenere un nome casuale
static const char* get_random_name(const char* race, const char* gender) {
    if (strcmp(race, "Umano") == 0) {
        if (strcmp(gender, "Maschio") == 0) return get_random_from_array(HUMAN_MALE_NAMES, G_N_ELEMENTS(HUMAN_MALE_NAMES));
        if (strcmp(gender, "Femmina") == 0) return get_random_from_array(HUMAN_FEMALE_NAMES, G_N_ELEMENTS(HUMAN_FEMALE_NAMES));
        return get_random_from_array(HUMAN_NEUTRAL_NAMES, G_N_ELEMENTS(HUMAN_NEUTRAL_NAMES));
    }
    if (strcmp(race, "Elfo") == 0) {
        if (strcmp(gender, "Maschio") == 0) return get_random_from_array(ELF_MALE_NAMES, G_N_ELEMENTS(ELF_MALE_NAMES));
        if (strcmp(gender, "Femmina") == 0) return get_random_from_array(ELF_FEMALE_NAMES, G_N_ELEMENTS(ELF_FEMALE_NAMES));
        return get_random_from_array(ELF_NEUTRAL_NAMES, G_N_ELEMENTS(ELF_NEUTRAL_NAMES));
    }
    if (strcmp(race, "Nano") == 0) {
        if (strcmp(gender, "Maschio") == 0) return get_random_from_array(DWARF_MALE_NAMES, G_N_ELEMENTS(DWARF_MALE_NAMES));
        if (strcmp(gender, "Femmina") == 0) return get_random_from_array(DWARF_FEMALE_NAMES, G_N_ELEMENTS(DWARF_FEMALE_NAMES));
        return get_random_from_array(DWARF_NEUTRAL_NAMES, G_N_ELEMENTS(DWARF_NEUTRAL_NAMES));
    }
    // Fallback per altre razze
    return get_random_from_array(HUMAN_NEUTRAL_NAMES, G_N_ELEMENTS(HUMAN_NEUTRAL_NAMES));
}

// Calcola il bonus competenza in base al livello.
static int get_proficiency_bonus(int level) {
    return 2 + (level - 1) / 4;
}


// Struct per i dati globali dell'applicazione.
typedef struct {
    GtkWindow *main_window;
    AdwNavigationView *nav_view;
    GtkDropDown *dropdown_razza;
    GtkDropDown *dropdown_classe;
    GtkDropDown *dropdown_background;
    GtkDropDown *dropdown_genere;
    GtkEntry *entry_nome;
    GtkScale *scale_livello;
    GtkWidget *row_sottorazza;
    GtkDropDown *dropdown_sottorazza;
} AppData;

// Struct per i dati specifici della pagina delle statistiche.
typedef struct {
    // Modalità di generazione
    GtkWidget *point_buy_box;
    GtkWidget *rolling_box;

    // Point Buy
    GtkWidget *point_buy_label;
    AdwSpinRow *spin_rows_point_buy[6];
    GtkWidget *total_score_labels_pb[6];
    GtkWidget *modifier_labels_pb[6];

    // Rolling
    GtkFlowBox *rolls_flowbox;
    GtkEntry *stat_entries_rolling[6];
    GtkWidget *total_score_labels_roll[6];
    GtkWidget *modifier_labels_roll[6];
    GtkWidget *reset_button_rolling;
    
    // Comuni
    GtkDropDown *method_dropdown;
    char *race;
    char *subrace;
    int choice_bonus_count;
    GtkWidget *racial_choice_group;
    GtkCheckButton *stat_choice_checks[6];
    GtkWidget *forward_button;
} StatsPageData;


// Dichiarazione anticipata delle funzioni.
static void update_total_scores(StatsPageData *stats_data);
static void update_forward_button_sensitivity(StatsPageData *stats_data);
static AdwNavigationPage* create_stats_page(AppData *data, const char* nome_scelto, const char* genere_scelto, int livello_scelto, const char* razza_scelta, const char* subrace_scelta, const char* classe_scelta, const char* background_scelto);
static AdwNavigationPage* create_skills_page(AppData *data);
static void on_back_clicked(GtkButton *button, gpointer user_data);
static void on_stats_avanti_clicked(GtkButton *button, gpointer user_data);
static void on_generate_sheet_clicked(GtkButton *button, gpointer user_data);
static void on_quit_clicked(GtkButton *button, gpointer user_data);
static void on_roll_dice_clicked(GtkButton *button, gpointer user_data);
static void on_point_buy_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data);
static void on_method_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data);
static void on_reset_button_clicked(GtkButton *button, gpointer user_data);
static void on_random_name_clicked(GtkButton *button, gpointer user_data);
static void on_randomize_all_clicked(GtkButton *button, gpointer user_data);
static void on_racial_bonus_choice_toggled(GtkCheckButton *button, gpointer user_data);
static char* get_racial_bonus_string(const char* race, const char* subrace);


// Funzioni per il Drag-and-Drop
static GdkContentProvider* on_drag_prepare(GtkDragSource *source, double x, double y, gpointer user_data);
static gboolean on_stat_drop(GtkDropTarget *target, const GValue *value, double x, double y, gpointer user_data);
static GtkWidget* create_draggable_score_label(int score);

// Logica di reset per i punteggi dei dadi.
static void on_reset_button_clicked(GtkButton *button, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    for (int i = 0; i < 6; i++) {
        GtkEntry *entry = stats_data->stat_entries_rolling[i];
        GtkWidget *label_to_reenable = GTK_WIDGET(g_object_get_data(G_OBJECT(entry), "dragged-label"));
        if (label_to_reenable != NULL) {
            gtk_widget_set_sensitive(label_to_reenable, TRUE);
            gtk_widget_set_opacity(label_to_reenable, 1.0);
            g_object_set_data(G_OBJECT(entry), "dragged-label", NULL);
        }
        gtk_editable_set_text(GTK_EDITABLE(entry), "");
    }
    // Resetta anche i bonus a scelta
    if (stats_data->choice_bonus_count > 0) {
        for (int i = 0; i < 6; i++) {
            gtk_check_button_set_active(stats_data->stat_choice_checks[i], FALSE);
        }
    }
    update_total_scores(stats_data);
    update_forward_button_sensitivity(stats_data);
}

// Callback per il pulsante "Avanti".
static void on_avanti_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    const char *subrace_scelta = "";

    const char *nome_scelto = gtk_editable_get_text(GTK_EDITABLE(data->entry_nome));

    GtkStringList *generi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_genere));
    guint genere_pos = gtk_drop_down_get_selected(data->dropdown_genere);
    const char *genere_scelto = gtk_string_list_get_string(generi_model, genere_pos);
    
    int livello_scelto = (int)gtk_range_get_value(GTK_RANGE(data->scale_livello));

    GtkStringList *razze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_razza));
    guint razza_pos = gtk_drop_down_get_selected(data->dropdown_razza);
    const char *razza_scelta = gtk_string_list_get_string(razze_model, razza_pos);

    if (gtk_widget_is_visible(data->row_sottorazza)) {
        GtkStringList *sottorazze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_sottorazza));
        guint sottorazza_pos = gtk_drop_down_get_selected(data->dropdown_sottorazza);
        subrace_scelta = gtk_string_list_get_string(sottorazze_model, sottorazza_pos);
    }

    GtkStringList *classi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_classe));
    guint classe_pos = gtk_drop_down_get_selected(data->dropdown_classe);
    const char *classe_scelta = gtk_string_list_get_string(classi_model, classe_pos);

    GtkStringList *backgrounds_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_background));
    guint background_pos = gtk_drop_down_get_selected(data->dropdown_background);
    const char *background_scelto = gtk_string_list_get_string(backgrounds_model, background_pos);

    AdwNavigationPage *stats_page = create_stats_page(data, nome_scelto, genere_scelto, livello_scelto, razza_scelta, subrace_scelta, classe_scelta, background_scelto);
    adw_navigation_view_push(data->nav_view, stats_page);
}

// Callback per il pulsante "Indietro".
static void on_back_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    adw_navigation_view_pop(data->nav_view);
}

// Callback per il pulsante "Avanti" dalla pagina delle statistiche.
static void on_stats_avanti_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    AdwNavigationPage *skills_page = create_skills_page(data);
    adw_navigation_view_push(data->nav_view, skills_page);
}

// Callback per il pulsante finale "Genera Scheda".
static void on_generate_sheet_clicked(GtkButton *button, gpointer user_data) {
    g_print("Pulsante 'Genera Scheda' cliccato! La generazione del file .tex è in sviluppo...\n");
}


// Callback per il pulsante "Esci".
static void on_quit_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    gtk_window_destroy(data->main_window);
}

// Disconnessione segnali per la pagina delle statistiche.
static void disconnect_stats_data_signals(StatsPageData *stats_data) {
    // ... (omesso per brevità, nessuna modifica qui)
}

static void on_stats_page_destroy(GtkWidget *widget, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    // disconnect_stats_data_signals(stats_data); // Disabilitato temporaneamente per semplicità
    g_free(stats_data->race);
    g_free(stats_data->subrace);
    g_free(stats_data);
}

// Funzione che imposta i bonus razziali e la UI per la scelta
static void setup_racial_bonuses(StatsPageData *stats_data) {
    const char* stats_names[] = {"FOR", "DES", "COS", "INT", "SAG", "CAR"};
    stats_data->choice_bonus_count = 0;
    
    // Umano
    if (strcmp(stats_data->race, "Umano") == 0) {
        if (strcmp(stats_data->subrace, "Variante") == 0) {
            stats_data->choice_bonus_count = 2;
        }
    }
    // Mezzelfo
    else if (strcmp(stats_data->race, "Mezzelfo") == 0) {
        stats_data->choice_bonus_count = 2;
    }

    // Se ci sono bonus a scelta, crea la UI corrispondente
    if (stats_data->choice_bonus_count > 0) {
        gtk_widget_set_visible(stats_data->racial_choice_group, TRUE);
        char title[100];
        sprintf(title, "Scegli %d aumenti di caratteristica", stats_data->choice_bonus_count);
        adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(stats_data->racial_choice_group), title);

        for (int i = 0; i < 6; i++) {
            AdwActionRow *row = ADW_ACTION_ROW(adw_action_row_new());
            adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), stats_names[i]);
            stats_data->stat_choice_checks[i] = GTK_CHECK_BUTTON(gtk_check_button_new());
            adw_action_row_add_suffix(row, GTK_WIDGET(stats_data->stat_choice_checks[i]));
            adw_action_row_set_activatable_widget(row, GTK_WIDGET(stats_data->stat_choice_checks[i]));
            g_signal_connect(stats_data->stat_choice_checks[i], "toggled", G_CALLBACK(on_racial_bonus_choice_toggled), stats_data);
            adw_preferences_group_add(ADW_PREFERENCES_GROUP(stats_data->racial_choice_group), GTK_WIDGET(row));
        }
    }
}

// Genera la stringa descrittiva per i bonus razziali.
static char* get_racial_bonus_string(const char* race, const char* subrace) {
    char buffer[100] = "";

    // Umano
    if (strcmp(race, "Umano") == 0) {
        if (strcmp(subrace, "Normale") == 0) strcpy(buffer, "(+1 a tutte le caratteristiche)");
        else strcpy(buffer, "(+1 a due caratteristiche a scelta)");
    }
    // Elfo
    else if (strcmp(race, "Elfo") == 0) {
        if (strcmp(subrace, "Alto Elfo") == 0) strcpy(buffer, "(+2 DES, +1 INT)");
        else if (strcmp(subrace, "Elfo dei Boschi") == 0) strcpy(buffer, "(+2 DES, +1 SAG)");
        else if (strcmp(subrace, "Drow (Elfo Oscuro)") == 0) strcpy(buffer, "(+2 DES, +1 CAR)");
    }
    // Halfling
    else if (strcmp(race, "Halfling") == 0) {
        if (strcmp(subrace, "Piedelesto") == 0) strcpy(buffer, "(+2 DES, +1 CAR)");
        else if (strcmp(subrace, "Tozzo") == 0) strcpy(buffer, "(+2 DES, +1 COS)");
    }
    // Nano
    else if (strcmp(race, "Nano") == 0) {
        if (strcmp(subrace, "Nano delle Montagne") == 0) strcpy(buffer, "(+2 COS, +2 FOR)");
        else if (strcmp(subrace, "Nano delle Colline") == 0) strcpy(buffer, "(+2 COS, +1 SAG)");
    }
    // Dragonide
    else if (strcmp(race, "Dragonide") == 0) strcpy(buffer, "(+2 FOR, +1 CAR)");
    // Gnomo
    else if (strcmp(race, "Gnomo") == 0) {
        if (strcmp(subrace, "Gnomo delle Rocce") == 0) strcpy(buffer, "(+2 INT, +1 COS)");
        else if (strcmp(subrace, "Gnomo delle Foreste") == 0) strcpy(buffer, "(+2 INT, +1 DES)");
    }
    // Mezzelfo
    else if (strcmp(race, "Mezzelfo") == 0) strcpy(buffer, "(+2 CAR, +1 a due a scelta)");
    // Mezzorco
    else if (strcmp(race, "Mezzorco") == 0) strcpy(buffer, "(+2 FOR, +1 COS)");
    // Tiefling
    else if (strcmp(race, "Tiefling") == 0) strcpy(buffer, "(+2 CAR, +1 INT)");

    return g_strdup(buffer);
}


// Creazione della pagina delle statistiche con i dati di riepilogo.
static AdwNavigationPage* create_stats_page(AppData *data, const char* nome_scelto, const char* genere_scelto, int livello_scelto, const char* razza_scelta, const char* subrace_scelta, const char* classe_scelta, const char* background_scelto) {
    AdwNavigationPage *content;
    GtkWidget *page_vbox, *stats_hbox, *summary_group, *right_vbox, *button_box, *back_button;
    const char* stats_names[] = {"FOR", "DES", "COS", "INT", "SAG", "CAR", NULL};

    StatsPageData *stats_data = g_new0(StatsPageData, 1);
    stats_data->race = g_strdup(razza_scelta);
    stats_data->subrace = g_strdup(subrace_scelta);

    page_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(page_vbox, 24);
    gtk_widget_set_margin_end(page_vbox, 24);
    gtk_widget_set_margin_top(page_vbox, 24);
    gtk_widget_set_margin_bottom(page_vbox, 24);
    g_signal_connect(page_vbox, "destroy", G_CALLBACK(on_stats_page_destroy), stats_data);

    stats_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);
    gtk_widget_set_halign(stats_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(stats_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(stats_hbox, TRUE);

    // --- Gruppo Sinistro (Riepilogo Personaggio) ---
    summary_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(summary_group), "Riepilogo Personaggio");
    gtk_widget_set_size_request(summary_group, 280, -1);
    
    GtkWidget *row_nome = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_nome), "Nome");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_nome), strlen(nome_scelto) > 0 ? nome_scelto : "Senza nome");
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_nome);
    GtkWidget *row_genere = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_genere), "Genere");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_genere), genere_scelto);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_genere);
    GtkWidget *row_livello = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_livello), "Livello");
    char livello_str[4];
    sprintf(livello_str, "%d", livello_scelto);
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_livello), livello_str);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_livello);
    GtkWidget *row_competenza = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_competenza), "Bonus Competenza");
    int bonus_competenza = get_proficiency_bonus(livello_scelto);
    char bonus_str[4];
    sprintf(bonus_str, "+%d", bonus_competenza);
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_competenza), bonus_str);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_competenza);
    
    GtkWidget *row_razza = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_razza), "Razza");
    char razza_display[100];
    if (strlen(subrace_scelta) > 0) {
        sprintf(razza_display, "%s (%s)", razza_scelta, subrace_scelta);
    } else {
        strcpy(razza_display, razza_scelta);
    }
    char* bonus_string = get_racial_bonus_string(razza_scelta, subrace_scelta);
    char final_subtitle[256];
    sprintf(final_subtitle, "%s\n%s", razza_display, bonus_string);
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_razza), final_subtitle);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_razza);
    g_free(bonus_string);

    GtkWidget *row_classe = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_classe), "Classe");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_classe), classe_scelta);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_classe);
    GtkWidget *row_background = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_background), "Background");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_background), background_scelto);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_background);

    
    // --- Pannello Destro (Scelta Metodo Statistiche) ---
    right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

    GtkWidget *method_label = gtk_label_new("Metodo Punteggi");
    gtk_widget_set_halign(method_label, GTK_ALIGN_START);
    const char *methods[] = {"Point Buy", "Tira i Dadi", NULL};
    stats_data->method_dropdown = GTK_DROP_DOWN(gtk_drop_down_new_from_strings(methods));
    gtk_drop_down_set_selected(stats_data->method_dropdown, 0);
    g_signal_connect(stats_data->method_dropdown, "notify::selected-item", G_CALLBACK(on_method_changed), stats_data);

    gtk_box_append(GTK_BOX(right_vbox), method_label);
    gtk_box_append(GTK_BOX(right_vbox), GTK_WIDGET(stats_data->method_dropdown));

    // --- Contenitore per Point Buy ---
    stats_data->point_buy_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_box_append(GTK_BOX(right_vbox), stats_data->point_buy_box);
    
    stats_data->point_buy_label = gtk_label_new("Punti Rimanenti: 27");
    gtk_widget_add_css_class(stats_data->point_buy_label, "title-4");
    gtk_box_append(GTK_BOX(stats_data->point_buy_box), stats_data->point_buy_label);

    GtkWidget *pb_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(pb_grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(pb_grid), 12);
    GtkWidget *pb_total_header = gtk_label_new("<b>Totale</b>");
    gtk_label_set_use_markup(GTK_LABEL(pb_total_header), TRUE);
    gtk_grid_attach(GTK_GRID(pb_grid), pb_total_header, 2, 0, 1, 1);
    GtkWidget *pb_mod_header = gtk_label_new("<b>Mod.</b>");
    gtk_label_set_use_markup(GTK_LABEL(pb_mod_header), TRUE);
    gtk_grid_attach(GTK_GRID(pb_grid), pb_mod_header, 3, 0, 1, 1);

    for (int i = 0; stats_names[i] != NULL; ++i) {
        GtkWidget *label = gtk_label_new(stats_names[i]);
        stats_data->spin_rows_point_buy[i] = ADW_SPIN_ROW(adw_spin_row_new_with_range(8, 15, 1));
        adw_spin_row_set_value(stats_data->spin_rows_point_buy[i], 8);
        g_signal_connect(stats_data->spin_rows_point_buy[i], "notify::value", G_CALLBACK(on_point_buy_changed), stats_data);
        stats_data->total_score_labels_pb[i] = gtk_label_new("8");
        gtk_widget_add_css_class(stats_data->total_score_labels_pb[i], "title-3");
        stats_data->modifier_labels_pb[i] = gtk_label_new("-1");
        gtk_widget_add_css_class(stats_data->modifier_labels_pb[i], "title-3");

        gtk_grid_attach(GTK_GRID(pb_grid), label, 0, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(pb_grid), GTK_WIDGET(stats_data->spin_rows_point_buy[i]), 1, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(pb_grid), stats_data->total_score_labels_pb[i], 2, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(pb_grid), stats_data->modifier_labels_pb[i], 3, i + 1, 1, 1);
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

    stats_data->reset_button_rolling = gtk_button_new_with_label("Reset");
    gtk_widget_set_sensitive(stats_data->reset_button_rolling, FALSE);
    gtk_box_append(GTK_BOX(stats_data->rolling_box), stats_data->reset_button_rolling);
    g_signal_connect(stats_data->reset_button_rolling, "clicked", G_CALLBACK(on_reset_button_clicked), stats_data);

    GtkWidget *roll_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(roll_grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(roll_grid), 12);
    GtkWidget *roll_total_header = gtk_label_new("<b>Totale</b>");
    gtk_label_set_use_markup(GTK_LABEL(roll_total_header), TRUE);
    gtk_grid_attach(GTK_GRID(roll_grid), roll_total_header, 2, 0, 1, 1);
    GtkWidget *roll_mod_header = gtk_label_new("<b>Mod.</b>");
    gtk_label_set_use_markup(GTK_LABEL(roll_mod_header), TRUE);
    gtk_grid_attach(GTK_GRID(roll_grid), roll_mod_header, 3, 0, 1, 1);

    for (int i = 0; stats_names[i] != NULL; ++i) {
        GtkWidget *label = gtk_label_new(stats_names[i]);
        stats_data->stat_entries_rolling[i] = GTK_ENTRY(gtk_entry_new());
        gtk_editable_set_editable(GTK_EDITABLE(stats_data->stat_entries_rolling[i]), FALSE);
        gtk_entry_set_alignment(stats_data->stat_entries_rolling[i], 0.5);
        gtk_widget_add_css_class(GTK_WIDGET(stats_data->stat_entries_rolling[i]), "title-3");
        GtkDropTarget *target = gtk_drop_target_new(G_TYPE_STRING, GDK_ACTION_MOVE);
        g_signal_connect(target, "drop", G_CALLBACK(on_stat_drop), stats_data);
        gtk_widget_add_controller(GTK_WIDGET(stats_data->stat_entries_rolling[i]), GTK_EVENT_CONTROLLER(target));
        stats_data->total_score_labels_roll[i] = gtk_label_new("-");
        gtk_widget_add_css_class(stats_data->total_score_labels_roll[i], "title-3");
        stats_data->modifier_labels_roll[i] = gtk_label_new("-");
        gtk_widget_add_css_class(stats_data->modifier_labels_roll[i], "title-3");

        gtk_grid_attach(GTK_GRID(roll_grid), label, 0, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(roll_grid), GTK_WIDGET(stats_data->stat_entries_rolling[i]), 1, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(roll_grid), stats_data->total_score_labels_roll[i], 2, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(roll_grid), stats_data->modifier_labels_roll[i], 3, i + 1, 1, 1);
    }
    gtk_box_append(GTK_BOX(stats_data->rolling_box), roll_grid);

    // --- Gruppo per i Bonus a Scelta ---
    stats_data->racial_choice_group = adw_preferences_group_new();
    gtk_widget_set_visible(stats_data->racial_choice_group, FALSE);
    gtk_box_append(GTK_BOX(right_vbox), stats_data->racial_choice_group);
    
    gtk_box_append(GTK_BOX(stats_hbox), summary_group);
    gtk_box_append(GTK_BOX(stats_hbox), right_vbox);

    // --- Barra dei Pulsanti in basso ---
    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_halign(button_box, GTK_ALIGN_END);
    back_button = gtk_button_new_with_label("Indietro");
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_clicked), data);
    
    stats_data->forward_button = gtk_button_new_with_label("Avanti");
    gtk_widget_add_css_class(stats_data->forward_button, "suggested-action");
    gtk_widget_set_sensitive(stats_data->forward_button, FALSE); // Disabilitato di default
    g_signal_connect(stats_data->forward_button, "clicked", G_CALLBACK(on_stats_avanti_clicked), data);

    gtk_box_append(GTK_BOX(button_box), back_button);
    gtk_box_append(GTK_BOX(button_box), stats_data->forward_button);


    gtk_box_append(GTK_BOX(page_vbox), stats_hbox);
    gtk_box_append(GTK_BOX(page_vbox), button_box);
    
    // Imposta i bonus e aggiorna i totali
    setup_racial_bonuses(stats_data);
    update_total_scores(stats_data);
    update_forward_button_sensitivity(stats_data);

    content = adw_navigation_page_new(page_vbox, "Statistiche");
    return content;
}

// Funzione che gestisce l'aggiornamento della UI della sottorazza
static void on_race_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data) {
    AppData *data = (AppData*)user_data;
    GtkWidget *row_sottorazza = data->row_sottorazza;
    GtkDropDown *dropdown_sottorazza = data->dropdown_sottorazza;
    
    GtkStringList *razze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_razza));
    guint razza_pos = gtk_drop_down_get_selected(data->dropdown_razza);
    const char *razza_scelta = gtk_string_list_get_string(razze_model, razza_pos);

    const char **sottorazze = NULL;
    gboolean has_subrace = FALSE;

    if (strcmp(razza_scelta, "Elfo") == 0) {
        static const char *elf_subraces[] = {"Alto Elfo", "Elfo dei Boschi", "Drow (Elfo Oscuro)", NULL};
        sottorazze = elf_subraces;
        has_subrace = TRUE;
    } else if (strcmp(razza_scelta, "Nano") == 0) {
        static const char *dwarf_subraces[] = {"Nano delle Colline", "Nano delle Montagne", NULL};
        sottorazze = dwarf_subraces;
        has_subrace = TRUE;
    } else if (strcmp(razza_scelta, "Halfling") == 0) {
        static const char *halfling_subraces[] = {"Piedelesto", "Tozzo", NULL};
        sottorazze = halfling_subraces;
        has_subrace = TRUE;
    } else if (strcmp(razza_scelta, "Gnomo") == 0) {
        static const char *gnome_subraces[] = {"Gnomo delle Foreste", "Gnomo delle Rocce", NULL};
        sottorazze = gnome_subraces;
        has_subrace = TRUE;
    } else if (strcmp(razza_scelta, "Umano") == 0) {
        static const char *human_subraces[] = {"Normale", "Variante", NULL};
        sottorazze = human_subraces;
        has_subrace = TRUE;
    }

    gtk_widget_set_visible(row_sottorazza, has_subrace);
    if (has_subrace) {
        gtk_drop_down_set_model(dropdown_sottorazza, G_LIST_MODEL(gtk_string_list_new(sottorazze)));
        gtk_drop_down_set_selected(dropdown_sottorazza, 0);
    }
}


// Creazione della pagina iniziale di selezione.
static AdwNavigationPage* create_selections_page(AppData *data) {
    AdwNavigationPage *content;
    GtkWidget *content_box, *avanti_button;

    content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(content_box, 24);
    gtk_widget_set_margin_end(content_box, 24);
    gtk_widget_set_margin_top(content_box, 24);
    gtk_widget_set_margin_bottom(content_box, 24);

    GtkWidget *randomize_all_button = gtk_button_new_with_label("Tutto Casuale (tranne il livello)");
    gtk_widget_add_css_class(randomize_all_button, "suggested-action");
    gtk_widget_set_halign(randomize_all_button, GTK_ALIGN_CENTER);
    g_signal_connect(randomize_all_button, "clicked", G_CALLBACK(on_randomize_all_clicked), data);
    gtk_box_append(GTK_BOX(content_box), randomize_all_button);

    GtkWidget* label_nome = gtk_label_new("Nome Personaggio");
    gtk_widget_add_css_class(label_nome, "title-4");
    gtk_widget_set_halign(label_nome, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_nome);
    GtkWidget *name_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    data->entry_nome = GTK_ENTRY(gtk_entry_new());
    gtk_widget_set_hexpand(GTK_WIDGET(data->entry_nome), TRUE);
    GtkWidget *random_name_button = gtk_button_new_from_icon_name("view-refresh-symbolic");
    gtk_widget_set_tooltip_text(random_name_button, "Genera nome casuale");
    g_signal_connect(random_name_button, "clicked", G_CALLBACK(on_random_name_clicked), data);
    gtk_box_append(GTK_BOX(name_box), GTK_WIDGET(data->entry_nome));
    gtk_box_append(GTK_BOX(name_box), random_name_button);
    gtk_box_append(GTK_BOX(content_box), name_box);
    GtkWidget* label_genere = gtk_label_new("Genere");
    gtk_widget_add_css_class(label_genere, "title-4");
    gtk_widget_set_halign(label_genere, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_genere);
    const char *generi[] = {"Maschio", "Femmina", "Non-binario", "Agender", NULL};
    data->dropdown_genere = GTK_DROP_DOWN(gtk_drop_down_new_from_strings(generi));
    gtk_drop_down_set_selected(data->dropdown_genere, 0);
    gtk_box_append(GTK_BOX(content_box), GTK_WIDGET(data->dropdown_genere));
    GtkWidget* label_livello = gtk_label_new("Livello");
    gtk_widget_add_css_class(label_livello, "title-4");
    gtk_widget_set_halign(label_livello, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_livello);
    data->scale_livello = GTK_SCALE(gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 20, 1));
    gtk_scale_set_draw_value(data->scale_livello, TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(data->scale_livello), TRUE);
    for (int i = 1; i <= 20; i++) {
        if (i == 1 || i % 5 == 0 || i == 20) {
            char level_str[3];
            sprintf(level_str, "%d", i);
            gtk_scale_add_mark(data->scale_livello, i, GTK_POS_BOTTOM, level_str);
        }
    }
    gtk_range_set_value(GTK_RANGE(data->scale_livello), 1);
    gtk_box_append(GTK_BOX(content_box), GTK_WIDGET(data->scale_livello));
    GtkWidget* label_razza = gtk_label_new("Razza");
    gtk_widget_add_css_class(label_razza, "title-4");
    gtk_widget_set_halign(label_razza, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), label_razza);
    const char *razze[] = {"Umano", "Elfo", "Halfling", "Nano", "Dragonide", "Gnomo", "Mezzelfo", "Mezzorco", "Tiefling", NULL};
    data->dropdown_razza = GTK_DROP_DOWN(gtk_drop_down_new_from_strings(razze));
    gtk_drop_down_set_selected(data->dropdown_razza, 0);
    gtk_box_append(GTK_BOX(content_box), GTK_WIDGET(data->dropdown_razza));
    
    // --- Campo Sottorazza (dinamico) ---
    data->row_sottorazza = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(data->row_sottorazza), "Sottorazza");
    data->dropdown_sottorazza = GTK_DROP_DOWN(gtk_drop_down_new(NULL, NULL));
    adw_action_row_add_suffix(ADW_ACTION_ROW(data->row_sottorazza), GTK_WIDGET(data->dropdown_sottorazza));
    adw_action_row_set_activatable_widget(ADW_ACTION_ROW(data->row_sottorazza), GTK_WIDGET(data->dropdown_sottorazza));
    gtk_box_append(GTK_BOX(content_box), data->row_sottorazza);
    g_signal_connect(data->dropdown_razza, "notify::selected-item", G_CALLBACK(on_race_changed), data);
    on_race_changed(G_OBJECT(data->dropdown_razza), NULL, data); // Chiamata iniziale

    // Associazione dati per recupero successivo (semplificato ma mantenuto per ora)
    g_object_set_data(G_OBJECT(content_box), "subrace-row", data->row_sottorazza);
    g_object_set_data(G_OBJECT(data->dropdown_razza), "parent-page-box", content_box);


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
    gtk_window_set_default_size(GTK_WINDOW(window), -1, -1);

    // Aggiungi CSS Provider per lo stile personalizzato
    GtkCssProvider *provider = gtk_css_provider_new();
    const char *css =
        "checkbutton.saving-throw-check > check {"
        "  min-width: 22px;"
        "  min-height: 22px;"
        "  padding: 0;"
        "  border-radius: 0px;"
        "  clip-path: polygon(50% 0%, 100% 50%, 50% 100%, 0% 50%);"
        "}";
    gtk_css_provider_load_from_string(provider, css);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    g_object_unref(provider);


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

// --- Funzioni Helper ---

// Callback per il pulsante "Tutto Casuale".
static void on_randomize_all_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    GtkStringList *generi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_genere));
    guint generi_count = g_list_model_get_n_items(G_LIST_MODEL(generi_model));
    if (generi_count > 0) gtk_drop_down_set_selected(data->dropdown_genere, rand() % generi_count);
    GtkStringList *razze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_razza));
    guint razze_count = g_list_model_get_n_items(G_LIST_MODEL(razze_model));
    if (razze_count > 0) gtk_drop_down_set_selected(data->dropdown_razza, rand() % razze_count);
    GtkStringList *classi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_classe));
    guint classi_count = g_list_model_get_n_items(G_LIST_MODEL(classi_model));
    if (classi_count > 0) gtk_drop_down_set_selected(data->dropdown_classe, rand() % classi_count);
    GtkStringList *backgrounds_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_background));
    guint backgrounds_count = g_list_model_get_n_items(G_LIST_MODEL(backgrounds_model));
    if (backgrounds_count > 0) gtk_drop_down_set_selected(data->dropdown_background, rand() % backgrounds_count);
    on_random_name_clicked(NULL, data);
}

// Callback per il pulsante di generazione nome casuale.
static void on_random_name_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    GtkStringList *razze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_razza));
    guint razza_pos = gtk_drop_down_get_selected(data->dropdown_razza);
    const char *razza_scelta = gtk_string_list_get_string(razze_model, razza_pos);
    GtkStringList *generi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_genere));
    guint genere_pos = gtk_drop_down_get_selected(data->dropdown_genere);
    const char *genere_scelto = gtk_string_list_get_string(generi_model, genere_pos);
    const char* random_name = get_random_name(razza_scelta, genere_scelto);
    gtk_editable_set_text(GTK_EDITABLE(data->entry_nome), random_name);
}

// Gestisce il cambio di metodo per la generazione delle statistiche
static void on_method_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    guint selected = gtk_drop_down_get_selected(GTK_DROP_DOWN(source_object));
    gtk_widget_set_visible(stats_data->point_buy_box, selected == 0);
    gtk_widget_set_visible(stats_data->rolling_box, selected == 1);
    update_total_scores(stats_data);
    update_forward_button_sensitivity(stats_data);
}

// Logica per il Point Buy
static void on_point_buy_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    int costs[] = {0, 1, 2, 3, 4, 5, 7, 9}; // Costo *totale* per raggiungere un punteggio
    int total_cost = 0;
    for (int i = 0; i < 6; i++) {
        int value = adw_spin_row_get_value(stats_data->spin_rows_point_buy[i]);
        total_cost += costs[value - 8];
    }
    int budget = 27 - total_cost;
    char buffer[50];
    sprintf(buffer, "Punti Rimanenti: %d", budget);
    gtk_label_set_text(GTK_LABEL(stats_data->point_buy_label), buffer);

    // CORREZIONE BUG: Logica per bloccare l'aumento quando i punti non bastano
    int increment_costs[] = {1, 1, 1, 1, 1, 2, 2}; // Costo per passare da 8->9, 9->10, ..., 14->15
    for (int i = 0; i < 6; i++) {
        GtkAdjustment *adj = adw_spin_row_get_adjustment(stats_data->spin_rows_point_buy[i]);
        int current_value = adw_spin_row_get_value(stats_data->spin_rows_point_buy[i]);

        if (current_value < 15) {
            int cost_for_next_point = increment_costs[current_value - 8];
            if (budget < cost_for_next_point) {
                gtk_adjustment_set_upper(adj, current_value);
            } else {
                gtk_adjustment_set_upper(adj, 15);
            }
        } else { // Già al massimo
            gtk_adjustment_set_upper(adj, 15);
        }
    }

    gtk_widget_set_visible(stats_data->point_buy_label, budget >= 0);
    update_total_scores(stats_data);
    update_forward_button_sensitivity(stats_data);
}

// Logica per il lancio dei dadi
static void on_roll_dice_clicked(GtkButton *button, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    on_reset_button_clicked(NULL, stats_data);
    
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(stats_data->rolls_flowbox));
    while (child != NULL) {
        GtkWidget* next_child = gtk_widget_get_next_sibling(child);
        gtk_flow_box_remove(stats_data->rolls_flowbox, child);
        child = next_child;
    }

    for (int i = 0; i < 6; i++) {
        int rolls[4], sum = 0, min = 7;
        for (int j = 0; j < 4; j++) {
            rolls[j] = (rand() % 6) + 1;
            sum += rolls[j];
            if (rolls[j] < min) min = rolls[j];
        }
        GtkWidget *label = create_draggable_score_label(sum - min);
        gtk_flow_box_insert(stats_data->rolls_flowbox, label, -1);
    }
    gtk_widget_set_sensitive(stats_data->reset_button_rolling, TRUE);
}

// Crea un'etichetta trascinabile per un punteggio
static GtkWidget* create_draggable_score_label(int score) {
    char buffer[10];
    sprintf(buffer, "%d", score);
    GtkWidget *label = gtk_label_new(buffer);
    gtk_widget_add_css_class(label, "title-2");
    gtk_widget_set_margin_start(label, 6);
    gtk_widget_set_margin_end(label, 6);
    GtkDragSource *source = gtk_drag_source_new();
    gtk_drag_source_set_actions(source, GDK_ACTION_MOVE);
    g_signal_connect(source, "prepare", G_CALLBACK(on_drag_prepare), label);
    gtk_widget_add_controller(label, GTK_EVENT_CONTROLLER(source));
    return label;
}

// Prepara i dati per il drag
static GdkContentProvider* on_drag_prepare(GtkDragSource *source, double x, double y, gpointer user_data) {
    GtkWidget *label = GTK_WIDGET(user_data);
    const char *text = gtk_label_get_text(GTK_LABEL(label));
    char payload[100];
    sprintf(payload, "%s:%p", text, (void*)label);
    GValue value = G_VALUE_INIT;
    g_value_init(&value, G_TYPE_STRING);
    g_value_set_string(&value, payload);
    return gdk_content_provider_new_for_value(&value);
}

// Gestisce il drop di un punteggio su una caratteristica
static gboolean on_stat_drop(GtkDropTarget *target, const GValue *value, double x, double y, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    GtkEntry *entry = GTK_ENTRY(gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(target)));

    const char *payload = g_value_get_string(value);
    char payload_copy[100];
    strcpy(payload_copy, payload);

    char *new_score_str = strtok(payload_copy, ":");
    char *widget_addr_str = strtok(NULL, ":");
    if (!new_score_str || !widget_addr_str) return FALSE;

    GtkWidget *dragged_label = NULL;
    sscanf(widget_addr_str, "%p", (void**)&dragged_label);
    if (!dragged_label) return FALSE;

    GtkWidget *old_label = GTK_WIDGET(g_object_get_data(G_OBJECT(entry), "dragged-label"));
    if (old_label != NULL) {
        gtk_widget_set_sensitive(old_label, TRUE);
        gtk_widget_set_opacity(old_label, 1.0);
    }

    gtk_editable_set_text(GTK_EDITABLE(entry), new_score_str);
    g_object_set_data(G_OBJECT(entry), "dragged-label", dragged_label);
    gtk_widget_set_opacity(dragged_label, 0.4);
    gtk_widget_set_sensitive(dragged_label, FALSE);

    update_total_scores(stats_data);
    update_forward_button_sensitivity(stats_data);
    return TRUE;
}

// Callback per la selezione dei bonus razziali
static void on_racial_bonus_choice_toggled(GtkCheckButton *button, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    int checked_count = 0;
    for (int i = 0; i < 6; i++) {
        if (gtk_check_button_get_active(stats_data->stat_choice_checks[i])) {
            checked_count++;
        }
    }

    if (checked_count >= stats_data->choice_bonus_count) {
        for (int i = 0; i < 6; i++) {
            if (!gtk_check_button_get_active(stats_data->stat_choice_checks[i])) {
                gtk_widget_set_sensitive(GTK_WIDGET(stats_data->stat_choice_checks[i]), FALSE);
            }
        }
    } else {
        for (int i = 0; i < 6; i++) {
            gtk_widget_set_sensitive(GTK_WIDGET(stats_data->stat_choice_checks[i]), TRUE);
        }
    }
    update_total_scores(stats_data);
}

// Funzione centrale per aggiornare i punteggi totali
static void update_total_scores(StatsPageData *stats_data) {
    int base_scores[6] = {0};
    gboolean is_point_buy = gtk_widget_is_visible(stats_data->point_buy_box);

    // 1. Ottieni i punteggi base
    for (int i = 0; i < 6; i++) {
        if (is_point_buy) {
            base_scores[i] = adw_spin_row_get_value(stats_data->spin_rows_point_buy[i]);
        } else {
            const char *text = gtk_editable_get_text(GTK_EDITABLE(stats_data->stat_entries_rolling[i]));
            if (strlen(text) > 0) {
                base_scores[i] = atoi(text);
            }
        }
    }

    // 2. Calcola i bonus razziali
    int racial_bonuses[6] = {0};
    // Umano Normale
    if (strcmp(stats_data->race, "Umano") == 0 && strcmp(stats_data->subrace, "Normale") == 0) {
        for(int i=0; i<6; i++) racial_bonuses[i] = 1;
    }
    // Elfo
    else if (strcmp(stats_data->race, "Elfo") == 0) {
        racial_bonuses[1] += 2; // DES
        if (strcmp(stats_data->subrace, "Alto Elfo") == 0) racial_bonuses[3] += 1; // INT
        else if (strcmp(stats_data->subrace, "Elfo dei Boschi") == 0) racial_bonuses[4] += 1; // SAG
        else if (strcmp(stats_data->subrace, "Drow (Elfo Oscuro)") == 0) racial_bonuses[5] += 1; // CAR
    }
    // Halfling
    else if (strcmp(stats_data->race, "Halfling") == 0) {
        racial_bonuses[1] += 2; // DES
        if (strcmp(stats_data->subrace, "Piedelesto") == 0) racial_bonuses[5] += 1; // CAR
        else if (strcmp(stats_data->subrace, "Tozzo") == 0) racial_bonuses[2] += 1; // COS
    }
    // Nano
    else if (strcmp(stats_data->race, "Nano") == 0) {
        racial_bonuses[2] += 2; // COS
        if (strcmp(stats_data->subrace, "Nano delle Montagne") == 0) racial_bonuses[0] += 2; // FOR
        else if (strcmp(stats_data->subrace, "Nano delle Colline") == 0) racial_bonuses[4] += 1; // SAG
    }
    // Dragonide
    else if (strcmp(stats_data->race, "Dragonide") == 0) {
        racial_bonuses[0] += 2; // FOR
        racial_bonuses[5] += 1; // CAR
    }
    // Gnomo
    else if (strcmp(stats_data->race, "Gnomo") == 0) {
        racial_bonuses[3] += 2; // INT
        if (strcmp(stats_data->subrace, "Gnomo delle Rocce") == 0) racial_bonuses[2] += 1; // COS
        else if (strcmp(stats_data->subrace, "Gnomo delle Foreste") == 0) racial_bonuses[1] += 1; // DES
    }
    // Mezzelfo
    else if (strcmp(stats_data->race, "Mezzelfo") == 0) {
        racial_bonuses[5] += 2; // CAR
    }
    // Mezzorco
    else if (strcmp(stats_data->race, "Mezzorco") == 0) {
        racial_bonuses[0] += 1; // FOR
        racial_bonuses[2] += 1; // COS
    }
    // Tiefling
    else if (strcmp(stats_data->race, "Tiefling") == 0) {
        racial_bonuses[5] += 2; // CAR
        racial_bonuses[3] += 1; // INT
    }

    // 3. Aggiungi i bonus a scelta
    if (stats_data->choice_bonus_count > 0) {
        for (int i = 0; i < 6; i++) {
            if (gtk_check_button_get_active(stats_data->stat_choice_checks[i])) {
                racial_bonuses[i] += 1;
            }
        }
    }

    // 4. Aggiorna le etichette dei totali e dei modificatori
    for (int i = 0; i < 6; i++) {
        int total = base_scores[i] + racial_bonuses[i];
        int modifier = floor((double)(total - 10) / 2.0);
        char total_str[4];
        char modifier_str[5];
        sprintf(total_str, "%d", total);
        sprintf(modifier_str, "%+d", modifier);

        if (is_point_buy) {
            gtk_label_set_text(GTK_LABEL(stats_data->total_score_labels_pb[i]), total_str);
            gtk_label_set_text(GTK_LABEL(stats_data->modifier_labels_pb[i]), modifier_str);
        } else {
            if (base_scores[i] > 0) {
                gtk_label_set_text(GTK_LABEL(stats_data->total_score_labels_roll[i]), total_str);
                gtk_label_set_text(GTK_LABEL(stats_data->modifier_labels_roll[i]), modifier_str);
            } else {
                gtk_label_set_text(GTK_LABEL(stats_data->total_score_labels_roll[i]), "-");
                gtk_label_set_text(GTK_LABEL(stats_data->modifier_labels_roll[i]), "-");
            }
        }
    }
}

// Controlla se il pulsante "Avanti" deve essere attivo.
static void update_forward_button_sensitivity(StatsPageData *stats_data) {
    gboolean is_point_buy = gtk_widget_is_visible(stats_data->point_buy_box);
    gboolean is_sensitive = FALSE;

    if (is_point_buy) {
        int costs[] = {0, 1, 2, 3, 4, 5, 7, 9};
        int total_cost = 0;
        for (int i = 0; i < 6; i++) {
            int value = adw_spin_row_get_value(stats_data->spin_rows_point_buy[i]);
            total_cost += costs[value - 8];
        }
        int budget = 27 - total_cost;
        if (budget == 0) {
            is_sensitive = TRUE;
        }
    } else { // Rolling mode
        int filled_entries = 0;
        for (int i = 0; i < 6; i++) {
            const char *text = gtk_editable_get_text(GTK_EDITABLE(stats_data->stat_entries_rolling[i]));
            if (strlen(text) > 0) {
                filled_entries++;
            }
        }
        if (filled_entries == 6) {
            is_sensitive = TRUE;
        }
    }

    gtk_widget_set_sensitive(stats_data->forward_button, is_sensitive);
}

// Creazione della pagina delle competenze con layout migliorato
static AdwNavigationPage* create_skills_page(AppData *data) {
    // Contenitore principale della pagina con margini
    GtkWidget *page_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(page_vbox, 24);
    gtk_widget_set_margin_end(page_vbox, 24);
    gtk_widget_set_margin_top(page_vbox, 24);
    gtk_widget_set_margin_bottom(page_vbox, 24);

    // Finestra a scorrimento per contenere la lista di competenze
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_box_append(GTK_BOX(page_vbox), scrolled_window);

    // Box centrato per contenere la lista e forzare l'allineamento
    GtkWidget *center_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(center_box, GTK_ALIGN_CENTER);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), center_box);

    // Box per il contenuto con larghezza fissa
    GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_size_request(content_box, 500, -1); // Larghezza fissa per allineare i suffix
    gtk_box_append(GTK_BOX(center_box), content_box);


    // Etichetta di intestazione
    GtkWidget *header_label = gtk_label_new("Competenze e Tiri Salvezza");
    gtk_widget_add_css_class(header_label, "title-2");
    gtk_widget_set_halign(header_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content_box), header_label);

    // Struttura dati per le competenze
    struct SkillGroup {
        const char* characteristic;
        const char* skills[10]; // Array per contenere i nomi delle competenze
    };

    // Definizione delle competenze raggruppate per caratteristica
    const struct SkillGroup skill_data[] = {
        {"Forza", {"Atletica", NULL}},
        {"Destrezza", {"Acrobazia", "Furtività", "Rapidità di Mano", NULL}},
        {"Costituzione", {NULL}},
        {"Intelligenza", {"Arcano", "Indagare", "Natura", "Religione", "Storia", NULL}},
        {"Saggezza", {"Addestrare Animali", "Intuizione", "Medicina", "Percezione", "Sopravvivenza", NULL}},
        {"Carisma", {"Inganno", "Intimidire", "Intrattenere", "Persuasione", NULL}},
        {NULL, {NULL}} // Terminatore
    };

    // Ciclo per creare i gruppi di preferenze e le righe per ogni competenza
    for (int i = 0; skill_data[i].characteristic != NULL; i++) {
        GtkWidget *group = adw_preferences_group_new();
        adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(group), skill_data[i].characteristic);
        
        // Aggiunge una checkbox per il tiro salvezza nell'header del gruppo
        GtkWidget *saving_throw_check = gtk_check_button_new();
        gtk_widget_set_tooltip_text(saving_throw_check, "Competenza nel Tiro Salvezza");
        gtk_widget_add_css_class(saving_throw_check, "saving-throw-check"); // Aggiunge la classe CSS
        adw_preferences_group_set_header_suffix(ADW_PREFERENCES_GROUP(group), saving_throw_check);
        
        gtk_box_append(GTK_BOX(content_box), group);

        for (int j = 0; skill_data[i].skills[j] != NULL; j++) {
            AdwActionRow *row = ADW_ACTION_ROW(adw_action_row_new());
            adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), skill_data[i].skills[j]);
            
            GtkWidget *check = gtk_check_button_new();
            adw_action_row_add_suffix(row, check);
            adw_action_row_set_activatable_widget(row, check);

            adw_preferences_group_add(ADW_PREFERENCES_GROUP(group), GTK_WIDGET(row));
        }
    }

    // Barra dei pulsanti in basso
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_halign(button_box, GTK_ALIGN_END);
    
    GtkWidget *back_button = gtk_button_new_with_label("Indietro");
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_clicked), data);

    GtkWidget *generate_button = gtk_button_new_with_label("Genera Scheda");
    gtk_widget_add_css_class(generate_button, "suggested-action");
    g_signal_connect(generate_button, "clicked", G_CALLBACK(on_generate_sheet_clicked), data);

    gtk_box_append(GTK_BOX(button_box), back_button);
    gtk_box_append(GTK_BOX(button_box), generate_button);
    gtk_box_append(GTK_BOX(page_vbox), button_box);

    // Creazione della pagina di navigazione
    AdwNavigationPage *page = adw_navigation_page_new(page_vbox, "Competenze");
    return page;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    AdwApplication *app;
    int status;
    app = adw_application_new("com.esempio.dndgenerator", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
