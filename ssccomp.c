extern unsigned char get_scancode();
extern char get_ascii_char(unsigned char scancode, int shift_pressed);
extern void print(char *szoveg);
extern int parancs_e(const char *beirt, const char *parancs);
extern void print_char(char c);
extern void print_int(int value);
char character;
int doldxs = 0;
char sor[300];

void init_sor() {
    int doldxs = 0;
    while(1) {
        character = get_ascii_char(get_scancode(), 0);
        if (character == '\n') {
            print_char('\n');
            break;
        }
        print_char(character);
        sor[doldxs] = character;
        doldxs++;
    }
    if (parancs_e(sor, "print ") == 0) {
        int nindex = 6;
        print_char('\n');
        while(1) {
            if (sor[0] == 'p' && sor[1] == 'r' && sor[2] == 'i' && sor[3] == 'n' && sor[4] == 't' && sor[5] == ' ') {
                if (sor[nindex] == '\0') {
                    print_char('\n');
                    break;
                }
                print_char(sor[nindex]);
                nindex++;
            }
        }
    } else if (parancs_e(sor, "calc ") == 0) {
        int nindex = 5;
        int szam;
        int calced;
        while (1) {
            if (sor[0] == 'c' && sor[1] == 'a' && sor[2] == 'l' && sor[3] == 'c' && sor[4] == ' ') {
                if (sor[nindex] == '\0') {
                    break;
                }
                if (sor[nindex] < 10) {
                    int szam = sor[nindex];
                    nindex + 2;
                }
                if (sor[nindex] == 'p' && sor[nindex + 1] == 'l' && sor[nindex + 2] == 'u' && sor[nindex + 3] == 's') {
                    nindex + 5;
                    if (sor[nindex] < 10) {
                        int calcing = sor[nindex];
                        int calced = szam + calcing;
                    }
                }
                print_int(calced);
            }
        }
    }
}