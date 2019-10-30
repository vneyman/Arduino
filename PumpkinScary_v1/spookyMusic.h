/*************************************************
 * Public Constants
 *************************************************/

int spooky_music_num_tones = 291; //change this to reflect the number of notes in your song
int cs6 = 1109; // frequencies of the notes used in the song
int fs5 = 740;
int d6 = 1175;
int c6 = 1047;
int f5 = 698;
int b5 = 988;
int e5 = 659;
int bf5 = 932;
int ef5 = 622;
int b4 = 494;
int g5 = 784;
int wait = 200;  // length of the notes

int spookyMusicTones[] = { cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5, //notes in the song
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5, 
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                 c6,  f5,  f5,  c6,  f5,  f5,  c6,  f5, cs6,  f5,
                 c6,  f5,  f5,  c6,  f5,  f5,  c6,  f5, cs6,  f5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                 c6,  f5,  f5,  c6,  f5,  f5,  c6,  f5, cs6,  f5,
                 c6,  f5,  f5,  c6,  f5,  f5,  c6,  f5, cs6,  f5,
                 b5,  e5,  e5,  b5,  e5,  e5,  b5,  e5,  c6,  e5,
                 b5,  e5,  e5,  b5,  e5,  e5,  b5,  e5,  c6,  e5,
                bf5, ef5, ef5, bf5, ef5, ef5, bf5, ef5,  b5, ef5,
                bf5, ef5, ef5, bf5, ef5, ef5, bf5, ef5,  b5, ef5,
                 b5,  e5,  e5,  b5,  e5,  e5,  b5,  e5,  c6,  e5,
                 b5,  e5,  e5,  b5,  e5,  e5,  b5,  e5,  c6,  e5,
                bf5, ef5, ef5, bf5, ef5, ef5, bf5, ef5,  b5, ef5,
                bf5, ef5, ef5, bf5, ef5, ef5, bf5, ef5,  b5, ef5,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4};
        
unsigned long current_song_time = 0; unsigned long prev_song_time = 0; int note_count = 0; int spooky_music_on_off = 0; //variables for running the song
