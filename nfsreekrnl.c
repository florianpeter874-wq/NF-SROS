int cursor_x = 0;
int cursor_y = 0;
// Array size corresponds to the maximum expected scancode index (Set 1 uses < 0x80)
#define SCANCODE_MAX 128
#define MAX_FILES 10
int notepad();
extern int jatek();
extern int init_sor();
extern void print_int(int value);

// Egy fájl felépítése a memóriában
typedef struct {
    char nev[32];      // A fájl neve (pl. "readme.txt")
    int meret;         // A fájl mérete bájtban
    char tartalom[128]; // A fájl valós tartalma
} Fajl;

// Létrehozzuk a virtuális fájlrendszerünket 4 alapértelmezett fájllal
Fajl fajlrendszer[MAX_FILES] = {
    {"readme.txt", 42, "Udvözlünk a Netfloor Searee OS-ben! Ez egy falu a memoriaban."},
    {"verzio.txt", 18, "Searee OS v0.1 alpha"},
    {"titok.txt",  34, "A kernel sikeresen betoltott! 42."},
    {"auto.exe", 456, "enton version"},
};

int fajlok_szama = 4;
void cls();
void print_char(char c);
void print(char *szoveg);
unsigned char get_scancode();
char get_ascii_char(unsigned char scancode, int shift_pressed);
int parancs_e(const char *beirt, const char *parancs);
void input_commands();

void szoveg_masol(char *cel, const char *forras) {
    while (*forras) {
        *cel = *forras;
        cel++;
        forras++;
    }
    *cel = '\0'; // Lezárjuk a stringet
}

void uj_fajl(const char *nev) {
    if (fajlok_szama >= MAX_FILES) {
        print("Hiba: A fajlrendszer megtelt!\n");
        return;
    }

    // Átmásoljuk a nevet az új üres helyre
    szoveg_masol(fajlrendszer[fajlok_szama].nev, nev);
    fajlrendszer[fajlok_szama].meret = 0; // Új fájl, még üres
    szoveg_masol(fajlrendszer[fajlok_szama].tartalom, ""); // Üres tartalom

    fajlok_szama++; // Növeljük a fájlok számát
    print("Fajl sikeresen letrehozva!\n");
}

void listaz_fajlrendszer() {
    print("--- Searee OS Virtualis Fajlrendszer ---\n");
    print("Nev\t\t\tMeret (Bajt)\n");
    print("----------------------------------------\n");

    for (int i = 0; i < fajlok_szama; i++) {
        print(fajlrendszer[i].nev);
        
        // Mivel nincs kész printf-ünk ami számokat formáz, 
        // egyelőre csak tabulátorral eltoljuk a méret vizuális jelzéséhez
        print("\t\t\t[Fajl]\n"); 
    }
    print("----------------------------------------\n");
}

// Saját string összehasonlító függvény (mivel nincs string.h-d)
int parancs_e(const char *beirt, const char *parancs) {
    while (*beirt && (*beirt == *parancs)) {
        beirt++;
        parancs++;
    }
    return *(const unsigned char*)beirt - *(const unsigned char*)parancs;
}

void input_commands() {
    char puffer[64]; // Itt tároljuk a beírt parancsot (max 63 karakter)
    int i = 0;       // Index a pufferhez

    while (1) {
        unsigned char scancode = get_scancode();
        int shift_pressed = 0; 
        char c = get_ascii_char(scancode, shift_pressed);
        
        if (c != 0) {
            // Ha ENTER-t nyomott a felhasználó (ASCII kódja általában '\n' vagy 13)
            if (c == '\n' || c == '\r') {
                puffer[i] = '\0'; // Lezárjuk a stringet a memóriában
                print_char('\n'); // Új sort kezdünk a képernyőn
                int new_file_parancs = 1;
                for(int j = 0; j < 8; j++) {
                    if(puffer[j] != "newfile "[j]) {
                    new_file_parancs = 0;
                    break;
                    }
                }
                // Parancsok ellenőrzése
                if (parancs_e(puffer, "help") == 0) {
                    print("Elerheto Parancsok:\nHelp - Segitseg\nExit - Kilepes\nlsfilesystem - Fajl listazas\nnewfile <nev> - uj fajl\njatek - jatszas egy jatekot\n");
                } 
                else if (parancs_e(puffer, "exit") == 0) {
                    print("A rendszer leall.\n");
                    while(1) {
                        __asm__ volatile("hlt"); // CPU leállítása
                    }
                } 
                
                else if (parancs_e(puffer, "cls") == 0) {
                    cls();
                } else if (parancs_e(puffer, "lsfilesystem") == 0) {
                    listaz_fajlrendszer();
                } else if (new_file_parancs == 1) {
                    // A puffer 9. karakterétől kezdődik a fájlnév (a "newfile " után)
                    char *fajl_neve = &puffer[8]; 
                    if (*fajl_neve != '\0') {
                        uj_fajl(fajl_neve);
                    } else {
                        print("Hiba: Nem adtal meg fajlnevet! Hasznalat: newfile <nev>\n");
                    }
                } else if (parancs_e(puffer, "notepad") == 0) {
                    notepad();
                } else if (parancs_e(puffer, "jatek") == 0) {
                    jatek();
                } else if (parancs_e(puffer, "scriptssc") == 0) {
                    init_sor();
                } else if (parancs_e(puffer, "sysinfo") == 0) {
                    print("A Rendszer Informacioja: Netfloor Searee 0.1 (NF-SROS) Text Edition\n");
                    print("Files in Virtual Filesystem: ");
                    print_int(fajlok_szama);
                    print_char('\n');
                    print("Developer: Netfloor's CEO\nA legfiatalabb OS fejleszto cim birtokosa.\n");
                } else if (i > 0) {
                    print("Ismeretlen parancs! Ird be: Help\n");
                }

                // Alaphelyzetbe állítjuk a puffert a következő parancshoz
                i = 0;
                print("> "); // Parancssor jelző
            } 
            // Ha nem Enter, és van még hely a pufferben, elmentjük és kiírjuk
            else if (i < 63) {
                puffer[i] = c;
                i++;
                print_char(c); // Megjelenítjük a képernyőn, amit gépel
            }
        }
    }
}

// 1. Map for standard keystrokes (No Shift)
const char scancode_to_ascii_normal[SCANCODE_MAX] = {
    0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,
  '\\', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,   '*',   0, ' '
};

// 2. Map for shifted keystrokes (Shift held down)
const char scancode_to_ascii_shift[SCANCODE_MAX] = {
    0,   27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',   0,
  '|',  'Y', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0,   '*',   0, ' '
};

/**
 * Converts a raw hardware scancode to an ASCII character.
 * 
 * @param scancode      The raw byte from the keyboard controller (e.g., Port 0x60).
 * @param shift_pressed Current state of the shift key (1 for pressed, 0 for released).
 * @return              The mapped ASCII character, or 0 if it is a release/non-printable key.
 */

unsigned char inb(unsigned short port) {
    unsigned char adat;
    __asm__ volatile("inb %1, %0" : "=a"(adat) : "Nd"(port));
    return adat;
}

void print_char(char c) {
    char *video_memoria = (char *)0xB8000;
    int i = (cursor_y *160) + (cursor_x * 2);
    if (c == '\n') {
        cursor_y++;
        cursor_x = 0;
        i = (cursor_y * 160) + (cursor_x * 2);
    } else {
        video_memoria[i] = c;
        video_memoria[i + 1] = 0x02;
        cursor_x++;
    }
}

void print(char *szoveg) {
    char *video_memoria = (char *)0xB8000;
    int i = (cursor_y *160) + (cursor_x * 2);
    int s = 0;
    while (szoveg[s] != '\0') {
        if (szoveg[s] == '\n') {
            cursor_y++;
            cursor_x = 0;
            i = (cursor_y * 160) + (cursor_x * 2);
        } else {
            video_memoria[i] = szoveg[s];
            video_memoria[i + 1] = 0x02;
            i+= 2;
            cursor_x++;
        }
        s++;
    }
}

void wait(unsigned int dwSeconds) {
    for (unsigned int i = 0; i < dwSeconds; i++) {
        for (volatile unsigned int j = 0; j < 10000000; j++) {
        }
    }
}

unsigned char get_scancode() {
    unsigned char kod = 0;
    
    // 1. lépés: Végtelen ciklusban figyeljük a billentyűzetet
    while (1) {
        // Beolvassuk az aktuális bájtot a 0x60-as hardver portról
        kod = inb(0x60);
        
        // Ha kaptunk kódot, ÉS az kisebb mint 0x80, az egy gomb LENYOMÁSA.
        // (A 0x80 feletti kódok azt jelentik, hogy felengedtek egy gombot)
        if (kod != 0 && kod < 0x80) {
            break; // Megvan a gombnyomás, kilépünk a végtelen ciklusból!
        }
    }
    
    // 2. lépés: Megvárjuk, amíg a felhasználó FELENGEDI a gombot.
    // Ha ezt nem tennénk meg, a gép olyan gyorsan pörögne, hogy egyetlen
    // gombnyomást akár 50-szer is beolvasna egymás után.
    // Addig várunk üresen, amíg a portról beérkező kód kisebb mint 0x80.
    while (inb(0x60) < 0x80) {
        // Nem csinálunk semmit, csak várunk a felengedésre
    }
    
    // 3. lépés: Visszaadjuk a tiszta, lenyomott gomb kódját
    return kod;
}

char get_ascii_char(unsigned char scancode, int shift_pressed) {
    // Filter out key release (break) codes (Set 1 sets the highest bit 0x80)
    if (scancode & 0x80) {
        return 0; 
    }
    
    // Memory safety boundary guard
    if (scancode >= SCANCODE_MAX) {
        return 0;
    }

    // Return mapping based on modifier state
    if (shift_pressed) {
        return scancode_to_ascii_shift[scancode];
    } else {
        return scancode_to_ascii_normal[scancode];
    }
}

void cls() {
    char *video_memoria = (char *)0xB8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memoria[i] = ' ';
        video_memoria[i + 1] = 0x02; // Set color attribute
    }
    cursor_x = 0;
    cursor_y = 0;
}

void bejelentkezes() {
    cls();
    char username[32];
    int sdx = 0;
    char jelszo[32];
    print("Felhasznalonev?\n");
    while(1) {
        char character = get_ascii_char(get_scancode(), 0);
        if (character == '\n') {
            break;
        } else if (character == '\b') {
            if (cursor_x > 0) {
                cursor_x--;
                print_char(' '); // Töröljük a karaktert a képernyőről
                cursor_x--;
            }
        } else if (parancs_e(username, "peti") == 0) {
            print("Bebizonyult, hogy te vagy az aki készített.\n");
            print("Ird be a jelszavad\n");
            int sdx = 0;
            while(1) {
                char password_char = get_ascii_char(get_scancode(), 0);
                if (password_char == '\n') {
                    print_char('\0');
                    break;
                } else if (password_char == '\b') {
                    if (cursor_x > 0) {
                        cursor_x--;
                        print_char(' '); // Töröljük a karaktert a képernyőről
                        cursor_x--;
                    }
                } else if (parancs_e(jelszo, "searee") == 0) {
                    print_char('*');
                    print("Elfogadva!\n");
                    print_char('\0');
                    break; // Csillaggal jelöljük a jelszó karaktereit
                } else {
                    print_char('*');
                    jelszo[sdx] = password_char;
                    sdx++;
                }
            }
            print_char('\0');
        } else {
            print_char(character);
            username[sdx] = character;
            sdx++;
        }
    }
    print_char('\n');
    print("Jelszo?\n");
}

void boot_kepernyo() {
    print("Udvozlünk a Searee OS-ben\n");
    print("Netfloor Searee 0.1\n\n");
    print("[");
    for (volatile unsigned int j = 0; j < 20; j++) {
        print("-");
        wait(1);
    }
    print("]\n");
    print("Rendszer Betoltve!\n");
}

int kernel_main() {
    boot_kepernyo();
    bejelentkezes();
    cls();
    input_commands();
    print("Hello World\n\n");
    while (1) {}
}