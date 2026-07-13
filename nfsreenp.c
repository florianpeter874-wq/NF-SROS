void cls();
int get_ascii_char(unsigned char scancode, int shift_pressed);
unsigned char get_scancode();
void print(char *szoveg);
void print_char(char c);
int dxs = 0;
int notepad() {
    cls();
    while (1) {
        char character = get_ascii_char(get_scancode(), 0);
        print_char(character);
        char szoveg[500];
        szoveg[dxs] = character;
        dxs++;
        if (character == '\n') {
            print_char('\n');
        } else if (character == 27) {
            print_char('\n');
            break;
        }
        if (dxs == 500) {
            print("Figyelem! A szovegdoboz megtelt!");
            break;
            print(szoveg);
            int dxs = 0;
        }
    }
}