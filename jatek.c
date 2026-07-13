int coordinate_y = 0;
int coordinate_x = 0;
int coordinate_z = 0;
int dis = 0;
char *szovegem;
int coin = 0;

extern void print(char *szoveg);
extern void cls();
extern int get_ascii_char(unsigned char scancode, int shift_pressed);
extern unsigned char get_scancode();
extern int parancs_e(const char *beirt, const char *parancs);
extern void print_char(char c);
void print_int(int value);

int szovegmegjelenites() {
    print(szovegem);
    print_char('\n');
    return 0;
}

int print_coin() {
    print("Ennyi penzed lett: ");
    print_int(coin);
    print_char('\n');
}

void print_int(int value) {
    char buffer[16];
    int i = 0;
    int is_negative = 0;

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    if (value == 0) {
        buffer[i++] = '0';
    } else {
        while (value > 0) {
            buffer[i++] = (char)('0' + (value % 10));
            value /= 10;
        }
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    while (i > 0) {
        print_char(buffer[--i]);
    }
}

int jatek() {
    while(1) {
        char character = get_ascii_char(get_scancode(), 0);
        if (character == 'w') {
            coordinate_x++;
            print("X koordinatad: ");
            print_int(coordinate_x);
            print_char('\n');
            print("És a Z koordinatad: ");
            print_int(coordinate_z);
            print_char('\n');
            print("És az Y koordinatad: ");
            print_int(coordinate_y);
            print_char('\n');
        } else if (character == 'a') {
            coordinate_z++;
            print("X koordinatad: ");
            print_int(coordinate_x);
            print_char('\n');
            print("És a Z koordinatad: ");
            print_int(coordinate_z);
            print_char('\n');
            print("És az Y koordinatad: ");
            print_int(coordinate_y);
            print_char('\n');
        } else if (character == 's') {
            coordinate_x--;
            print("X koordinatad: ");
            print_int(coordinate_x);
            print_char('\n');
            print("És a Z koordinatad: ");
            print_int(coordinate_z);
            print_char('\n');
            print("És az Y koordinatad: ");
            print_int(coordinate_y);
            print_char('\n');
        } else if (character == 'd') {
            coordinate_z--;
            print("X koordinatad: ");
            print_int(coordinate_x);
            print_char('\n');
            print("És a Z koordinatad: ");
            print_int(coordinate_z);
            print_char('\n');
            print("És az Y koordinatad: ");
            print_int(coordinate_y);
            print_char('\n');
        } else if (character == 'u') {
            coordinate_y++;
            print("X koordinatad: ");
            print_int(coordinate_x);
            print_char('\n');
            print("És a Z koordinatad: ");
            print_int(coordinate_z);
            print_char('\n');
            print("És az Y koordinatad: ");
            print_int(coordinate_y);
            print_char('\n');
        } else if (character == 'o') {
            coordinate_y--;
            print("X koordinatad: ");
            print_int(coordinate_x);
            print_char('\n');
            print("És a Z koordinatad: ");
            print_int(coordinate_z);
            print_char('\n');
            print("És az Y koordinatad: ");
            print_int(coordinate_y);
            print_char('\n');
        } else if (character == 'e') {
            szovegmegjelenites();
            print_coin();
            print("Megnyomta a kilepo gombot\n");
            return 0;
        } else {
            szovegem[dis] = character;
        }
        if (coordinate_x == 12 && coordinate_y == 2 && coordinate_z == 4) {
            print("Ugyes vagy! Talaltal egy kincsesladat! Nyertel 4 penzt\n");
            coin += 4;
        } else if (coordinate_x == 6 && coordinate_y == 0 && coordinate_z == 0) {
            print("Falba utkoztel. Nem mozoghatsz tovabb.\n");
            print("De azert megtekintheted az eredmenyeid.\n");
            szovegmegjelenites();
            print_coin();
        }
        szovegem[dis] = character;
        dis++;
    }
}