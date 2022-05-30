import pygame
import sys
import os
import random
pygame.init()

''' CONSTANTE CE TIN DE FERESTRA, FRAME RATE SAU SCRIS '''

WIDTH = 800
HEIGHT = 600
SIZE = (WIDTH, HEIGHT)
FPS = 60
FONT = pygame.font.Font('freesansbold.ttf', 15)

''' CULORI '''

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

''' SETAREA NR DE EXTRATERESTRII '''

NR_OF_ALIENS_PER_ROW = 8
NR_OF_ALIEN_ROWS = 5

''' SETAREA NR DE ASTEROIZI SI PRODUCEREA VARIATIEI DINTRE INALTIMILE LOR '''

NR_OF_ASTEROIDS = 6
asteroid_variation = []
for i in range(NR_OF_ASTEROIDS):
    asteroid_variation.append(random.randint(-20, 60))

''' VARIABILE CARE VERIFICA STARILE LASERELOR SI ALE INAMICILOR SAU PLAYER-ULUI '''

kill_player_laser = False

kill_alien_laser = False
alien_laser_is_shot = False

player_killed = False
aliens_killed = False

''' SETAREA PATH-URILOR UNIVERSALE PENTRU IMAGINI '''

dir_path = os.path.dirname(__file__)
highscore_path = os.path.join(dir_path, 'highscore.txt')
assets_path = os.path.join(dir_path, 'assets')
icon_path = os.path.join(assets_path, 'icon.png')
player_path = os.path.join(assets_path, 'ship.png')
asteroid_path = os.path.join(assets_path, 'asteroid.png')
alien1_1_path = os.path.join(assets_path, 'alien1_1.png')
alien2_1_path = os.path.join(assets_path, 'alien2_1.png')
alien1_2_path = os.path.join(assets_path, 'alien1_2.png')
alien2_2_path = os.path.join(assets_path, 'alien2_2.png')
player_laser_path = os.path.join(assets_path, 'player_laser.png')
alien_laser_path = os.path.join(assets_path, 'alien_laser.png')
highscore_img_path = os.path.join(assets_path, 'highscore.png')
score_path = os.path.join(assets_path, 'score.png')
wallpaper_path = os.path.join(assets_path, 'wallpaper3.png')
heart_path = os.path.join(assets_path, 'heart.png')


class Player:
    VEL = 5
    global nr_of_lasers_taken 
    nr_of_lasers_taken = 0
    taken_laser_limit = 4

    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.img = pygame.image.load(player_path)

    def render(self, background):
        background.blit(self.img, (self.x, self.y))

    def move_right(self):
        self.x += self.VEL
        if (self.x + self.img.get_width()) > WIDTH:
            self.x -= self.VEL

    def move_left(self):
        self.x -= self.VEL
        if self.x < 0:
            self.x += self.VEL

    def damage_check(self, x_laser, y_laser):   
        global nr_of_lasers_taken
        global kill_alien_laser
        global player_killed
        check_x_laser = round(x_laser) in range(self.x, self.x + self.img.get_width())
        check_y_laser = y_laser in range(self.y, self.y + self.img.get_height())
        if check_y_laser and check_x_laser:
            kill_alien_laser = True
            if nr_of_lasers_taken >= self.taken_laser_limit:
                nr_of_lasers_taken += 1
                player_killed = True
            else:
                nr_of_lasers_taken += 1


class Health(Player):

    def __init__(self):
        self.max_health = Player.taken_laser_limit + 1
        self.img = pygame.image.load(heart_path)
        self.health_bar_heart_dist = 5

    def render(self, background, x, y):
        global nr_of_lasers_taken
        for i in range(self.max_health - nr_of_lasers_taken):
            background.blit(self.img, (x + i * (self.health_bar_heart_dist + self.img.get_width()), y))


class Laser:
    VEL = 5

    def __init__(self, x, y, image_path):
        self.x = x
        self.y = y
        self.img = pygame.image.load(image_path)

    def move_up(self):
        self.y -= self.VEL

    def move_down(self):
        self.y += self.VEL

    def player_render(self, background):
        self.move_up()
        background.blit(self.img, (self.x, self.y))

    def alien_render(self, background):
        self.move_down()
        background.blit(self.img, (self.x, self.y))

    def laser_collision(self, other):
        if round(self.x) == round(other.x) and round(self.y) == round(other.y):
            return True
        else:
            return False


class Asteroid:
    ast_belt_dist = 80
    is_destroyed = [False for x in range(NR_OF_ASTEROIDS)]
    nr_of_lasers_taken = [0 for x in range(NR_OF_ASTEROIDS)]
    taken_laser_limit = 15

    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.img = pygame.image.load(asteroid_path)

    def player_laser_check(self, x_laser, y_laser, new_x, new_y, n):
        global kill_player_laser
        check_x_laser = x_laser in range(new_x, new_x + self.img.get_width())
        check_y_laser = y_laser in range(new_y + self.img.get_height()//2, new_y + self.img.get_height())
        if check_y_laser and check_x_laser:
            kill_player_laser = True
            if self.nr_of_lasers_taken[n] >= self.taken_laser_limit:
                self.is_destroyed[n] = True
            else:
                self.nr_of_lasers_taken[n] += 1

    def alien_laser_check(self, x_laser, y_laser, new_x, new_y, n):
        global kill_alien_laser
        check_x_laser = round(x_laser) in range(new_x, new_x + self.img.get_width())
        check_y_laser = y_laser in range(new_y, new_y + self.img.get_height() + 1)
        if check_y_laser and check_x_laser:
            kill_alien_laser = True
            if self.nr_of_lasers_taken[n] >= self.taken_laser_limit:
                self.is_destroyed[n] = True
            else:
                self.nr_of_lasers_taken[n] += 1

    def render(self, background, x_lasers, y_lasers, n=0, m=0):
        player_laser_x, alien_laser_x = x_lasers
        player_laser_y, alien_laser_y = y_lasers
        new_x = self.x + n * (self.ast_belt_dist + self.img.get_width())
        new_y = self.y - m
        self.player_laser_check(player_laser_x, player_laser_y, new_x, new_y, n)
        self.alien_laser_check(alien_laser_x, alien_laser_y, new_x, new_y, n)
        if not self.is_destroyed[n]:    
            background.blit(self.img, (new_x, new_y))    

    def belt_render(self, background, x_lasers, y_lasers):
        for i in range(NR_OF_ASTEROIDS):
            if not self.is_destroyed[i]:
                self.render(background, x_lasers, y_lasers, i, asteroid_variation[i])


class Alien:
    VEL = 0.2
    continue_direction = True
    alien_row_dist = 10
    alien_line_dist = 5
    is_destroyed = [[False for x in range(NR_OF_ALIENS_PER_ROW)] for y in range(NR_OF_ALIEN_ROWS)]
    nr_of_lasers_taken = [[False for x in range(NR_OF_ALIENS_PER_ROW)] for y in range(NR_OF_ALIEN_ROWS)]

    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.img1 = pygame.image.load(alien1_1_path)
        self.img2 = pygame.image.load(alien2_1_path)
        self.img1_2 = pygame.image.load(alien1_2_path)
        self.img2_2 = pygame.image.load(alien2_2_path)

    def get_lowest_alien(self):
        low_aliens = []
        search = True
        for i in range(NR_OF_ALIENS_PER_ROW):
            for j in range(NR_OF_ALIEN_ROWS-1, -1, -1):
                if not self.is_destroyed[j][i]:
                    low_aliens.append((j, i))
                    break
        chose = random.randint(0, len(low_aliens)-1)
        return low_aliens[chose]

    def player_laser_check(self, image, x_laser, y_laser, new_x, new_y, row_nr, n):
        global kill_player_laser
        global killed_alien

        if (x_laser >= new_x) and (x_laser <= (new_x + image.get_width())):
            check_x_laser = True
        else:
            check_x_laser = False

        if (y_laser >= (new_y + image.get_height()//2)) and (y_laser <= (new_y + image.get_height())):
            check_y_laser = True
        else:
            check_y_laser = False

        if check_y_laser and check_x_laser:
            kill_player_laser = True
            self.is_destroyed[row_nr][n] = True

    def render(self, image, background, row_nr, n, x_laser, y_laser):
        global killed_alien
        global alien_laser_is_shot
        global alien_laser
        global kill_alien_laser
        new_x = self.x + n * (self.alien_row_dist + image.get_width())
        new_y = self.y + row_nr * (self.alien_line_dist + image.get_height()) 
        self.killed_alien = False
        if not self.is_destroyed[row_nr][n]:
            self.player_laser_check(image, x_laser, y_laser, new_x, new_y, row_nr, n)     
        if not self.is_destroyed[row_nr][n]:  
            background.blit(image, (new_x, new_y))
            alien_y, alien_x = self.get_lowest_alien()
            if not alien_laser_is_shot and row_nr == alien_y and n == alien_x:
                kill_alien_laser = False
                alien_laser = Laser(new_x + image.get_width()//2, new_y + image.get_height(), alien_laser_path)
                alien_laser_is_shot = True

    def move_right(self, img_width):
        if (self.x + NR_OF_ALIENS_PER_ROW * (self.alien_row_dist + img_width)) > WIDTH:
            self.continue_direction = False
            return
        self.x += self.VEL

    def move_left(self):
        if self.x < 0:
            self.continue_direction = True
            return
        self.x -= self.VEL
  
    def move_alien(self, img_width):
        if self.continue_direction:
            self.move_right(img_width)
        else:
            self.move_left()

    def alien_row_render(self, background, which_img, row_nr, x_laser, y_laser):
        global h
        if which_img:
            self.move_alien(self.img1.get_width())
            for i in range(NR_OF_ALIENS_PER_ROW):
                if i%2 == 0:
                    self.render(self.img1, background, row_nr, i, x_laser, y_laser)
                else:
                    self.render(self.img2, background, row_nr, i, x_laser, y_laser)
        else:
            self.move_alien(self.img1_2.get_width())
            for i in range(NR_OF_ALIENS_PER_ROW):
                if i%2 == 0:
                    self.render(self.img1_2, background, row_nr, i, x_laser, y_laser)
                else:
                    self.render(self.img2_2, background, row_nr, i, x_laser, y_laser)

    def alien_army_render(self, background, which_img, x_laser, y_laser):
        for i in range(NR_OF_ALIEN_ROWS):
            self.alien_row_render(background, which_img, i, x_laser, y_laser)

    def count_dead_aliens(self):
        global aliens_killed
        dead_aliens = 0
        for i in range(NR_OF_ALIEN_ROWS):
            for j in range(NR_OF_ALIENS_PER_ROW):
                if self.is_destroyed[i][j]:
                    dead_aliens += 1
        if dead_aliens == NR_OF_ALIEN_ROWS*NR_OF_ALIENS_PER_ROW:
            aliens_killed = True
        return dead_aliens


def main():
    global kill_player_laser
    global kill_alien_laser
    global alien_laser_is_shot
    global player_killed
    global FONT

    ''' SETAREA UNEI SUPRAFETE SI COSTUMIZAREA FERESTREI DE JOC '''

    pygame.display.set_caption("*Space Invaders*")
    screen = pygame.display.set_mode(SIZE)

    background = pygame.Surface(SIZE)
    background = background.convert()
    background.fill(BLACK)
    screen.blit(background, (0, 0))

    wallpaper = pygame.image.load(wallpaper_path)

    pygame.display.flip()
    
    clock = pygame.time.Clock()

    icon = pygame.image.load(icon_path)
    pygame.display.set_icon(icon)

    ''' SETAREA JUCATORULUI SI A INAMICILOR '''

    player = Player(385, 550)
    asteroid = Asteroid(30, 450)
    alien = Alien(10, 50)

    animation_stage = True
    animation_clock = 0

    player_laser_is_shot = False
    player_laser_x = WIDTH + 1
    player_laser_y = 0

    alien_laser_x = WIDTH + 1
    alien_laser_y = HEIGHT

    ''' SETAREA SCORULUI SI A HIGHSCOR-ULUI '''

    highscore_img = pygame.image.load(highscore_img_path)
    score_img = pygame.image.load(score_path)

    score = 0

    ''' SETAREA AFISARII VIETILOR RAMASE '''

    health = Health()

    while True:

        ''' STERGEREA BACKGROUND-ULUI SI LIMITAREA FRAME RATE-ULUI '''

        clock.tick(FPS)
        screen.blit(background , (0, 0))
        background.fill(BLACK)

        ''' TESTAREA APASARII X-ULUI PENTRU A INCHIDE FEREASTRA JOCULUI '''

        for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()

        ''' DESENAREA IMAGINII DE FUNDAL, A HIGHSCORE-ULUI, SCORULUI SI A VIETILOR RAMASE '''

        background.blit(wallpaper, (0, 0))

        background.blit(highscore_img, (5, 5))
        background.blit(score_img, (5, 22))
       
        score = FONT.render(str(score), True, WHITE)
        background.blit(score, (score_img.get_width() + 10, 22))

        health.render(background, 665, 5)

        highscore_value = open(highscore_path, 'r')
        saved_highscore = highscore_value.read()
        highscore = FONT.render(str(saved_highscore), True, WHITE)
        background.blit(highscore, (highscore_img.get_width() + 10, 5))
        highscore_value.close()
        
        ''' TESTAREA TASTELOR PENTRU CONTROLAREA JUCATORULUI '''

        if not player_killed:
            keys = pygame.key.get_pressed()
            if keys[ord('a')] or keys[pygame.K_LEFT]:
                player.move_left()
            if keys[ord('d')] or keys[pygame.K_RIGHT]:
                player.move_right()
            if keys[pygame.K_SPACE]:
                # testez daca laserul jucatorului este pe ecran sau nu si daca nu il creez
                if not player_laser_is_shot:
                    player_laser = Laser(player.x + player.img.get_width()/2 - 5, player.y, player_laser_path)
                    player_laser_is_shot = True
                    kill_player_laser = False

        ''' ACTUALIZEZ IMAGINEA LASERULUI JUCATORULUI SI/SAU EXTRATERESTRULUI DACA EXISTA PE ECRAN '''

        if player_laser_is_shot:
            if alien_laser_is_shot:
                if player_laser.laser_collision(alien_laser):
                    kill_alien_laser = True
                    kill_player_laser = True
            player_laser_x = player_laser.x
            player_laser_y = player_laser.y
            if player_laser.y <= 0 or kill_player_laser:
                player_laser_is_shot = False
                player_laser_x = WIDTH + 1
                player_laser_y = 0
            else: player_laser.player_render(background)

               
        if alien_laser_is_shot:
            alien_laser_x = alien_laser.x
            alien_laser_y = alien_laser.y
            if alien_laser.y >= HEIGHT or kill_alien_laser:
                alien_laser_is_shot = False
                alien_laser_x = WIDTH + 1
                alien_laser_y = HEIGHT
            else: alien_laser.alien_render(background)

        ''' EXTRAG COORDONATELE LASERELOR SI ACTUALIZEZ ASTEROIZII '''

        x_lasers = (player_laser_x, alien_laser_x)
        y_lasers = (player_laser_y, alien_laser_y)
        asteroid.belt_render(background, x_lasers, y_lasers)

        ''' VERIFIC DACA JUCATORUL A PRIMIT DAMAGE SI II ACTUALIZEZ POZITIA '''
        
        if not player_killed:
            player.damage_check(alien_laser_x, alien_laser_y)
            player.render(background)

        ''' SELECTEZ CE IMAGINE SA FOLOSEASCA EXTRATERESTRII PT ANIMATIE SI II ACTUALIZEZ '''

        if animation_clock == 30:
            if animation_stage:
                animation_stage = False
            else:
                animation_stage = True
            animation_clock = 0
        else:
            animation_clock += 1

        alien.alien_army_render(background, animation_stage, player_laser_x, player_laser_y)

        ''' ACTUALIZEZ SCORUL SI VERIFIC DACA L-A INTRECUT PE CEL SALVAT IN FISIERUL TXT '''

        score = alien.count_dead_aliens()*10
        if score > int(saved_highscore):
            highscore_value = open(highscore_path, 'w')
            highscore_value.write(str(score))
            highscore_value.close()

        ''' VERIFIC DACA JUCATORUL A MURIT SAU NU MAI SUNT EXTRATERESTRII SI DACA DA INCHID JOCUL '''

        if player_killed or aliens_killed:
            pygame.time.wait(600)
            pygame.quit()
            sys.exit()

        ''' PUN TOATE IMAGINILE ACTUALIZATE PE ECRAN '''

        pygame.display.flip()       


if __name__ == '__main__':
    main()
