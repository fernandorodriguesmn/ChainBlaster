#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_ENEMIES 15
#define TIRO_INATIVO 0
#define TIRO_ATIVO 1
#define RAIO_TIRO 100
#define TEMPO_TIRO 2.0
#define BORDA_TIRO 4
#define SCORE_PENALTY 0.3
#define ATIVO 1
#define INATIVO 0

const float FPS = 60;  

const int SCREEN_W = 960; //largura da tela
const int SCREEN_H = 540; //altura da tela
const int HERO_W = 30; 
const int HERO_H = 40;
//const int ENEMY_W = 20;
//const int ENEMY_H = 25;

/*
gcc chain.c -o chain_completo.exe \
$(pkg-config --cflags --libs \
allegro-5 \
allegro_font-5 \
allegro_ttf-5 \
allegro_primitives-5 \
allegro_image-5) \
-lm
*/



ALLEGRO_COLOR BKG_COLOR;
ALLEGRO_FONT *FONT_32;

typedef struct Tiro {
	int x, y;
	float raio;
	int modo;
	float timer;
	ALLEGRO_COLOR cor;
} Tiro;

typedef struct Ship {
	int x, y;
	float vel;	
	ALLEGRO_COLOR cor;
	Tiro tiro;
} Ship;

typedef struct Hero {

	Ship ship;
	int dir_x;
	int dir_y;
	float score;
} Hero;


typedef struct Enemy {

	Ship ship;
	float raio;
	int dir_x;
	int dir_y;
	int active;
	int w, h;

} Enemy;


void initTiro(Ship *s) {

	//printf("\nInit tiro!");

	s->tiro.x = s->x;
	s->tiro.y = s->y;
	s->tiro.raio = 1;
	s->tiro.cor = s->cor;
	s->tiro.timer = TEMPO_TIRO;
	s->tiro.modo = TIRO_INATIVO;

}

void initGlobals() {
	BKG_COLOR = al_map_rgb(10, 10, 10);
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
	FONT_32 = al_load_font("arial.ttf", 32, 1);   
}



void initHero(Hero *s) {

	s->score = 0;
	s->ship.cor = al_map_rgb(100 + rand()%156, 100 + rand()%156, 100 + rand()%156);
	s->ship.x = SCREEN_W/2;
	s->ship.y = SCREEN_H - HERO_H - 10;
	s->ship.vel = 3;
	s->dir_x = 0;
	s->dir_y = 0;
	initTiro(&s->ship);

}

void initEnemy(Enemy *e) {
	srand(time(NULL));
	
	for(int i = 0; i < NUM_ENEMIES; i++){
		e[i].ship.cor = al_map_rgb(11, 238, 70);
		e[i].ship.vel = rand()%4 + 2;
		e[i].dir_x = 0;
		e[i].dir_y = 0;
		e[i].active = ATIVO;
		e[i].w = rand()%21 + 20;
		e[i].h = e[i].w;
		initTiro(&e[i].ship);
	}

	for(int i = 0, j = 0; i < NUM_ENEMIES/2; i++, j += 2){
		e[i].ship.x = 40 + rand()%SCREEN_W/2;
		e[i].ship.y = 40 + j;
	}
	
	for(int i = NUM_ENEMIES/2, j = 0; i < NUM_ENEMIES; i++, j += 2){
		e[i].ship.x = rand()%SCREEN_W/2 + SCREEN_W/2 - 40 - j;
		e[i].ship.y = 40 + j;
	}

}

void drawScenario(Hero s) {

	char score_txt[5];

	al_clear_to_color(BKG_COLOR);


}

void drawHero(Hero s) {


	al_draw_filled_triangle(s.ship.x, s.ship.y, s.ship.x - HERO_W/2, s.ship.y + HERO_H, s.ship.x + HERO_W/2, s.ship.y + HERO_H, s.ship.cor);

	char score_txt[5];
	sprintf(score_txt, "%d", (int)s.score);
	//imprime o texto armazenado em my_text na posicao x=10,y=10 e com a cor rgb(128,200,30)
	al_draw_text(FONT_32, s.ship.cor, 100, 20, 0, score_txt);	

	al_draw_circle(s.ship.tiro.x, s.ship.tiro.y, s.ship.tiro.raio, s.ship.tiro.cor, BORDA_TIRO);


}

void drawEnemy(Enemy e[]) {

	for(int i = 0; i<NUM_ENEMIES; i++){
		al_draw_filled_triangle(e[i].ship.x, e[i].ship.y, e[i].ship.x - e[i].w/2, e[i].ship.y - e[i].h, e[i].ship.x + e[i].w/2, e[i].ship.y - e[i].h, e[i].ship.cor);
		al_draw_circle(e[i].ship.tiro.x, e[i].ship.tiro.y, e[i].ship.tiro.raio, e[i].ship.tiro.cor, BORDA_TIRO);
	}

}





void updateHero(Hero *s) {


	s->ship.x += s->dir_x * s->ship.vel;

	s->ship.y += s->dir_y * s->ship.vel;

	//limitar o Hero dentro dos limites da tela
	if(s->ship.x >= SCREEN_W - HERO_W){
		s->ship.x = SCREEN_W - HERO_W;
	}

	if(s->ship.x <= 0 + HERO_W){
		s->ship.x = 0 + HERO_W;
	}

	if(s->ship.y >= SCREEN_H - HERO_H/2){
		s->ship.y = SCREEN_H - HERO_H/2 - 10;
	}

	if(s->ship.y <= 0 + HERO_H/2){
		s->ship.y = 0 + HERO_H/2;
	}

	if(s->ship.tiro.modo != TIRO_ATIVO) {
		s->ship.tiro.x = s->ship.x;
		s->ship.tiro.y = s->ship.y;
	}
	else {
		if(s->ship.tiro.timer > 0)
			s->ship.tiro.timer -= 1.0/FPS;
		else
			initTiro(&s->ship);
	}

}

void updateEnemy(Enemy e[], Hero *s) {
	
	for(int i = 0; i < NUM_ENEMIES; i++){
		if(e[i].active == ATIVO){
			if(e[i].ship.x - s->ship.x < 0)
				e[i].ship.x += e[i].ship.vel;
			if(e[i].ship.x - s->ship.x > 0)
				e[i].ship.x -= e[i].ship.vel;

			if(e[i].ship.y - s->ship.y < 0)
				e[i].ship.y += e[i].ship.vel;
			if(e[i].ship.y - s->ship.y > 0)
				e[i].ship.y -= e[i].ship.vel;

			if(e[i].ship.tiro.modo != TIRO_ATIVO){
				e[i].ship.tiro.x = e[i].ship.x;
				e[i].ship.tiro.y = e[i].ship.y;
			}
		}
			else{
				if(e[i].ship.tiro.timer > 0)
					e[i].ship.tiro.timer -= 1.0/FPS;
				else{
					e[i].ship.tiro.modo = TIRO_INATIVO;
					e[i].ship.tiro.raio = 0;
				}
			}

			//e[i].ship.x += e[i].dir_x * e[i].ship.vel;
			//e[i].ship.y += e[i].dir_y * e[i].ship.vel;	
	}

}


void DeadHero(Enemy e[], Hero s, int *p){
	
	for(int i = 0; i < NUM_ENEMIES; i++){
		if(e[i].active == ATIVO){
			if((e[i].ship.x == s.ship.x && e[i].ship.y == s.ship.y)
			|| (e[i].ship.x - HERO_W/2 >= s.ship.x - e[i].w/2 && e[i].ship.x + HERO_W/2 <= s.ship.x + e[i].w/2 && e[i].ship.y - s.ship.y == HERO_H + e[i].h)
			|| (e[i].ship.y >= s.ship.y && e[i].ship.y <= s.ship.y + HERO_H && e[i].ship.x + e[i].w/2 >= s.ship.x - HERO_W/2 && e[i].ship.x + e[i].w/2 <= s.ship.x + HERO_W/2 )
			|| (e[i].ship.y >= s.ship.y && e[i].ship.y <= s.ship.y + HERO_H && e[i].ship.x - e[i].w/2 >= s.ship.x - HERO_W/2 && e[i].ship.x - e[i].w/2 <= s.ship.x + HERO_W/2))
				*p = 0;
		}	
	}

}


void DeadEnmy(Hero s, Enemy e[]){
	if(s.ship.tiro.modo == TIRO_ATIVO){
		for(int i = 0; i < NUM_ENEMIES; i++){
			for(int j = 0; j < NUM_ENEMIES; j++){
				if((e[i].ship.x - s.ship.tiro.x)*(e[i].ship.x - s.ship.tiro.x) + (e[i].ship.y - s.ship.tiro.y)*(e[i].ship.y - s.ship.tiro.y) <= (s.ship.tiro.raio*s.ship.tiro.raio) ||
				(e[j].ship.tiro.modo == TIRO_ATIVO && (e[i].ship.x - e[j].ship.tiro.x)*(e[i].ship.x - e[j].ship.tiro.x) + (e[i].ship.y - e[j].ship.tiro.y)*(e[i].ship.y - e[j].ship.tiro.y) <= (e[j].ship.tiro.raio*e[j].ship.tiro.raio))){
					e[i].active = INATIVO;
					e[i].ship.cor = BKG_COLOR;
					e[i].ship.x = 0;
					e[i].ship.y = 0;
					e[i].ship.tiro.modo = TIRO_ATIVO;
					e[i].ship.tiro.raio = e[i].w;
				}
			}
		}
	}
}

void Nextround(Enemy e[]){
	int cont = 0;
	for(int i = 0; i < NUM_ENEMIES; i++){
		if(e[i].active == INATIVO)
			cont++;
	}
	if(cont == NUM_ENEMIES){
		initEnemy(e);
	}

}

 
int main(int argc, char **argv){

	//srand(time(NULL));

	int i;
	
	ALLEGRO_DISPLAY *display = NULL; //typedef struct, cria uma tela, criado como ponteiro para passar para outras funções
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro; stderr -> tela
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   


	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());


	//----------------------jogo-------------------

	//inicializa globais
	initGlobals();

	//Cria o heroi
	Hero Hero;
	initHero(&Hero);


	//cria os inimigos:
	Enemy enemy[NUM_ENEMIES];
	initEnemy(enemy);
	
	//inicia o temporizador
	al_start_timer(timer);
	
	int playing = 1;
	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {


			drawScenario(Hero);
			updateHero(&Hero);
			updateEnemy(enemy, &Hero);

			drawHero(Hero);
			drawEnemy(enemy);

			//playing = 0, quando Hero colide com Enemy
			DeadHero(enemy, Hero, &playing);
			DeadEnmy(Hero, enemy);
			Nextround(enemy);

			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();


			//pausa o jogo por 3 segundos se o jogador morrer
			if(!playing)
				al_rest(3);

		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi
			//printf("\ncodigo tecla: %d", ev.keyboard.keycode);
			switch(ev.keyboard.keycode) {
			//se a tecla for o W
				case ALLEGRO_KEY_W:
						Hero.dir_y--;
				break;
					

				case ALLEGRO_KEY_S:
						Hero.dir_y++;
				break;

				case ALLEGRO_KEY_A:
						Hero.dir_x--;
				break;

				case ALLEGRO_KEY_D:
						Hero.dir_x++;
				break;	
					
				case ALLEGRO_KEY_SPACE:
					if(Hero.ship.tiro.modo == TIRO_INATIVO) {
						Hero.ship.tiro.modo = TIRO_ATIVO;
						Hero.ship.tiro.raio = RAIO_TIRO;
					}
				break;
			}
		}			

		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//imprime qual tecla foi
			//printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode) {
			//se a tecla for o W
				case ALLEGRO_KEY_W:
					Hero.dir_y++;
				break;

				case ALLEGRO_KEY_S:
					Hero.dir_y--;
				break;

				case ALLEGRO_KEY_A:
					Hero.dir_x++;
				break;

				case ALLEGRO_KEY_D:
					Hero.dir_x--;
				break;	
				/*
				case ALLEGRO_KEY_SPACE:
					if(Hero.tiro.modo == TIRO_HOLDING)
						Hero.tiro.modo = TIRO_ATIVO;
				break;		
				*/		
			}			

		}

	} //fim do while
     
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	

		char my_text[100];
		al_clear_to_color(al_map_rgb(0,0,0));
	 	sprintf(my_text, "Pontuação: %d", (int)Hero.score);
		al_draw_text(FONT_32, al_map_rgb(220, 30, 0), SCREEN_W/3, SCREEN_H/2 + 50, 0, my_text);


		al_flip_display();
		al_rest(3);	
 
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}