#include <adwaita.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// --- DEFINIZIONI GLOBALI ---
const char* SKILL_NAMES[] = {
    "Atletica", "Acrobazia", "Furtività", "Rapidità di Mano", "Arcano", 
    "Indagare", "Natura", "Religione", "Storia", "Addestrare Animali", 
    "Intuizione", "Medicina", "Percezione", "Sopravvivenza", "Inganno", 
    "Intimidire", "Intrattenere", "Persuasione"
};
const int NUM_SKILLS = 18;

const char* STAT_NAMES_FULL[] = {
    "Forza", "Destrezza", "Costituzione", "Intelligenza", "Saggezza", "Carisma"
};
const int NUM_STATS = 6;

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

// --- STRUCT ---
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
    GtkWidget *avanti_button;
    GtkWidget *random_method_box; // NUOVO: Box per i pulsanti di generazione casuale
    int generated_stats[6];
    gboolean stats_are_generated;
} AppData;

// Struct per i dati specifici della pagina delle statistiche.
typedef struct {
    // Modalità di generazione
    GtkWidget *standard_array_box;
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
    GtkWidget *roll_button;
    GtkWidget *auto_assign_rolling_button;
    
    // Standard Array
    GtkFlowBox *standard_scores_flowbox;
    GtkEntry *stat_entries_standard[6];
    GtkWidget *total_score_labels_standard[6];
    GtkWidget *modifier_labels_standard[6];
    GtkWidget *reset_button_standard;
    GtkWidget *auto_assign_standard_button; // Rinominato

    // Comuni
    GtkDropDown *method_dropdown;
    char *race;
    char *subrace;
    char *class_name;
    int choice_bonus_count;
    GtkWidget *racial_choice_group;
    GtkCheckButton *stat_choice_checks[6];
    GtkWidget *forward_button;
} StatsPageData;

// Struct per i dati specifici della pagina delle abilità.
typedef struct {
    // Dati del personaggio
    char *race;
    char *subrace;
    char *class_name;
    char *background;
    int final_stats[6]; // NUOVO: Per conservare le statistiche finali

    // Elementi UI
    GtkCheckButton *st_checks[6];
    GtkCheckButton *skill_checks[18];
    AdwActionRow* skill_rows[18];
    GtkWidget *choice_label;

    // Stato
    int num_skill_choices_total;
    int num_skill_choices_made;
    GList *selectable_skills; 
} SkillsPageData;


// --- DICHIARAZIONI ANTICIPATE ---
static void update_total_scores(StatsPageData *stats_data);
static void update_forward_button_sensitivity(StatsPageData *stats_data);
static AdwNavigationPage* create_stats_page(AppData *data, const char* nome_scelto, const char* genere_scelto, int livello_scelto, const char* razza_scelta, const char* subrace_scelta, const char* classe_scelta, const char* background_scelto);
static AdwNavigationPage* create_skills_page(AppData *data, const char* razza_scelta, const char* subrace_scelta, const char* classe_scelta, const char* background_scelto);
static AdwNavigationPage* create_character_sheet_page(AppData *data, SkillsPageData *skills_data);
static void on_back_clicked(GtkButton *button, gpointer user_data);
static void on_stats_avanti_clicked(GtkButton *button, gpointer user_data);
static void on_generate_sheet_clicked(GtkButton *button, gpointer user_data);
static void on_new_character_clicked(GtkButton *button, gpointer user_data);
static void on_export_pdf_clicked(GtkButton *button, gpointer user_data);
static void on_quit_clicked(GtkButton *button, gpointer user_data);
static void on_roll_dice_clicked(GtkButton *button, gpointer user_data);
static void on_point_buy_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data);
static void on_method_changed(GObject *source_object, GParamSpec *pspec, gpointer user_data);
static void on_reset_button_clicked(GtkButton *button, gpointer user_data);
static void on_random_name_clicked(GtkButton *button, gpointer user_data);
static void on_randomize_all_clicked(GtkButton *button, gpointer user_data);
static void on_racial_bonus_choice_toggled(GtkCheckButton *button, gpointer user_data);
static char* get_racial_bonus_string(const char* race, const char* subrace);
static void on_auto_assign_standard_clicked(GtkButton *button, gpointer user_data);
static void on_auto_assign_rolling_clicked(GtkButton *button, gpointer user_data);
static void on_reset_standard_clicked(GtkButton *button, gpointer user_data);
static void on_random_method_chosen(GtkButton *button, gpointer user_data);


// Funzioni per il Drag-and-Drop
static GdkContentProvider* on_drag_prepare(GtkDragSource *source, double x, double y, gpointer user_data);
static gboolean on_stat_drop(GtkDropTarget *target, const GValue *value, double x, double y, gpointer user_data);
static GtkWidget* create_draggable_score_label(int score);

// --- FUNZIONI HELPER PER ABILITÀ ---
static int get_skill_index(const char* name) {
    for (int i = 0; i < NUM_SKILLS; i++) {
        if (strcmp(SKILL_NAMES[i], name) == 0) return i;
    }
    return -1;
}

static int get_st_index(const char* name) {
    for (int i = 0; i < NUM_STATS; i++) {
        if (strcmp(STAT_NAMES_FULL[i], name) == 0) return i;
    }
    return -1;
}


// --- CALLBACKS PAGINA ABILITÀ ---
static void on_skill_choice_toggled(GtkCheckButton *button, gpointer user_data);
static void apply_auto_proficiencies(SkillsPageData *page_data);

// Abilita la selezione manuale di tutte le competenze
static void on_manual_setup_clicked(GtkButton *button, gpointer user_data) {
    SkillsPageData *page_data = (SkillsPageData *)user_data;
    
    // Resetta e abilita tutto
    for (int i = 0; i < NUM_STATS; i++) {
        gtk_check_button_set_active(page_data->st_checks[i], FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(page_data->st_checks[i]), TRUE);
    }
    for (int i = 0; i < NUM_SKILLS; i++) {
        gtk_check_button_set_active(page_data->skill_checks[i], FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(page_data->skill_checks[i]), TRUE);
        gtk_widget_set_visible(GTK_WIDGET(page_data->skill_rows[i]), TRUE);
    }
    
    // Nascondi l'etichetta di scelta
    gtk_label_set_text(GTK_LABEL(page_data->choice_label), "");
    page_data->num_skill_choices_total = 0;
    page_data->num_skill_choices_made = 0;
    if (page_data->selectable_skills) {
        g_list_free(page_data->selectable_skills);
        page_data->selectable_skills = NULL;
    }
}

// Applica le competenze automatiche
static void on_auto_setup_clicked(GtkButton *button, gpointer user_data) {
    SkillsPageData *page_data = (SkillsPageData *)user_data;
    
    // Resetta tutto
    on_manual_setup_clicked(NULL, page_data);

    // Disabilita e nascondi tutto prima di applicare le regole
    for (int i = 0; i < NUM_STATS; i++) gtk_widget_set_sensitive(GTK_WIDGET(page_data->st_checks[i]), FALSE);
    for (int i = 0; i < NUM_SKILLS; i++) {
        gtk_widget_set_sensitive(GTK_WIDGET(page_data->skill_checks[i]), FALSE);
        gtk_widget_set_visible(GTK_WIDGET(page_data->skill_rows[i]), FALSE);
    }
    
    apply_auto_proficiencies(page_data);
}

// Gestisce il conteggio delle abilità a scelta
static void on_skill_choice_toggled(GtkCheckButton *button, gpointer user_data) {
    SkillsPageData *page_data = (SkillsPageData *)user_data;

    // Se il segnale è emesso da un reset (button == NULL), non fare nulla
    if (button == NULL) {
        char buffer[100];
        sprintf(buffer, "Scegli %d abilità (rimanenti: %d)", 
                page_data->num_skill_choices_total, 
                page_data->num_skill_choices_total - page_data->num_skill_choices_made);
        gtk_label_set_text(GTK_LABEL(page_data->choice_label), buffer);
        return;
    }

    if (gtk_check_button_get_active(button)) {
        page_data->num_skill_choices_made++;
    } else {
        page_data->num_skill_choices_made--;
    }

    // Aggiorna l'etichetta
    char buffer[100];
    sprintf(buffer, "Scegli %d abilità (rimanenti: %d)", 
            page_data->num_skill_choices_total, 
            page_data->num_skill_choices_total - page_data->num_skill_choices_made);
    gtk_label_set_text(GTK_LABEL(page_data->choice_label), buffer);

    // Blocca/sblocca le altre opzioni
    if (page_data->num_skill_choices_made >= page_data->num_skill_choices_total) {
        for (GList *l = page_data->selectable_skills; l != NULL; l = l->next) {
            int skill_idx = GPOINTER_TO_INT(l->data);
            if (!gtk_check_button_get_active(page_data->skill_checks[skill_idx])) {
                gtk_widget_set_sensitive(GTK_WIDGET(page_data->skill_checks[skill_idx]), FALSE);
            }
        }
    } else {
        for (GList *l = page_data->selectable_skills; l != NULL; l = l->next) {
            int skill_idx = GPOINTER_TO_INT(l->data);
            gtk_widget_set_sensitive(GTK_WIDGET(page_data->skill_checks[skill_idx]), TRUE);
        }
    }
}

// --- LOGICA DI GIOCO E UI ---

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
    
    // Mostra di nuovo il pulsante per lanciare i dadi e nascondi quello di assegnazione
    gtk_widget_set_visible(stats_data->roll_button, TRUE);
    gtk_widget_set_visible(stats_data->auto_assign_rolling_button, FALSE);
    gtk_widget_set_visible(GTK_WIDGET(stats_data->rolls_flowbox), TRUE);


    update_total_scores(stats_data);
    update_forward_button_sensitivity(stats_data);
}

// Logica di reset per il vettore standard
static void on_reset_standard_clicked(GtkButton *button, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    for (int i = 0; i < 6; i++) {
        GtkEntry *entry = stats_data->stat_entries_standard[i];
        GtkWidget *label_to_reenable = GTK_WIDGET(g_object_get_data(G_OBJECT(entry), "dragged-label"));
        if (label_to_reenable != NULL) {
            gtk_widget_set_sensitive(label_to_reenable, TRUE);
            gtk_widget_set_opacity(label_to_reenable, 1.0);
            g_object_set_data(G_OBJECT(entry), "dragged-label", NULL);
        }
        gtk_editable_set_text(GTK_EDITABLE(entry), "");
    }
    
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(stats_data->standard_scores_flowbox));
    while (child != NULL) {
        gtk_widget_set_visible(child, TRUE);
        gtk_widget_set_sensitive(child, TRUE);
        gtk_widget_set_opacity(child, 1.0);
        child = gtk_widget_get_next_sibling(child);
    }

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

    data->stats_are_generated = FALSE; // L'utente sta procedendo manualmente
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

    // Recupera i dati della pagina delle statistiche per calcolare i punteggi finali
    AdwNavigationPage* current_page = adw_navigation_view_get_visible_page(data->nav_view);
    GtkWidget* page_vbox = adw_navigation_page_get_child(current_page);
    StatsPageData *stats_data = (StatsPageData *)g_object_get_data(G_OBJECT(page_vbox), "stats_data");

    if (!stats_data) return; // Sicurezza

    // Calcola i punteggi finali e salvali in AppData
    update_total_scores(stats_data); // Assicura che le label siano aggiornate
    
    gboolean is_standard = gtk_widget_is_visible(stats_data->standard_array_box);
    gboolean is_point_buy = gtk_widget_is_visible(stats_data->point_buy_box);
    
    for(int i=0; i<6; i++) {
        const char* total_str;
        if(is_point_buy) total_str = gtk_label_get_text(GTK_LABEL(stats_data->total_score_labels_pb[i]));
        else if(is_standard) total_str = gtk_label_get_text(GTK_LABEL(stats_data->total_score_labels_standard[i]));
        else total_str = gtk_label_get_text(GTK_LABEL(stats_data->total_score_labels_roll[i]));
        data->generated_stats[i] = atoi(total_str);
    }
    data->stats_are_generated = TRUE;


    // Rileggi i dati dalla prima pagina perché potrebbero essere cambiati
    GtkStringList *razze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_razza));
    guint razza_pos = gtk_drop_down_get_selected(data->dropdown_razza);
    const char *razza_scelta = gtk_string_list_get_string(razze_model, razza_pos);

    const char *subrace_scelta = "";
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

    AdwNavigationPage *skills_page = create_skills_page(data, razza_scelta, subrace_scelta, classe_scelta, background_scelto);
    adw_navigation_view_push(data->nav_view, skills_page);
}

// Struttura per contenere tutti i dati del personaggio completo
typedef struct {
    char* name;
    char* race;
    char* subrace;
    char* class_name;
    char* background;
    char* gender;
    int level;
    int stats[6];
    int modifiers[6];
    int saving_throws[6];
    gboolean saving_throw_proficiencies[6];
    gboolean skill_proficiencies[18];
    int armor_class;
    int hit_points;
    int proficiency_bonus;
    int initiative;
    int speed;
} CharacterSheetData;

// Crea la pagina finale della scheda del personaggio
static AdwNavigationPage* create_character_sheet_page(AppData *data, SkillsPageData *skills_data) {
    // Raccoglie tutti i dati del personaggio
    CharacterSheetData *sheet_data = g_new0(CharacterSheetData, 1);
    
    // Informazioni base
    const char *nome = gtk_editable_get_text(GTK_EDITABLE(data->entry_nome));
    sheet_data->name = g_strdup(strlen(nome) > 0 ? nome : "Senza Nome");
    sheet_data->race = g_strdup(skills_data->race);
    sheet_data->subrace = g_strdup(skills_data->subrace);
    sheet_data->class_name = g_strdup(skills_data->class_name);
    sheet_data->background = g_strdup(skills_data->background);
    sheet_data->level = (int)gtk_range_get_value(GTK_RANGE(data->scale_livello));
    
    GtkStringList *generi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_genere));
    guint genere_pos = gtk_drop_down_get_selected(data->dropdown_genere);
    sheet_data->gender = g_strdup(gtk_string_list_get_string(generi_model, genere_pos));
    
    // Statistiche e modificatori
    for (int i = 0; i < 6; i++) {
        sheet_data->stats[i] = skills_data->final_stats[i];
        sheet_data->modifiers[i] = floor((double)(sheet_data->stats[i] - 10) / 2.0);
        
        // Tiri salvezza
        gboolean has_st_prof = gtk_check_button_get_active(skills_data->st_checks[i]);
        sheet_data->saving_throw_proficiencies[i] = has_st_prof;
        sheet_data->saving_throws[i] = sheet_data->modifiers[i] + (has_st_prof ? get_proficiency_bonus(sheet_data->level) : 0);
    }
    
    // Competenze abilità
    for (int i = 0; i < NUM_SKILLS; i++) {
        sheet_data->skill_proficiencies[i] = gtk_check_button_get_active(skills_data->skill_checks[i]);
    }
    
    // Calcoli derivati
    sheet_data->proficiency_bonus = get_proficiency_bonus(sheet_data->level);
    sheet_data->initiative = sheet_data->modifiers[1]; // Destrezza
    sheet_data->armor_class = 10 + sheet_data->modifiers[1]; // Base AC
    sheet_data->hit_points = 8 + sheet_data->modifiers[2]; // Base HP (d8 + CON mod)
    sheet_data->speed = 9; // Velocità base standard (9 metri = 30 piedi)
    
    // Adattamenti velocità per razza
    if (strcmp(sheet_data->race, "Nano") == 0 || strcmp(sheet_data->race, "Halfling") == 0) {
        sheet_data->speed = 7; // 7.5 metri (25 piedi) arrotondato a 7
    } else if (strcmp(sheet_data->race, "Elfo") == 0) {
        sheet_data->speed = 9; // 9 metri (30 piedi)
    }
    
    // Contenitore principale con scrolling
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(main_box, "character-sheet");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), main_box);
    
    // === HEADER DELLA SCHEDA ===
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(header_box, 32);
    gtk_widget_set_margin_end(header_box, 32);
    gtk_widget_set_margin_top(header_box, 24);
    gtk_widget_set_margin_bottom(header_box, 24);
    gtk_widget_add_css_class(header_box, "sheet-header");
    gtk_box_append(GTK_BOX(main_box), header_box);
    
    // Nome del personaggio
    GtkWidget *character_name = gtk_label_new(sheet_data->name);
    gtk_widget_add_css_class(character_name, "title-1");
    gtk_widget_add_css_class(character_name, "character-name");
    gtk_widget_set_halign(character_name, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(header_box), character_name);
    
    // Informazioni base in layout orizzontale
    GtkWidget *info_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(info_grid), 24);
    gtk_grid_set_row_spacing(GTK_GRID(info_grid), 8);
    gtk_widget_set_halign(info_grid, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(header_box), info_grid);
    
    char race_class_str[200];
    if (strlen(sheet_data->subrace) > 0) {
        sprintf(race_class_str, "%s (%s) %s", sheet_data->race, sheet_data->subrace, sheet_data->class_name);
    } else {
        sprintf(race_class_str, "%s %s", sheet_data->race, sheet_data->class_name);
    }
    
    char level_background_str[100];
    sprintf(level_background_str, "Livello %d • %s", sheet_data->level, sheet_data->background);
    
    GtkWidget *race_class_label = gtk_label_new(race_class_str);
    gtk_widget_add_css_class(race_class_label, "title-3");
    gtk_widget_add_css_class(race_class_label, "character-race-class");
    gtk_grid_attach(GTK_GRID(info_grid), race_class_label, 0, 0, 1, 1);
    
    GtkWidget *level_bg_label = gtk_label_new(level_background_str);
    gtk_widget_add_css_class(level_bg_label, "title-4");
    gtk_widget_add_css_class(level_bg_label, "character-details");
    gtk_grid_attach(GTK_GRID(info_grid), level_bg_label, 0, 1, 1, 1);
    
    // === STATISTICHE PRINCIPALI ===
    GtkWidget *stats_section = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_start(stats_section, 32);
    gtk_widget_set_margin_end(stats_section, 32);
    gtk_widget_set_margin_bottom(stats_section, 24);
    gtk_box_append(GTK_BOX(main_box), stats_section);
    
    GtkWidget *stats_title = gtk_label_new("Caratteristiche");
    gtk_widget_add_css_class(stats_title, "heading");
    gtk_widget_set_halign(stats_title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(stats_section), stats_title);
    
    // Griglia per le statistiche (3x2)
    GtkWidget *stats_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(stats_grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(stats_grid), 12);
    gtk_widget_set_halign(stats_grid, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(stats_section), stats_grid);
    
    for (int i = 0; i < 6; i++) {
        GtkWidget *stat_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        gtk_widget_add_css_class(stat_card, "stat-card");
        gtk_widget_set_size_request(stat_card, 120, 100);
        
        GtkWidget *stat_name = gtk_label_new(STAT_NAMES_FULL[i]);
        gtk_widget_add_css_class(stat_name, "caption");
        gtk_widget_add_css_class(stat_name, "dim-label");
        gtk_widget_set_halign(stat_name, GTK_ALIGN_CENTER);
        
        char modifier_str[10];
        sprintf(modifier_str, "%+d", sheet_data->modifiers[i]);
        GtkWidget *stat_modifier = gtk_label_new(modifier_str);
        gtk_widget_add_css_class(stat_modifier, "title-1");
        gtk_widget_add_css_class(stat_modifier, "stat-modifier");
        gtk_widget_set_halign(stat_modifier, GTK_ALIGN_CENTER);
        
        char value_str[10];
        sprintf(value_str, "%d", sheet_data->stats[i]);
        GtkWidget *stat_value = gtk_label_new(value_str);
        gtk_widget_add_css_class(stat_value, "title-4");
        gtk_widget_set_halign(stat_value, GTK_ALIGN_CENTER);
        
        gtk_box_append(GTK_BOX(stat_card), stat_name);
        gtk_box_append(GTK_BOX(stat_card), stat_modifier);
        gtk_box_append(GTK_BOX(stat_card), stat_value);
        
        int row = i / 3;
        int col = i % 3;
        gtk_grid_attach(GTK_GRID(stats_grid), stat_card, col, row, 1, 1);
    }
    
    // === STATISTICHE COMBATTIMENTO ===
    GtkWidget *combat_section = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_start(combat_section, 32);
    gtk_widget_set_margin_end(combat_section, 32);
    gtk_widget_set_margin_bottom(combat_section, 24);
    gtk_box_append(GTK_BOX(main_box), combat_section);
    
    GtkWidget *combat_title = gtk_label_new("Statistiche di Combattimento");
    gtk_widget_add_css_class(combat_title, "heading");
    gtk_widget_set_halign(combat_title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(combat_section), combat_title);
    
    GtkWidget *combat_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(combat_grid), 24);
    gtk_grid_set_row_spacing(GTK_GRID(combat_grid), 12);
    gtk_widget_set_halign(combat_grid, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(combat_section), combat_grid);
    
    // Carte per le statistiche di combattimento
    struct CombatStat {
        const char* name;
        int value;
        const char* unit;
    } combat_stats[] = {
        {"Classe Armatura", sheet_data->armor_class, ""},
        {"Punti Ferita", sheet_data->hit_points, ""},
        {"Iniziativa", sheet_data->initiative, ""},
        {"Velocità", sheet_data->speed, "piedi"},
        {"Bonus Competenza", sheet_data->proficiency_bonus, ""}
    };
    
    for (int i = 0; i < 5; i++) {
        GtkWidget *combat_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        gtk_widget_add_css_class(combat_card, "combat-stat-card");
        gtk_widget_set_size_request(combat_card, 140, 80);
        
        GtkWidget *combat_name = gtk_label_new(combat_stats[i].name);
        gtk_widget_add_css_class(combat_name, "caption");
        gtk_widget_add_css_class(combat_name, "dim-label");
        gtk_widget_set_halign(combat_name, GTK_ALIGN_CENTER);
        
        char value_str[20];
        if (strlen(combat_stats[i].unit) > 0) {
            sprintf(value_str, "%d %s", combat_stats[i].value, combat_stats[i].unit);
        } else {
            sprintf(value_str, "%d", combat_stats[i].value);
        }
        GtkWidget *combat_value = gtk_label_new(value_str);
        gtk_widget_add_css_class(combat_value, "title-2");
        gtk_widget_add_css_class(combat_value, "combat-value");
        gtk_widget_set_halign(combat_value, GTK_ALIGN_CENTER);
        
        gtk_box_append(GTK_BOX(combat_card), combat_name);
        gtk_box_append(GTK_BOX(combat_card), combat_value);
        
        int row = i / 3;
        int col = i % 3;
        gtk_grid_attach(GTK_GRID(combat_grid), combat_card, col, row, 1, 1);
    }
    
    // === TIRI SALVEZZA ===
    GtkWidget *saves_section = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_start(saves_section, 32);
    gtk_widget_set_margin_end(saves_section, 32);
    gtk_widget_set_margin_bottom(saves_section, 24);
    gtk_box_append(GTK_BOX(main_box), saves_section);
    
    GtkWidget *saves_title = gtk_label_new("Tiri Salvezza");
    gtk_widget_add_css_class(saves_title, "heading");
    gtk_widget_set_halign(saves_title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(saves_section), saves_title);
    
    GtkWidget *saves_group = adw_preferences_group_new();
    gtk_box_append(GTK_BOX(saves_section), saves_group);
    
    for (int i = 0; i < 6; i++) {
        AdwActionRow *save_row = ADW_ACTION_ROW(adw_action_row_new());
        adw_preferences_row_set_title(ADW_PREFERENCES_ROW(save_row), STAT_NAMES_FULL[i]);
        
        char save_str[10];
        sprintf(save_str, "%+d", sheet_data->saving_throws[i]);
        adw_action_row_set_subtitle(save_row, save_str);
        
        if (sheet_data->saving_throw_proficiencies[i]) {
            GtkWidget *prof_icon = gtk_image_new_from_icon_name("emblem-default-symbolic");
            gtk_widget_set_tooltip_text(prof_icon, "Competente");
            adw_action_row_add_suffix(save_row, prof_icon);
        }
        
        adw_preferences_group_add(ADW_PREFERENCES_GROUP(saves_group), GTK_WIDGET(save_row));
    }
    
    // === COMPETENZE ===
    GtkWidget *skills_section = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_start(skills_section, 32);
    gtk_widget_set_margin_end(skills_section, 32);
    gtk_widget_set_margin_bottom(skills_section, 24);
    gtk_box_append(GTK_BOX(main_box), skills_section);
    
    GtkWidget *skills_title = gtk_label_new("Competenze");
    gtk_widget_add_css_class(skills_title, "heading");
    gtk_widget_set_halign(skills_title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(skills_section), skills_title);
    
    GtkWidget *skills_group = adw_preferences_group_new();
    gtk_box_append(GTK_BOX(skills_section), skills_group);
    
    // Mappa abilità -> caratteristica
    int skill_to_stat[] = {0, 1, 1, 1, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5};
    
    for (int i = 0; i < NUM_SKILLS; i++) {
        if (sheet_data->skill_proficiencies[i]) {
            AdwActionRow *skill_row = ADW_ACTION_ROW(adw_action_row_new());
            adw_preferences_row_set_title(ADW_PREFERENCES_ROW(skill_row), SKILL_NAMES[i]);
            
            int skill_modifier = sheet_data->modifiers[skill_to_stat[i]] + sheet_data->proficiency_bonus;
            char skill_str[10];
            sprintf(skill_str, "%+d", skill_modifier);
            adw_action_row_set_subtitle(skill_row, skill_str);
            
            GtkWidget *prof_icon = gtk_image_new_from_icon_name("emblem-default-symbolic");
            gtk_widget_set_tooltip_text(prof_icon, "Competente");
            adw_action_row_add_suffix(skill_row, prof_icon);
            
            adw_preferences_group_add(ADW_PREFERENCES_GROUP(skills_group), GTK_WIDGET(skill_row));
        }
    }
    
    // === PULSANTI AZIONE ===
    GtkWidget *actions_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_halign(actions_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(actions_box, 32);
    gtk_widget_set_margin_end(actions_box, 32);
    gtk_widget_set_margin_bottom(actions_box, 32);
    gtk_box_append(GTK_BOX(main_box), actions_box);
    
    GtkWidget *new_character_btn = gtk_button_new_with_label("Nuovo Personaggio");
    gtk_widget_add_css_class(new_character_btn, "suggested-action");
    gtk_widget_add_css_class(new_character_btn, "pill");
    g_signal_connect(new_character_btn, "clicked", G_CALLBACK(on_new_character_clicked), data);
    
    GtkWidget *export_btn = gtk_button_new_with_label("Esporta PDF");
    gtk_widget_add_css_class(export_btn, "pill");
    g_object_set_data(G_OBJECT(export_btn), "sheet_data", sheet_data);
    g_signal_connect(export_btn, "clicked", G_CALLBACK(on_export_pdf_clicked), sheet_data);
    
    gtk_box_append(GTK_BOX(actions_box), new_character_btn);
    gtk_box_append(GTK_BOX(actions_box), export_btn);
    
    // Cleanup della memoria quando la pagina viene distrutta
    g_signal_connect_swapped(scrolled_window, "destroy", G_CALLBACK(g_free), sheet_data->name);
    g_signal_connect_swapped(scrolled_window, "destroy", G_CALLBACK(g_free), sheet_data->race);
    g_signal_connect_swapped(scrolled_window, "destroy", G_CALLBACK(g_free), sheet_data->subrace);
    g_signal_connect_swapped(scrolled_window, "destroy", G_CALLBACK(g_free), sheet_data->class_name);
    g_signal_connect_swapped(scrolled_window, "destroy", G_CALLBACK(g_free), sheet_data->background);
    g_signal_connect_swapped(scrolled_window, "destroy", G_CALLBACK(g_free), sheet_data->gender);
    g_signal_connect_swapped(scrolled_window, "destroy", G_CALLBACK(g_free), sheet_data);
    
    AdwNavigationPage *page = adw_navigation_page_new(scrolled_window, "Scheda Personaggio");
    return page;
}

// Callback per il pulsante finale "Genera Scheda".
static void on_generate_sheet_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    
    // Recupera i dati delle competenze dal pulsante stesso (dove vengono passati)
    SkillsPageData *skills_data = (SkillsPageData *)g_object_get_data(G_OBJECT(button), "skills_data");
    
    if (!skills_data) {
        // Se non riesce a recuperare dal pulsante, cerca nella pagina corrente
        AdwNavigationPage *current_page = adw_navigation_view_get_visible_page(data->nav_view);
        GtkWidget *page_content = adw_navigation_page_get_child(current_page);
        skills_data = (SkillsPageData *)g_object_get_data(G_OBJECT(page_content), "skills_data");
    }
    
    if (!skills_data) {
        g_warning("Impossibile recuperare i dati delle competenze");
        return;
    }
    
    // Crea e mostra la scheda del personaggio
    AdwNavigationPage *sheet_page = create_character_sheet_page(data, skills_data);
    adw_navigation_view_push(data->nav_view, sheet_page);
}


// Callback per il pulsante "Esci".
static void on_quit_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    gtk_window_destroy(data->main_window);
}

// Callback per il pulsante "Nuovo Personaggio"
static void on_new_character_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    
    // Torna alla prima pagina usando pop_to_tag o creando una nuova istanza
    while (adw_navigation_view_get_navigation_stack(data->nav_view) && 
           g_list_model_get_n_items(adw_navigation_view_get_navigation_stack(data->nav_view)) > 1) {
        adw_navigation_view_pop(data->nav_view);
    }
    
    // Reset dei dati
    gtk_editable_set_text(GTK_EDITABLE(data->entry_nome), "");
    gtk_drop_down_set_selected(data->dropdown_razza, 0);
    gtk_drop_down_set_selected(data->dropdown_classe, 0);
    gtk_drop_down_set_selected(data->dropdown_background, 0);
    gtk_drop_down_set_selected(data->dropdown_genere, 0);
    gtk_range_set_value(GTK_RANGE(data->scale_livello), 1);
    data->stats_are_generated = FALSE;
    memset(data->generated_stats, 0, sizeof(data->generated_stats));
}

// Callback per il pulsante "Esporta PDF"
static void on_export_pdf_clicked(GtkButton *button, gpointer user_data) {
    CharacterSheetData *sheet_data = (CharacterSheetData *)user_data;
    
    // Per ora mostra solo un messaggio, ma qui si potrebbe implementare l'export PDF
    AdwDialog *dialog = adw_alert_dialog_new("Export PDF", 
                                            "La funzionalità di export PDF sarà implementata in una versione futura.\n\n"
                                            "Per ora puoi fare uno screenshot della scheda o copiarla manualmente.");
    adw_alert_dialog_add_response(ADW_ALERT_DIALOG(dialog), "ok", "OK");
    adw_dialog_present(dialog, NULL);
}

// Distruttori per le pagine
static void on_stats_page_destroy(GtkWidget *widget, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    g_free(stats_data->race);
    g_free(stats_data->subrace);
    g_free(stats_data->class_name);
    g_free(stats_data);
}

static void on_skills_page_destroy(GtkWidget *widget, gpointer user_data) {
    SkillsPageData *page_data = (SkillsPageData *)user_data;
    g_free(page_data->race);
    g_free(page_data->subrace);
    g_free(page_data->class_name);
    g_free(page_data->background);
    if (page_data->selectable_skills) {
        g_list_free(page_data->selectable_skills);
    }
    g_free(page_data);
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
    stats_data->class_name = g_strdup(classe_scelta);

    page_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(page_vbox, 24);
    gtk_widget_set_margin_end(page_vbox, 24);
    gtk_widget_set_margin_top(page_vbox, 24);
    gtk_widget_set_margin_bottom(page_vbox, 24);
    g_signal_connect(page_vbox, "destroy", G_CALLBACK(on_stats_page_destroy), stats_data);
    g_object_set_data(G_OBJECT(page_vbox), "stats_data", stats_data); // Associa i dati alla pagina

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
    const char *methods[] = {"Vettore Standard", "Point Buy", "Tira i Dadi", NULL};
    stats_data->method_dropdown = GTK_DROP_DOWN(gtk_drop_down_new_from_strings(methods));
    gtk_drop_down_set_selected(stats_data->method_dropdown, 0);
    g_signal_connect(stats_data->method_dropdown, "notify::selected-item", G_CALLBACK(on_method_changed), stats_data);

    gtk_box_append(GTK_BOX(right_vbox), method_label);
    gtk_box_append(GTK_BOX(right_vbox), GTK_WIDGET(stats_data->method_dropdown));

    // --- Contenitore per Vettore Standard ---
    stats_data->standard_array_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_box_append(GTK_BOX(right_vbox), stats_data->standard_array_box);

    GtkWidget *standard_actions_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    stats_data->auto_assign_standard_button = gtk_button_new_with_label("Assegnazione Automatica");
    g_signal_connect(stats_data->auto_assign_standard_button, "clicked", G_CALLBACK(on_auto_assign_standard_clicked), stats_data);

    stats_data->standard_scores_flowbox = GTK_FLOW_BOX(gtk_flow_box_new());
    gtk_flow_box_set_max_children_per_line(stats_data->standard_scores_flowbox, 6);
    gtk_flow_box_set_selection_mode(stats_data->standard_scores_flowbox, GTK_SELECTION_NONE);

    int standard_scores[] = {15, 14, 13, 12, 10, 8};
    for (int i = 0; i < 6; i++) {
        GtkWidget *label = create_draggable_score_label(standard_scores[i]);
        gtk_flow_box_insert(stats_data->standard_scores_flowbox, label, -1);
    }

    gtk_box_append(GTK_BOX(standard_actions_box), stats_data->auto_assign_standard_button);
    gtk_box_append(GTK_BOX(standard_actions_box), GTK_WIDGET(stats_data->standard_scores_flowbox));
    gtk_box_append(GTK_BOX(stats_data->standard_array_box), standard_actions_box);

    stats_data->reset_button_standard = gtk_button_new_with_label("Reset");
    gtk_box_append(GTK_BOX(stats_data->standard_array_box), stats_data->reset_button_standard);
    g_signal_connect(stats_data->reset_button_standard, "clicked", G_CALLBACK(on_reset_standard_clicked), stats_data);

    GtkWidget *standard_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(standard_grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(standard_grid), 12);
    GtkWidget *standard_total_header = gtk_label_new("<b>Totale</b>");
    gtk_label_set_use_markup(GTK_LABEL(standard_total_header), TRUE);
    gtk_grid_attach(GTK_GRID(standard_grid), standard_total_header, 2, 0, 1, 1);
    GtkWidget *standard_mod_header = gtk_label_new("<b>Mod.</b>");
    gtk_label_set_use_markup(GTK_LABEL(standard_mod_header), TRUE);
    gtk_grid_attach(GTK_GRID(standard_grid), standard_mod_header, 3, 0, 1, 1);

    for (int i = 0; stats_names[i] != NULL; ++i) {
        GtkWidget *label = gtk_label_new(stats_names[i]);
        stats_data->stat_entries_standard[i] = GTK_ENTRY(gtk_entry_new());
        gtk_editable_set_editable(GTK_EDITABLE(stats_data->stat_entries_standard[i]), FALSE);
        gtk_entry_set_alignment(stats_data->stat_entries_standard[i], 0.5);
        gtk_widget_add_css_class(GTK_WIDGET(stats_data->stat_entries_standard[i]), "title-3");
        GtkDropTarget *target = gtk_drop_target_new(G_TYPE_STRING, GDK_ACTION_MOVE);
        g_signal_connect(target, "drop", G_CALLBACK(on_stat_drop), stats_data);
        gtk_widget_add_controller(GTK_WIDGET(stats_data->stat_entries_standard[i]), GTK_EVENT_CONTROLLER(target));
        stats_data->total_score_labels_standard[i] = gtk_label_new("-");
        gtk_widget_add_css_class(stats_data->total_score_labels_standard[i], "title-3");
        stats_data->modifier_labels_standard[i] = gtk_label_new("-");
        gtk_widget_add_css_class(stats_data->modifier_labels_standard[i], "title-3");

        gtk_grid_attach(GTK_GRID(standard_grid), label, 0, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(standard_grid), GTK_WIDGET(stats_data->stat_entries_standard[i]), 1, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(standard_grid), stats_data->total_score_labels_standard[i], 2, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(standard_grid), stats_data->modifier_labels_standard[i], 3, i + 1, 1, 1);
    }
    gtk_box_append(GTK_BOX(stats_data->standard_array_box), standard_grid);


    // --- Contenitore per Point Buy ---
    stats_data->point_buy_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_box_append(GTK_BOX(right_vbox), stats_data->point_buy_box);
    gtk_widget_set_visible(stats_data->point_buy_box, FALSE);
    
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
    stats_data->roll_button = gtk_button_new_with_label("Lancia i Dadi");
    stats_data->auto_assign_rolling_button = gtk_button_new_with_label("Assegnazione Automatica");
    gtk_widget_set_visible(stats_data->auto_assign_rolling_button, FALSE);

    stats_data->rolls_flowbox = GTK_FLOW_BOX(gtk_flow_box_new());
    gtk_flow_box_set_max_children_per_line(stats_data->rolls_flowbox, 6);
    gtk_flow_box_set_selection_mode(stats_data->rolls_flowbox, GTK_SELECTION_NONE);
    g_signal_connect(stats_data->roll_button, "clicked", G_CALLBACK(on_roll_dice_clicked), stats_data);
    g_signal_connect(stats_data->auto_assign_rolling_button, "clicked", G_CALLBACK(on_auto_assign_rolling_clicked), stats_data);
    
    gtk_box_append(GTK_BOX(roll_actions_box), stats_data->roll_button);
    gtk_box_append(GTK_BOX(roll_actions_box), stats_data->auto_assign_rolling_button);
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
    GtkWidget *content_box;

    content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(content_box, 24);
    gtk_widget_set_margin_end(content_box, 24);
    gtk_widget_set_margin_top(content_box, 24);
    gtk_widget_set_margin_bottom(content_box, 24);

    GtkWidget *randomize_all_button = gtk_button_new_with_label("Personaggio Casuale");
    gtk_widget_add_css_class(randomize_all_button, "suggested-action");
    gtk_widget_set_halign(randomize_all_button, GTK_ALIGN_CENTER);
    g_signal_connect(randomize_all_button, "clicked", G_CALLBACK(on_randomize_all_clicked), data);
    gtk_box_append(GTK_BOX(content_box), randomize_all_button);

    // --- Box per i pulsanti di generazione casuale ---
    data->random_method_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_set_halign(data->random_method_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(data->random_method_box, 6);
    gtk_widget_set_margin_bottom(data->random_method_box, 6);

    GtkWidget *roll_dice_button = gtk_button_new_with_label("Tiro dei Dadi");
    g_object_set_data(G_OBJECT(roll_dice_button), "app_data", data);
    g_signal_connect(roll_dice_button, "clicked", G_CALLBACK(on_random_method_chosen), GINT_TO_POINTER(0)); // 0 per dadi

    GtkWidget *standard_array_button = gtk_button_new_with_label("Vettore Standard");
    g_object_set_data(G_OBJECT(standard_array_button), "app_data", data);
    g_signal_connect(standard_array_button, "clicked", G_CALLBACK(on_random_method_chosen), GINT_TO_POINTER(1)); // 1 per vettore

    gtk_box_append(GTK_BOX(data->random_method_box), roll_dice_button);
    gtk_box_append(GTK_BOX(data->random_method_box), standard_array_button);
    gtk_box_append(GTK_BOX(content_box), data->random_method_box);
    gtk_widget_set_visible(data->random_method_box, FALSE); // Nascosto all'inizio

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

    data->avanti_button = gtk_button_new_with_label("Avanti");
    gtk_widget_add_css_class(data->avanti_button, "suggested-action");
    gtk_widget_set_valign(data->avanti_button, GTK_ALIGN_END);
    gtk_widget_set_vexpand(data->avanti_button, TRUE);
    g_signal_connect(data->avanti_button, "clicked", G_CALLBACK(on_avanti_clicked), data);
    gtk_box_append(GTK_BOX(content_box), data->avanti_button);

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
        "  -gtk-icon-source: none;"
        "  background-image: none;"
        "  border-radius: 0;"
        "  min-width: 22px;"
        "  min-height: 22px;"
        "  padding: 0;"
        "  clip-path: polygon(50% 0%, 100% 50%, 50% 100%, 0% 50%);"
        "}"
        "checkbutton.saving-throw-check:checked > check {"
        "  background-color: @theme_selected_bg_color;"
        "}"
        ""
        "/* Stili per la scheda del personaggio */"
        ".character-sheet {"
        "  background: linear-gradient(135deg, @theme_bg_color, alpha(@theme_selected_bg_color, 0.1));"
        "}"
        ""
        ".sheet-header {"
        "  background: linear-gradient(90deg, alpha(@theme_selected_bg_color, 0.2), alpha(@accent_color, 0.1));"
        "  border-radius: 12px;"
        "  box-shadow: 0 2px 8px alpha(@theme_fg_color, 0.1);"
        "}"
        ""
        ".character-name {"
        "  color: @theme_selected_bg_color;"
        "  font-weight: bold;"
        "  text-shadow: 0 1px 2px alpha(@theme_fg_color, 0.2);"
        "}"
        ""
        ".character-race-class {"
        "  color: @accent_color;"
        "  font-weight: 600;"
        "}"
        ""
        ".character-details {"
        "  color: alpha(@theme_fg_color, 0.8);"
        "}"
        ""
        ".stat-card {"
        "  background: @theme_base_color;"
        "  border: 1px solid alpha(@theme_fg_color, 0.1);"
        "  border-radius: 8px;"
        "  padding: 12px;"
        "  margin: 4px;"
        "  box-shadow: 0 1px 4px alpha(@theme_fg_color, 0.05);"
        "  transition: all 200ms ease;"
        "}"
        ""
        ".stat-card:hover {"
        "  box-shadow: 0 2px 8px alpha(@theme_fg_color, 0.1);"
        "  transform: translateY(-1px);"
        "}"
        ""
        ".stat-modifier {"
        "  color: @accent_color;"
        "  font-weight: bold;"
        "}"
        ""
        ".combat-stat-card {"
        "  background: linear-gradient(135deg, @theme_base_color, alpha(@accent_color, 0.05));"
        "  border: 1px solid alpha(@accent_color, 0.2);"
        "  border-radius: 8px;"
        "  padding: 12px;"
        "  margin: 4px;"
        "  box-shadow: 0 1px 4px alpha(@accent_color, 0.1);"
        "  transition: all 200ms ease;"
        "}"
        ""
        ".combat-stat-card:hover {"
        "  box-shadow: 0 2px 8px alpha(@accent_color, 0.15);"
        "  transform: translateY(-1px);"
        "}"
        ""
        ".combat-value {"
        "  color: @accent_color;"
        "  font-weight: bold;"
        "}"
        ""
        "button.pill {"
        "  border-radius: 20px;"
        "  padding: 12px 24px;"
        "  font-weight: 600;"
        "}"
        ""
        ".heading {"
        "  color: @theme_selected_bg_color;"
        "  font-weight: bold;"
        "  border-bottom: 2px solid alpha(@theme_selected_bg_color, 0.3);"
        "  padding-bottom: 8px;"
        "  margin-bottom: 12px;"
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

    // Mostra i pulsanti di generazione e nascondi "Avanti"
    gtk_widget_set_visible(data->avanti_button, FALSE);
    gtk_widget_set_visible(data->random_method_box, TRUE);
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
    gtk_widget_set_visible(stats_data->standard_array_box, selected == 0);
    gtk_widget_set_visible(stats_data->point_buy_box, selected == 1);
    gtk_widget_set_visible(stats_data->rolling_box, selected == 2);
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

    // Nascondi il pulsante di lancio e mostra quello di assegnazione automatica
    gtk_widget_set_visible(GTK_WIDGET(button), FALSE);
    gtk_widget_set_visible(stats_data->auto_assign_rolling_button, TRUE);
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
    gboolean is_standard = gtk_widget_is_visible(stats_data->standard_array_box);
    gboolean is_point_buy = gtk_widget_is_visible(stats_data->point_buy_box);
    gboolean is_rolling = gtk_widget_is_visible(stats_data->rolling_box);

    // 1. Ottieni i punteggi base
    for (int i = 0; i < 6; i++) {
        if (is_point_buy) {
            base_scores[i] = adw_spin_row_get_value(stats_data->spin_rows_point_buy[i]);
        } else if (is_rolling) {
            const char *text = gtk_editable_get_text(GTK_EDITABLE(stats_data->stat_entries_rolling[i]));
            if (strlen(text) > 0) base_scores[i] = atoi(text);
        } else { // Standard Array
            const char *text = gtk_editable_get_text(GTK_EDITABLE(stats_data->stat_entries_standard[i]));
            if (strlen(text) > 0) base_scores[i] = atoi(text);
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
        racial_bonuses[0] += 2; // FOR
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
        } else if (is_rolling) {
            if (base_scores[i] > 0) {
                gtk_label_set_text(GTK_LABEL(stats_data->total_score_labels_roll[i]), total_str);
                gtk_label_set_text(GTK_LABEL(stats_data->modifier_labels_roll[i]), modifier_str);
            } else {
                gtk_label_set_text(GTK_LABEL(stats_data->total_score_labels_roll[i]), "-");
                gtk_label_set_text(GTK_LABEL(stats_data->modifier_labels_roll[i]), "-");
            }
        } else { // Standard Array
             if (base_scores[i] > 0) {
                gtk_label_set_text(GTK_LABEL(stats_data->total_score_labels_standard[i]), total_str);
                gtk_label_set_text(GTK_LABEL(stats_data->modifier_labels_standard[i]), modifier_str);
            } else {
                gtk_label_set_text(GTK_LABEL(stats_data->total_score_labels_standard[i]), "-");
                gtk_label_set_text(GTK_LABEL(stats_data->modifier_labels_standard[i]), "-");
            }
        }
    }
}

// Controlla se il pulsante "Avanti" deve essere attivo.
static void update_forward_button_sensitivity(StatsPageData *stats_data) {
    gboolean is_standard = gtk_widget_is_visible(stats_data->standard_array_box);
    gboolean is_point_buy = gtk_widget_is_visible(stats_data->point_buy_box);
    gboolean is_rolling = gtk_widget_is_visible(stats_data->rolling_box);
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
    } else if (is_rolling || is_standard) { 
        int filled_entries = 0;
        for (int i = 0; i < 6; i++) {
            GtkEntry* entry = is_rolling ? stats_data->stat_entries_rolling[i] : stats_data->stat_entries_standard[i];
            const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));
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

// Creazione della pagina delle competenze con layout migliorato e riepilogo
static AdwNavigationPage* create_skills_page(AppData *data, const char* razza_scelta, const char* subrace_scelta, const char* classe_scelta, const char* background_scelto) {
    // Contenitore principale della pagina
    GtkWidget *page_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start(page_vbox, 24);
    gtk_widget_set_margin_end(page_vbox, 24);
    gtk_widget_set_margin_top(page_vbox, 24);
    gtk_widget_set_margin_bottom(page_vbox, 24);

    SkillsPageData *page_data = g_new0(SkillsPageData, 1);
    page_data->race = g_strdup(razza_scelta);
    page_data->subrace = g_strdup(subrace_scelta);
    page_data->class_name = g_strdup(classe_scelta);
    page_data->background = g_strdup(background_scelto);
    if(data->stats_are_generated) {
        memcpy(page_data->final_stats, data->generated_stats, sizeof(data->generated_stats));
    }
    g_signal_connect(page_vbox, "destroy", G_CALLBACK(on_skills_page_destroy), page_data);
    
    // Collega i dati delle competenze al contenitore per poterli recuperare dopo
    g_object_set_data(G_OBJECT(page_vbox), "skills_data", page_data);

    // Box orizzontale per il contenuto principale (Riepilogo | Competenze)
    GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);
    gtk_widget_set_vexpand(main_hbox, TRUE);
    gtk_box_append(GTK_BOX(page_vbox), main_hbox);

    // --- PANNELLO SINISTRO: RIEPILOGO ---
    GtkWidget *summary_scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(summary_scrolled_window, 320, -1);
    gtk_box_append(GTK_BOX(main_hbox), summary_scrolled_window);
    
    GtkWidget *summary_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(summary_scrolled_window), summary_vbox);

    // Gruppo Info Personaggio
    GtkWidget *summary_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(summary_group), "Riepilogo");
    gtk_box_append(GTK_BOX(summary_vbox), summary_group);

    // Recupera dati aggiuntivi da AppData
    const char *nome_scelto = gtk_editable_get_text(GTK_EDITABLE(data->entry_nome));
    int livello_scelto = (int)gtk_range_get_value(GTK_RANGE(data->scale_livello));
    GtkStringList *generi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_genere));
    guint genere_pos = gtk_drop_down_get_selected(data->dropdown_genere);
    const char *genere_scelto = gtk_string_list_get_string(generi_model, genere_pos);

    // Popola le righe del riepilogo
    GtkWidget *row_nome = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_nome), "Nome");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_nome), strlen(nome_scelto) > 0 ? nome_scelto : "Senza nome");
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_nome);

    char razza_display[100];
    if (strlen(subrace_scelta) > 0) {
        sprintf(razza_display, "%s (%s)", razza_scelta, subrace_scelta);
    } else {
        strcpy(razza_display, razza_scelta);
    }
    GtkWidget *row_razza_classe = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_razza_classe), "Razza e Classe");
    char razza_classe_str[200];
    sprintf(razza_classe_str, "%s %s", razza_display, classe_scelta);
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_razza_classe), razza_classe_str);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_razza_classe);

    GtkWidget *row_liv_back = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row_liv_back), "Livello e Background");
    char liv_back_str[200];
    sprintf(liv_back_str, "Livello %d, %s", livello_scelto, background_scelto);
    adw_action_row_set_subtitle(ADW_ACTION_ROW(row_liv_back), liv_back_str);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(summary_group), row_liv_back);

    // Gruppo Caratteristiche
    GtkWidget *stats_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(stats_group), "Caratteristiche");
    gtk_box_append(GTK_BOX(summary_vbox), stats_group);

    for (int i = 0; i < NUM_STATS; i++) {
        AdwActionRow *stat_row = ADW_ACTION_ROW(adw_action_row_new());
        adw_preferences_row_set_title(ADW_PREFERENCES_ROW(stat_row), STAT_NAMES_FULL[i]);

        int total = page_data->final_stats[i];
        int modifier = floor((double)(total - 10) / 2.0);

        char subtitle[20];
        sprintf(subtitle, "%d (%+d)", total, modifier);
        adw_action_row_set_subtitle(stat_row, subtitle);
        gtk_widget_add_css_class(GTK_WIDGET(stat_row), "title-4");

        adw_preferences_group_add(ADW_PREFERENCES_GROUP(stats_group), GTK_WIDGET(stat_row));
    }

    // --- PANNELLO DESTRO: COMPETENZE ---
    GtkWidget *skills_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_hexpand(skills_vbox, TRUE);
    gtk_box_append(GTK_BOX(main_hbox), skills_vbox);
    
    // Box per i pulsanti di modalità
    GtkWidget *mode_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_set_halign(mode_box, GTK_ALIGN_CENTER);
    GtkWidget *auto_button = gtk_button_new_with_label("Selezione Automatica");
    GtkWidget *manual_button = gtk_button_new_with_label("Selezione Manuale");
    gtk_box_append(GTK_BOX(mode_box), auto_button);
    gtk_box_append(GTK_BOX(mode_box), manual_button);
    gtk_box_append(GTK_BOX(skills_vbox), mode_box);

    g_signal_connect(auto_button, "clicked", G_CALLBACK(on_auto_setup_clicked), page_data);
    g_signal_connect(manual_button, "clicked", G_CALLBACK(on_manual_setup_clicked), page_data);

    // Etichetta per le scelte rimanenti
    page_data->choice_label = gtk_label_new("");
    gtk_widget_add_css_class(page_data->choice_label, "title-4");
    gtk_box_append(GTK_BOX(skills_vbox), page_data->choice_label);

    // Finestra a scorrimento per contenere la lista di competenze
    GtkWidget *skills_scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(skills_scrolled_window, TRUE);
    gtk_box_append(GTK_BOX(skills_vbox), skills_scrolled_window);

    // Box per il contenuto delle competenze
    GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(skills_scrolled_window), content_box);

    // Struttura dati per le competenze
    struct SkillGroup {
        const char* characteristic;
        const char* skills[10];
    };

    const struct SkillGroup skill_data[] = {
        {"Forza", {"Atletica", NULL}},
        {"Destrezza", {"Acrobazia", "Furtività", "Rapidità di Mano", NULL}},
        {"Costituzione", {NULL}},
        {"Intelligenza", {"Arcano", "Indagare", "Natura", "Religione", "Storia", NULL}},
        {"Saggezza", {"Addestrare Animali", "Intuizione", "Medicina", "Percezione", "Sopravvivenza", NULL}},
        {"Carisma", {"Inganno", "Intimidire", "Intrattenere", "Persuasione", NULL}},
        {NULL, {NULL}} 
    };

    // Ciclo per creare i gruppi di preferenze e le righe per ogni competenza
    int current_skill_idx = 0;
    for (int i = 0; skill_data[i].characteristic != NULL; i++) {
        GtkWidget *group = adw_preferences_group_new();
        adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(group), skill_data[i].characteristic);
        
        GtkWidget *saving_throw_check = gtk_check_button_new();
        page_data->st_checks[i] = GTK_CHECK_BUTTON(saving_throw_check);
        gtk_widget_set_tooltip_text(saving_throw_check, "Competenza nel Tiro Salvezza");
        gtk_widget_add_css_class(saving_throw_check, "saving-throw-check");
        adw_preferences_group_set_header_suffix(ADW_PREFERENCES_GROUP(group), saving_throw_check);
        
        gtk_box_append(GTK_BOX(content_box), group);

        for (int j = 0; skill_data[i].skills[j] != NULL; j++) {
            AdwActionRow *row = ADW_ACTION_ROW(adw_action_row_new());
            page_data->skill_rows[current_skill_idx] = row;
            adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), skill_data[i].skills[j]);
            
            GtkWidget *check = gtk_check_button_new();
            page_data->skill_checks[current_skill_idx++] = GTK_CHECK_BUTTON(check);
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
    g_object_set_data(G_OBJECT(generate_button), "skills_data", page_data);
    g_signal_connect(generate_button, "clicked", G_CALLBACK(on_generate_sheet_clicked), data);

    gtk_box_append(GTK_BOX(button_box), back_button);
    gtk_box_append(GTK_BOX(button_box), generate_button);
    gtk_box_append(GTK_BOX(page_vbox), button_box);
    
    // Imposta la modalità manuale di default
    on_manual_setup_clicked(NULL, page_data);

    AdwNavigationPage *page = adw_navigation_page_new(page_vbox, "Competenze e Riepilogo");
    return page;
}

// Applica automaticamente le competenze in base a classe e background
static void apply_auto_proficiencies(SkillsPageData *page_data) {
    // --- Background Proficiencies ---
    const char* bg_skills[2] = {NULL, NULL};
    if (strcmp(page_data->background, "Accolito") == 0) { bg_skills[0] = "Intuizione"; bg_skills[1] = "Religione"; }
    else if (strcmp(page_data->background, "Artigiano di Gilda") == 0) { bg_skills[0] = "Intuizione"; bg_skills[1] = "Persuasione"; }
    else if (strcmp(page_data->background, "Ciarlatano") == 0) { bg_skills[0] = "Inganno"; bg_skills[1] = "Rapidità di Mano"; }
    else if (strcmp(page_data->background, "Criminale") == 0) { bg_skills[0] = "Furtività"; bg_skills[1] = "Inganno"; }
    else if (strcmp(page_data->background, "Eremita") == 0) { bg_skills[0] = "Medicina"; bg_skills[1] = "Religione"; }
    else if (strcmp(page_data->background, "Eroe Popolare") == 0) { bg_skills[0] = "Addestrare Animali"; bg_skills[1] = "Sopravvivenza"; }
    else if (strcmp(page_data->background, "Forestiero") == 0) { bg_skills[0] = "Atletica"; bg_skills[1] = "Sopravvivenza"; }
    else if (strcmp(page_data->background, "Intrattenitore") == 0) { bg_skills[0] = "Acrobazia"; bg_skills[1] = "Intrattenere"; }
    else if (strcmp(page_data->background, "Monello") == 0) { bg_skills[0] = "Furtività"; bg_skills[1] = "Rapidità di Mano"; }
    else if (strcmp(page_data->background, "Nobile") == 0) { bg_skills[0] = "Persuasione"; bg_skills[1] = "Storia"; }
    else if (strcmp(page_data->background, "Sapiente") == 0) { bg_skills[0] = "Arcano"; bg_skills[1] = "Storia"; }
    else if (strcmp(page_data->background, "Soldato") == 0) { bg_skills[0] = "Atletica"; bg_skills[1] = "Intimidire"; }

    for (int i = 0; i < 2; i++) {
        if (bg_skills[i]) {
            int idx = get_skill_index(bg_skills[i]);
            if (idx != -1) {
                gtk_check_button_set_active(page_data->skill_checks[idx], TRUE);
            }
        }
    }

    // --- Class Proficiencies ---
    const char* class_sts[2] = {NULL, NULL};
    const char** skill_choices = NULL;
    int num_choices = 0;
    
    if (strcmp(page_data->class_name, "Barbaro") == 0) {
        class_sts[0] = "Forza"; class_sts[1] = "Costituzione"; num_choices = 2;
        static const char* choices[] = {"Addestrare Animali", "Atletica", "Intimidire", "Natura", "Percezione", "Sopravvivenza", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Bardo") == 0) {
        class_sts[0] = "Destrezza"; class_sts[1] = "Carisma"; num_choices = 3;
        // Qualsiasi abilità
    } else if (strcmp(page_data->class_name, "Chierico") == 0) {
        class_sts[0] = "Saggezza"; class_sts[1] = "Carisma"; num_choices = 2;
        static const char* choices[] = {"Intuizione", "Medicina", "Religione", "Storia", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Druido") == 0) {
        class_sts[0] = "Intelligenza"; class_sts[1] = "Saggezza"; num_choices = 2;
        static const char* choices[] = {"Addestrare Animali", "Arcano", "Intuizione", "Medicina", "Natura", "Percezione", "Religione", "Sopravvivenza", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Guerriero") == 0) {
        class_sts[0] = "Forza"; class_sts[1] = "Costituzione"; num_choices = 2;
        static const char* choices[] = {"Acrobazia", "Addestrare Animali", "Atletica", "Intimidire", "Intuizione", "Percezione", "Sopravvivenza", "Storia", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Ladro") == 0) {
        class_sts[0] = "Destrezza"; class_sts[1] = "Intelligenza"; num_choices = 4;
        static const char* choices[] = {"Acrobazia", "Atletica", "Furtività", "Indagare", "Inganno", "Intimidire", "Intrattenere", "Intuizione", "Percezione", "Persuasione", "Rapidità di Mano", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Mago") == 0) {
        class_sts[0] = "Intelligenza"; class_sts[1] = "Saggezza"; num_choices = 2;
        static const char* choices[] = {"Arcano", "Indagare", "Intuizione", "Medicina", "Religione", "Storia", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Monaco") == 0) {
        class_sts[0] = "Forza"; class_sts[1] = "Destrezza"; num_choices = 2;
        static const char* choices[] = {"Acrobazia", "Atletica", "Furtività", "Religione", "Storia", "Intuizione", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Paladino") == 0) {
        class_sts[0] = "Saggezza"; class_sts[1] = "Carisma"; num_choices = 2;
        static const char* choices[] = {"Atletica", "Intimidire", "Intuizione", "Medicina", "Persuasione", "Religione", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Ranger") == 0) {
        class_sts[0] = "Forza"; class_sts[1] = "Destrezza"; num_choices = 3;
        static const char* choices[] = {"Addestrare Animali", "Atletica", "Furtività", "Indagare", "Intuizione", "Natura", "Percezione", "Sopravvivenza", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Stregone") == 0) {
        class_sts[0] = "Costituzione"; class_sts[1] = "Carisma"; num_choices = 2;
        static const char* choices[] = {"Arcano", "Inganno", "Intimidire", "Intuizione", "Persuasione", "Religione", NULL}; skill_choices = choices;
    } else if (strcmp(page_data->class_name, "Warlock") == 0) {
        class_sts[0] = "Saggezza"; class_sts[1] = "Carisma"; num_choices = 2;
        static const char* choices[] = {"Arcano", "Indagare", "Inganno", "Intimidire", "Natura", "Religione", "Storia", NULL}; skill_choices = choices;
    }

    for (int i = 0; i < 2; i++) {
        if (class_sts[i]) {
            int idx = get_st_index(class_sts[i]);
            if (idx != -1) gtk_check_button_set_active(page_data->st_checks[idx], TRUE);
        }
    }
    
    page_data->num_skill_choices_total = num_choices;
    gboolean is_variant_human = (strcmp(page_data->race, "Umano") == 0 && strcmp(page_data->subrace, "Variante") == 0);

    // --- Umano Variante ---
    if (is_variant_human) {
        page_data->num_skill_choices_total++;
    }

    // --- Abilita le scelte ---
    page_data->selectable_skills = NULL;
    gboolean can_choose_any = (strcmp(page_data->class_name, "Bardo") == 0 || is_variant_human);

    if (can_choose_any) {
        for (int i = 0; i < NUM_SKILLS; i++) {
            if (!gtk_check_button_get_active(page_data->skill_checks[i])) {
                 page_data->selectable_skills = g_list_append(page_data->selectable_skills, GINT_TO_POINTER(i));
            }
        }
    } else if (skill_choices != NULL) {
        for (int i = 0; skill_choices[i] != NULL; i++) {
            int idx = get_skill_index(skill_choices[i]);
            if (idx != -1 && !gtk_check_button_get_active(page_data->skill_checks[idx])) {
                page_data->selectable_skills = g_list_append(page_data->selectable_skills, GINT_TO_POINTER(idx));
            }
        }
    } 

    for (GList *l = page_data->selectable_skills; l != NULL; l = l->next) {
        int skill_idx = GPOINTER_TO_INT(l->data);
        gtk_widget_set_visible(GTK_WIDGET(page_data->skill_rows[skill_idx]), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(page_data->skill_checks[skill_idx]), TRUE);
        g_signal_connect(page_data->skill_checks[skill_idx], "toggled", G_CALLBACK(on_skill_choice_toggled), page_data);
    }

    // Rendi visibili le abilità fisse del background
    for (int i = 0; i < 2; i++) {
        if (bg_skills[i]) {
            int idx = get_skill_index(bg_skills[i]);
            if(idx != -1) gtk_widget_set_visible(GTK_WIDGET(page_data->skill_rows[idx]), TRUE);
        }
    }


    // Aggiorna l'etichetta
    page_data->num_skill_choices_made = 0;
    on_skill_choice_toggled(NULL, page_data);
}

// Assegna automaticamente i punteggi del Vettore Standard in base alla classe
static void on_auto_assign_standard_clicked(GtkButton *button, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;
    on_reset_standard_clicked(NULL, stats_data);

    int scores[] = {15, 14, 13, 12, 10, 8};
    int priority[6] = {0, 1, 2, 3, 4, 5}; // Default: FOR, DES, COS, INT, SAG, CAR

    const char* class_name = stats_data->class_name;
    
    if (strcmp(class_name, "Guerriero") == 0 || strcmp(class_name, "Barbaro") == 0 || strcmp(class_name, "Paladino") == 0) {
        int p[] = {0, 2, 1, 4, 5, 3}; memcpy(priority, p, sizeof(p)); // FOR, COS, DES, SAG, CAR, INT
    } else if (strcmp(class_name, "Ladro") == 0 || strcmp(class_name, "Ranger") == 0 || strcmp(class_name, "Monaco") == 0) {
        int p[] = {1, 4, 2, 0, 3, 5}; memcpy(priority, p, sizeof(p)); // DES, SAG, CON, FOR, INT, CAR
    } else if (strcmp(class_name, "Bardo") == 0 || strcmp(class_name, "Stregone") == 0 || strcmp(class_name, "Warlock") == 0) {
        int p[] = {5, 1, 2, 4, 3, 0}; memcpy(priority, p, sizeof(p)); // CAR, DES, CON, SAG, INT, FOR
    } else if (strcmp(class_name, "Chierico") == 0 || strcmp(class_name, "Druido") == 0) {
        int p[] = {4, 2, 1, 0, 5, 3}; memcpy(priority, p, sizeof(p)); // SAG, CON, DES, FOR, CAR, INT
    } else if (strcmp(class_name, "Mago") == 0) {
        int p[] = {3, 2, 1, 4, 5, 0}; memcpy(priority, p, sizeof(p)); // INT, CON, DES, SAG, CAR, FOR
    }

    for (int i = 0; i < 6; i++) {
        char score_str[4];
        sprintf(score_str, "%d", scores[i]);
        gtk_editable_set_text(GTK_EDITABLE(stats_data->stat_entries_standard[priority[i]]), score_str);
    }
    
    // Nascondi le etichette dei punteggi
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(stats_data->standard_scores_flowbox));
    while (child != NULL) {
        gtk_widget_set_visible(child, FALSE);
        child = gtk_widget_get_next_sibling(child);
    }

    update_total_scores(stats_data);
    update_forward_button_sensitivity(stats_data);
}

// Helper per qsort
static int compare_ints_desc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

// Assegna automaticamente i punteggi tirati con i dadi
static void on_auto_assign_rolling_clicked(GtkButton *button, gpointer user_data) {
    StatsPageData *stats_data = (StatsPageData *)user_data;

    // 1. Recupera i punteggi dal flowbox
    int rolled_scores[6];
    int score_count = 0;
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(stats_data->rolls_flowbox));
    while (child != NULL && score_count < 6) {
        rolled_scores[score_count++] = atoi(gtk_label_get_text(GTK_LABEL(child)));
        child = gtk_widget_get_next_sibling(child);
    }
    if (score_count != 6) return; // Non dovrebbe succedere

    // 2. Ordina i punteggi
    qsort(rolled_scores, 6, sizeof(int), compare_ints_desc);

    // 3. Ottieni la priorità della classe
    int priority[6] = {0, 1, 2, 3, 4, 5};
    const char* class_name = stats_data->class_name;
    if (strcmp(class_name, "Guerriero") == 0 || strcmp(class_name, "Barbaro") == 0 || strcmp(class_name, "Paladino") == 0) {
        int p[] = {0, 2, 1, 4, 5, 3}; memcpy(priority, p, sizeof(p));
    } else if (strcmp(class_name, "Ladro") == 0 || strcmp(class_name, "Ranger") == 0 || strcmp(class_name, "Monaco") == 0) {
        int p[] = {1, 4, 2, 0, 3, 5}; memcpy(priority, p, sizeof(p));
    } else if (strcmp(class_name, "Bardo") == 0 || strcmp(class_name, "Stregone") == 0 || strcmp(class_name, "Warlock") == 0) {
        int p[] = {5, 1, 2, 4, 3, 0}; memcpy(priority, p, sizeof(p));
    } else if (strcmp(class_name, "Chierico") == 0 || strcmp(class_name, "Druido") == 0) {
        int p[] = {4, 2, 1, 0, 5, 3}; memcpy(priority, p, sizeof(p));
    } else if (strcmp(class_name, "Mago") == 0) {
        int p[] = {3, 2, 1, 4, 5, 0}; memcpy(priority, p, sizeof(p));
    }

    // 4. Assegna i punteggi
    for (int i = 0; i < 6; i++) {
        char score_str[4];
        sprintf(score_str, "%d", rolled_scores[i]);
        gtk_editable_set_text(GTK_EDITABLE(stats_data->stat_entries_rolling[priority[i]]), score_str);
    }
    
    // 5. Aggiorna la UI
    gtk_widget_set_visible(GTK_WIDGET(stats_data->rolls_flowbox), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(button), FALSE); // Nascondi il pulsante di assegnazione
    update_total_scores(stats_data);
    update_forward_button_sensitivity(stats_data);
}

// Helper per calcolare i bonus razziali senza UI
static void get_racial_bonuses_no_ui(const char* race, const char* subrace, int bonuses[6]) {
    memset(bonuses, 0, sizeof(int) * 6);
    if (strcmp(race, "Umano") == 0) {
        if (strcmp(subrace, "Normale") == 0) { for(int i=0; i<6; i++) bonuses[i] = 1; }
        else { // Variante, sceglie 2 casuali
            int choice1 = rand() % 6;
            int choice2;
            do { choice2 = rand() % 6; } while (choice1 == choice2);
            bonuses[choice1] += 1;
            bonuses[choice2] += 1;
        }
    }
    else if (strcmp(race, "Elfo") == 0) {
        bonuses[1] += 2;
        if (strcmp(subrace, "Alto Elfo") == 0) bonuses[3] += 1;
        else if (strcmp(subrace, "Elfo dei Boschi") == 0) bonuses[4] += 1;
        else if (strcmp(subrace, "Drow (Elfo Oscuro)") == 0) bonuses[5] += 1;
    }
    else if (strcmp(race, "Halfling") == 0) {
        bonuses[1] += 2;
        if (strcmp(subrace, "Piedelesto") == 0) bonuses[5] += 1;
        else if (strcmp(subrace, "Tozzo") == 0) bonuses[2] += 1;
    }
    else if (strcmp(race, "Nano") == 0) {
        bonuses[2] += 2;
        if (strcmp(subrace, "Nano delle Montagne") == 0) bonuses[0] += 2;
        else if (strcmp(subrace, "Nano delle Colline") == 0) bonuses[4] += 1;
    }
    else if (strcmp(race, "Dragonide") == 0) { bonuses[0] += 2; bonuses[5] += 1; }
    else if (strcmp(race, "Gnomo") == 0) {
        bonuses[3] += 2;
        if (strcmp(subrace, "Gnomo delle Rocce") == 0) bonuses[2] += 1;
        else if (strcmp(subrace, "Gnomo delle Foreste") == 0) bonuses[1] += 1;
    }
    else if (strcmp(race, "Mezzelfo") == 0) {
        bonuses[5] += 2;
        int choice1 = rand() % 5; // Scegli tra FOR, DES, COS, INT, SAG
        int choice2;
        do { choice2 = rand() % 5; } while (choice1 == choice2);
        bonuses[choice1] += 1;
        bonuses[choice2] += 1;
    }
    else if (strcmp(race, "Mezzorco") == 0) { bonuses[0] += 2; bonuses[2] += 1; }
    else if (strcmp(race, "Tiefling") == 0) { bonuses[5] += 2; bonuses[3] += 1; }
}

// Gestisce la scelta del metodo di generazione casuale dalla prima pagina
static void on_random_method_chosen(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)g_object_get_data(G_OBJECT(button), "app_data");
    int method = GPOINTER_TO_INT(user_data); // 0=dadi, 1=vettore

    // 1. Genera punteggi base
    int base_scores[6];
    if (method == 0) { // Tiro dei Dadi
        for (int i = 0; i < 6; i++) {
            int rolls[4], sum = 0, min = 7;
            for (int j = 0; j < 4; j++) {
                rolls[j] = (rand() % 6) + 1;
                sum += rolls[j];
                if (rolls[j] < min) min = rolls[j];
            }
            base_scores[i] = sum - min;
        }
    } else { // Vettore Standard
        int standard[] = {15, 14, 13, 12, 10, 8};
        memcpy(base_scores, standard, sizeof(standard));
    }

    qsort(base_scores, 6, sizeof(int), compare_ints_desc);

    // 2. Assegna in base alla priorità della classe
    GtkStringList *classi_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_classe));
    guint classe_pos = gtk_drop_down_get_selected(data->dropdown_classe);
    const char *classe_scelta = gtk_string_list_get_string(classi_model, classe_pos);

    int priority[6] = {0, 1, 2, 3, 4, 5};
    if (strcmp(classe_scelta, "Guerriero") == 0 || strcmp(classe_scelta, "Barbaro") == 0 || strcmp(classe_scelta, "Paladino") == 0) {
        int p[] = {0, 2, 1, 4, 5, 3}; memcpy(priority, p, sizeof(p));
    } else if (strcmp(classe_scelta, "Ladro") == 0 || strcmp(classe_scelta, "Ranger") == 0 || strcmp(classe_scelta, "Monaco") == 0) {
        int p[] = {1, 4, 2, 0, 3, 5}; memcpy(priority, p, sizeof(p));
    } else if (strcmp(classe_scelta, "Bardo") == 0 || strcmp(classe_scelta, "Stregone") == 0 || strcmp(classe_scelta, "Warlock") == 0) {
        int p[] = {5, 1, 2, 4, 3, 0}; memcpy(priority, p, sizeof(p));
    } else if (strcmp(classe_scelta, "Chierico") == 0 || strcmp(classe_scelta, "Druido") == 0) {
        int p[] = {4, 2, 1, 0, 5, 3}; memcpy(priority, p, sizeof(p));
    } else if (strcmp(classe_scelta, "Mago") == 0) {
        int p[] = {3, 2, 1, 4, 5, 0}; memcpy(priority, p, sizeof(p));
    }

    int assigned_scores[6];
    for (int i = 0; i < 6; i++) {
        assigned_scores[priority[i]] = base_scores[i];
    }

    // 3. Aggiungi bonus razziali
    GtkStringList *razze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_razza));
    guint razza_pos = gtk_drop_down_get_selected(data->dropdown_razza);
    const char *razza_scelta = gtk_string_list_get_string(razze_model, razza_pos);
    const char *subrace_scelta = "";
    if (gtk_widget_is_visible(data->row_sottorazza)) {
        GtkStringList *sottorazze_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_sottorazza));
        guint sottorazza_pos = gtk_drop_down_get_selected(data->dropdown_sottorazza);
        subrace_scelta = gtk_string_list_get_string(sottorazze_model, sottorazza_pos);
    }

    int racial_bonuses[6];
    get_racial_bonuses_no_ui(razza_scelta, subrace_scelta, racial_bonuses);

    for (int i = 0; i < 6; i++) {
        data->generated_stats[i] = assigned_scores[i] + racial_bonuses[i];
    }
    data->stats_are_generated = TRUE;

    // 4. Vai alla pagina delle competenze
    GtkStringList *backgrounds_model = GTK_STRING_LIST(gtk_drop_down_get_model(data->dropdown_background));
    guint background_pos = gtk_drop_down_get_selected(data->dropdown_background);
    const char *background_scelto = gtk_string_list_get_string(backgrounds_model, background_pos);

    AdwNavigationPage *skills_page = create_skills_page(data, razza_scelta, subrace_scelta, classe_scelta, background_scelto);
    adw_navigation_view_push(data->nav_view, skills_page);

    // Ripristina la UI della prima pagina per quando si torna indietro
    gtk_widget_set_visible(data->random_method_box, FALSE);
    gtk_widget_set_visible(data->avanti_button, TRUE);
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
