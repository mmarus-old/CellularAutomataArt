#!/usr/bin/env python2.7
#
# ******************************************************************************
# ****** BiCAS - Bidlo Cellular Automata Simulator pro potreby kurzu BIN *******
# ******************************************************************************
# K funkci vyzaduje nainstalovane prostredi Python s knihovnami NumPy a PyGame.
# Pokud toto nemate nainstalovano, je mozne spustit BiCAS vzdalene po pripojeni
# na server merlin:
#
#                     ssh -l xlogin00 -X merlin.fit.vutbr.cz
#
# --------------------------------- Licence ------------------------------------
# BiCAS je poskytnut v souladu s pravidly "The BSD 3-Clause License", jejiz
# zneni je uvedeno v souboru LICENSE distribuovaneho spolu s programem.
# ------------------------------------------------------------------------------
#
#     (c) 2015-2016, Michal Bidlo, FIT VUT v Brne, <bidlom AT fit vutbr cz>
#
# ******************************************************************************

import os, sys, glob, pygame
from random import randint
import numpy as np
pygame.init()

# implicitni velikost CA (_SIZE x _SIZE bunek) - modifikovatelne
_SIZE = 60
# velikost okna celularniho prostoru (_WIN x _WIN pixelu) - modif.
_WIN = 600

# nejake barevne konstanty => v tomto pripade CA s max. 13 stavy
white = pygame.Color(0xFFFFFF00)
black = pygame.Color(0x00000000)
red = pygame.Color(0xFF000000)
green = pygame.Color(0x00FF0000)
blue = pygame.Color(0x0000FF00)
cyan = pygame.Color(0x00FFFF00)
magenta = pygame.Color(0xFF00FF00)
yellow = pygame.Color(0xFFFF0000)
gray = pygame.Color(0x64646400)
pink = pygame.Color(0xFF087F00)
brown = pygame.Color(0x825A2C00)
orange = pygame.Color(0xFA680000)
violet = pygame.Color(0xAA00FF00)

color = [ black, cyan, yellow, magenta, pink, brown, orange, blue, violet, red, green, white, gray ]

class CA:
    def __init__(self, rows=20, cols=20, states=2, nsize=5):
        self.shelp = 1
        self.age = 0
        self.cell = np.empty((rows+2, cols+2), object)  # object bude str
        self.temp = np.empty((rows+2, cols+2), object)
        self.istt = np.empty((rows+2, cols+2), object)
        for row in range(rows + 2):
            for col in range(cols + 2):
                self.cell[row][col] = "00"
                self.temp[row][col] = "00"
                self.istt[row][col] = "00"
        self.ltf_dict = {}
        self.ltf_list = [] # seznam souboru s LTF
        self.ltf_index = 0 # index aktualne nactene LTF
        self.ltf_limit = 0 # kolik .tab souboru nacteno
        self.rows = rows
        self.cols = cols
        self.states = states
        self.nsize = nsize # velikost bunecneho okoli
        # nastaveni rozmeru bunek vzhledem k velikosti okna
        self.cell_w = pygame.display.Info().current_w / self.cols
        self.cell_h = pygame.display.Info().current_h / self.rows
        # souradnice pro vypis napovedy
        self.hy = 0

    def set_cell(self, row, col, state):
        temp = int(state)
        self.cell[row][col] = "%02d" % temp

    def get_cell(self, row, col):
        return int(self.cell[row][col])

    def set_clicked_cell(self, (pos_x, pos_y)):
        row = pos_y / self.cell_h + 1
        col = pos_x / self.cell_w + 1
        temp = (self.get_cell(row, col) + 1) % self.states
        self.set_cell(row, col, temp)

    def zero_init(self):
        self.age = 0
        for row in range(1, self.rows+1):
            for col in range(1, self.cols+1):
                self.set_cell(row, col, 0)

    def istt_init(self):
        self.age = 0
        for row in range(1, self.rows+1):
            for col in range(1, self.cols+1):
                self.set_cell(row, col, self.istt[row][col])

    def draw_text(self, win, label):
        global _WIN

        font = pygame.font.SysFont("monospace", 24)
        font.set_bold(1)
        text = font.render(label, 1, (255, 255, 0))
        if self.hy == 0:
            win.blit(text, (20, 10))
            self.hy = self.hy + 40
        else:
            win.blit(text, (20, self.hy))
            self.hy = self.hy + 20

    def show_help(self, win):
        self.hy = 0
        self.draw_text(win, "Interaktivni rizeni aplikace BiCAS")
        self.draw_text(win, "SPACE: spust/pozastav vyvoj CA")
        self.draw_text(win, "t: proved jeden vyvojovy krok CA")
        self.draw_text(win, "i: navrat do pocatecniho stavu")
        self.draw_text(win, "c: zapis stavu CA do souboru .png")
        self.draw_text(win, "klik mysi: inkrement stavu bunky")
        self.draw_text(win, "ESC: ukonceni aplikace BiCAS")
        self.draw_text(win, "h: zobrazeni teto napovedy")
        self.draw_text(win, "Detaily viz soubor BiCAS-man.txt.")
        pygame.display.update()

    def draw(self, win):
        for row in range(1, self.rows+1):
            for col in range(1, self.cols+1):
                pygame.draw.rect(win, color[self.get_cell(row, col)],
                                ((col-1)*self.cell_w, (row-1)*self.cell_h,
                                self.cell_w, self.cell_h), 0)
                #pygame.draw.rect(win, blue,
                #                ((col-1)*self.cell_w, (row-1)*self.cell_h,
                #                self.cell_w, self.cell_h), 1)
        pygame.display.update()
        # po startu aplikace zobraz napovedu
        if self.shelp == 1:
            self.show_help(win)

    def develop(self, win):
        # probehne krok vyvoje vsech bunek podle dane LTF
        for row in range(1, self.rows+1):
            for col in range(1, self.cols+1):
                self.LTF_next(row, col)
        # update stavu celularniho pole
        for row in range(1, self.rows+1):
            for col in range(1, self.cols+1):
                self.set_cell(row, col, self.temp[row][col])
        self.draw(win)
        self.age = self.age + 1

    def LTF_next(self, row, col):
        if self.nsize == 5:
            north=self.cell[row-1][col] if row>1 else self.cell[self.rows][col]
            west =self.cell[row][col-1] if col>1 else self.cell[row][self.cols]
            cent =self.cell[row][col]
            east =self.cell[row][col+1] if col<self.cols else self.cell[row][1]
            south=self.cell[row+1][col] if row<self.rows else self.cell[1][col]
            key = int(north) + int(west) + int(cent) + int(east) + int(south)
            # print key
            self.temp[row][col] = self.ltf_dict.get(key, self.cell[row][col])

        elif self.nsize == 9:
            row_cm = row - 1 if row > 1 else self.rows
            row_cp = row + 1 if row < self.rows else 1
            col_cm = col - 1 if col > 1 else self.cols
            col_cp = col + 1 if col < self.cols else 1
            key =  int(self.cell[row_cm][col_cm])
            key += int(self.cell[row_cm][col])
            key += int(self.cell[row_cm][col_cp])
            key += int(self.cell[row][col_cm])
            key += int(self.cell[row][col])
            key += int(self.cell[row][col_cp])
            key += int(self.cell[row_cp][col_cm])
            key += int(self.cell[row_cp][col])
            key += int(self.cell[row_cp][col_cp])
            # print key
            self.temp[row][col] = self.ltf_dict.get(key, self.cell[row][col])
        else:
            print "Nepodporovane okoli:", self.nsize
            exit(1)

    def read_state(self, sfile):
        istate = []
        for line in sfile.readlines():
            istate.append(line.strip().split(' '))
        sfile.close()
        roff = (self.rows - len(istate)) / 2
        coff = (self.cols - len(istate[0])) / 2
        for r in range(0, len(istate)):
            for s in range(0, len(istate[0])):
                self.istt[r+roff+1][s+coff+1] = istate[r][s]
        self.istt_init()

    def state_file_default(self):
        try:
            # pokud existuje soubor default.cas, je implicitne nacten
            # a system v nem ocekava specifikaci pocatecniho stavu CA
            caf = open("default.cas", "r")
        except IOError:
            print "Nezadan soubor stavu, CA inicializovan na 0"
            return
        with caf:
            self.read_state(caf)

    def state_file_custom(self, file_name):
        # je mozno specifikovat i vlastni soubor s pocatecnim stavem,
        # jehoz nazev lze zadat jako argument pri spusteni aplikace
        try:
            caf = open(file_name, "r")
        except IOError:
            print "Nelze otevrit soubor stavu:", file_name
            self.state_file_default()
            return
        with caf:
            self.read_state(caf)

    def read_ca(self, file_name):
        row = 0
        try:
            caf = open(file_name, "r")
        except IOError:
            print "Nelze otevrit soubor:", file_name
            exit(1)
        with caf:
            for line in caf:
                line = line.strip().split(' ')
                if row == 0:  # nactena hlavicka specifikujici pocet_stavu
                    if len(line) != 1:  # kdyz hlavicka neodpovida
                        print "Hlavicka musi obsahovat jen hodnotu poctu stavu!"
                        exit(1)
                    self.states = int(line[0])
                    if self.states > len(color):
                        print "Prilis mnoho stavu CA: %d (maximum je %d)" % \
                            (self.states, len(color))
                        exit(1)
                elif row == 1:
                    if len(line) != 1:  # kdyz hlavicka neodpovida
                        print "Hlavicka musi obsahovat velkost okolia!"
                        exit(1)
                    self.nsize = int(line[0])
                    if self.nsize != 5 and self.nsize != 9:
                        print "Nepodporovane okoli:", self.nsize
                        exit(1)

                elif line[0] != "#": # zakomentovana pravidla se ignoruji
                    # zde se nacitaji pravidla LTF
                    right = "%02d" % int(line.pop()) # posl. na radku = next st.
                    left = int(line[0])
                    self.ltf_dict[left] = right
                row = row + 1
            caf.close()
        print "Pocet stavu:", self.states
        print "Bunecne okoli:", self.nsize
        print "Nacteno pravidel:", row-1 # -1 = bez hlavicky
        pygame.display.set_caption(file_name)

    def read_tab_files(self):
        self.ltf_list = glob.glob("*.tab")
        if not self.ltf_list:
            return # nenalezen zadny .tab, vratime se a bude prazdny CA

        self.ltf_list.sort()
        self.ltf_limit = len(self.ltf_list)
        if "default.tab" in self.ltf_list:
            # byl-li nalezen default.tab (ten se vzdy nacte prednostne),
            # nastavime index v seznamu ltf_list primo na jeho pozici
            self.ltf_index = self.ltf_list.index("default.tab")
        # dle nastavene pozice nacteme prislusny soubor s prechodovou funkci
        self.read_ca(self.ltf_list[self.ltf_index])

    def ltf_change_next(self):
        self.ltf_dict.clear()
        if self.ltf_index < self.ltf_limit-1:
            self.ltf_index = self.ltf_index + 1
        else:
            sys.exit(0) # pro indikaci projiti vsech funkci
            self.ltf_index = 0
        self.read_ca(self.ltf_list[self.ltf_index])

    def ltf_change_prev(self):
        self.ltf_dict.clear()
        if self.ltf_index > 0:
            self.ltf_index = self.ltf_index - 1
        else:
            self.ltf_index = self.ltf_limit-1
        self.read_ca(self.ltf_list[self.ltf_index])

def main_loop(ca, win):
    countOfSteps = 0
    maxSteps = 60
    devel = 0
    capture = 0
    while True: # obsluha GUI a rizeni vyvoje CA
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit(0)
            elif event.type == pygame.KEYDOWN:
                ca.shelp = 0
                keyb = event.key
                if keyb == pygame.K_SPACE: # pozastavit/spustit vyvoj CA
                    devel = 1 - devel      # priznak, zda bezi vyvoj CA
                elif keyb == pygame.K_c:   # capture - zapis png se stavem CA
                    if devel == 0: # pro zachyceni stavu musi byt CA pozastaven
                        cap_img = "%03d" % capture
                        capture = capture + 1
                        pygame.image.save(win, cap_img + ".png")
# odkomentuj, pokud chces po zapisu stavu automaticky provest jeden dalsi krok
#                        ca.develop(win)
                elif keyb == pygame.K_t:   # proved 1 krok, pokud je CA pozast.
                    if devel == 0:
                        ca.develop(win)
                    print "step %d" % ca.age
                elif keyb == pygame.K_i:   # stop, inicializuj CA z istt.
                    ca.istt_init()
                    countOfSteps = 0
                    ca.draw(win)
                    devel = 0
                    capture = 0
                elif keyb == pygame.K_s:   # stop, vynuluj CA
                    ca.zero_init()
                    ca.draw(win)
                    devel = 0
                    capture = 0
                elif keyb == pygame.K_h:   # pozastav CA, ukaz napovedu
                    if devel == 1:
                        devel = 0
                    ca.shelp = 1
                    ca.draw(win)
                # pokud je v adresari nalezeno vice souboru .tab obsahujicich
                # prechodove funkce CA, lze mezi nimi pomoci sipek prepinat
                elif keyb == pygame.K_DOWN or keyb == pygame.K_RIGHT:
                    if ca.ltf_list:
                        ca.istt_init()
                        ca.draw(win)
                        devel = 0
                        capture = 0
                        ca.ltf_change_next()
                elif keyb == pygame.K_UP or keyb == pygame.K_LEFT:
                    if ca.ltf_list:
                        ca.istt_init()
                        ca.draw(win)
                        devel = 0
                        capture = 0
                        ca.ltf_change_prev()
                elif keyb == pygame.K_ESCAPE: # ukonceni aplikace
                    sys.exit(0)
            # klik mysi na bunku inkrementuje jeji stav - pro hrani si...
            elif event.type == pygame.MOUSEBUTTONDOWN:
                ca.shelp = 0
                ca.set_clicked_cell(pygame.mouse.get_pos())
                ca.draw(win)
        if devel == 1 and countOfSteps < maxSteps:
            ca.develop(win)
            print "Step #:", countOfSteps
            countOfSteps += 1


def run_parser(): # analyza argumentu zadanych pri spusteni aplikace
    global _SIZE

    ca = None
    size = _SIZE    # implicitni velikost CA
    state = False   # zda zadan pocatecni stav - zatim ne
    ltf = False     # zda zadana prechodova funkce - zatim ne

    i = 1 # index do pole sys.argv
    runl = len(sys.argv)
    if i < runl:
        if sys.argv[i].isdigit():
            new_size = int(sys.argv[i])
            if new_size > 0:
                print "Zadana velikost CA:", new_size
                size = new_size
            else:
                print "Velikost CA bude:", size, "x", size
            i = i + 1

    ca = CA(size, size)    # vytvorime CA daneho rozmeru

    if i < runl:
        if sys.argv[i].endswith("tab"): # soubor s pravidly LPF
            print "Nacitam soubor s LPF:", sys.argv[i]
            ca.read_ca(sys.argv[i])
            ltf = True
        elif sys.argv[i].endswith("cas"):    # soubor s pocatecnim stavem
            print "Zadan soubor poc. stavu:", sys.argv[i]
            ca.state_file_custom(sys.argv[i])
            state = True
        else:
            print "POZOR, neznamy argument:", sys.argv[i]
        i = i + 1

    if i < runl and not state and sys.argv[i].endswith("cas"):
        print "Zadan soubor poc. stavu:", sys.argv[i]
        ca.state_file_custom(sys.argv[i])
        state = True
        i = i + 1

    if i < runl:
        print "Ignoruji dalsi argumenty:",
        while i < runl:
            print sys.argv[i],
            i = i + 1
        print

    # pokud nebyl zadan pocateni stav, pokusime se najit default.cas
    if not state:
        ca.state_file_default()
    # pokud nebyl zadan soubor s prechodovou funkci, budou se hledat
    # vsechny soubory .tab, ty se nactou do seznamu a z prvniho bude
    # prechodova funkce nastavena
    if not ltf:
        ca.read_tab_files()

    return ca

def usage():
    print "Usage:"
    print "-------------------------------------------------------"
    print "PyCA [integer_ca_size] [ltf_file.tab] [istate_file.cas]"
    print "! Argumenty jsou volitelne, poradi a pripony dulezite !"
    print "-------------------------------------------------------"

def main():
    global _WIN

    usage()
    win = pygame.display.set_mode((_WIN, _WIN))
    ca = run_parser()
    ca.draw(win)
    pygame.display.flip()
    main_loop(ca, win)

if __name__ == "__main__":
    main()
