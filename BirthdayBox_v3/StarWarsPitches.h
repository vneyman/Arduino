/*************************************************
 * NOTES FOR Star Wars Imperial March
 *************************************************/
#define NOTE_c  261
#define NOTE_d  294
#define e  329
#define f  349
#define g  391
#define gS  415
#define a  440
#define aS  455
#define NOTE_b 466
#define cH  523
#define cSH  554
#define dH  587
#define dSH  622
#define eH  659
#define fH  698
#define fSH  740
#define gH  784
#define gSH  830
#define aH  880

//first section, first part
int starWarsNotesS1_1[] = {a, a, a, f, cH, a, f, cH, a };
int starWarsDurationsS1_1[] = {500, 500,500, 350,150,500, 350,150,650};

//first section, second part
int starWarsNotesS1_2[] = {eH,eH,eH, fH, cH, gS, f, cH, a };
int starWarsDurationsS1_2[] = {500, 500,500, 350,150,500,350,150,650};

//second section, first part
int starWarsNotesS2_1[] = {aH, a, a, aH, gSH, gH, fSH, fH, fSH };
int starWarsDurationsS2_1[] = {500, 300, 150, 500, 325, 175, 125, 125, 250};

//second section, second part
int starWarsNotesS2_2[] = {aS, dSH, dH, cSH, cH, NOTE_b, cH};
int starWarsDurationsS2_2[] = {250, 500, 325, 175, 125, 125, 250};
