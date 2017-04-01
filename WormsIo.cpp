#include "WormsIo.h"
#include <cstdlib>
#include <cstdio>


WormsIo :: WormsIo (uint32_t _nImages, char * imgFiles[])
{
	valid = false;	//estos valores se modificaran solo si la inicializacion es correcta
	display = NULL;
	images = NULL;
	nImages = 0;
	evQ = NULL;

	uint32_t i = 0;	//para indexar los punteros a bitmap y a string

	if (_nImages > 0) {
		if ((images = (ALLEGRO_BITMAP **) calloc(_nImages, sizeof(ALLEGRO_BITMAP *))) != NULL) {
			if(al_init()) {
				if( (display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT)) != NULL ) {
					if(al_install_keyboard()) {
						if( (evQ = al_create_event_queue()) != NULL ) {
							if (al_init_image_addon()) {
								if ( (timer = al_create_timer(1.0/(double)FPS)) != NULL) {
									valid = true;	//ya se instalo todo, falta cargar los bitmaps

									for (i = 0; i<_nImages && imgFiles[i] != NULL && valid == true; i++) {
										if ( (images[i]=al_load_bitmap(imgFiles[i])) == NULL) {
											valid = false;
										}
									}

									if (valid == false) {
										fprintf(stderr, "Unable to load bitmap (image id: %d)\n", i);
										i--;	//se incrementa uno mas de los que se inicializan correctamente

										while (i-- != 0) {
											al_destroy_bitmap(images[i]);
										}

										al_destroy_timer(timer);		//destruir todo lo que se inicializo
										timer = NULL;
										al_shutdown_image_addon();
										al_destroy_event_queue(evQ);
										evQ = NULL;	
										al_destroy_display(display);
										display = NULL;
										free(images);
										images = NULL;
									}
									else { //en este punto ya todo salio bien
										nImages = _nImages;
										al_register_event_source(evQ, al_get_keyboard_event_source());
									    al_register_event_source(evQ, al_get_timer_event_source(timer));

										al_clear_to_color(al_map_rgb(0,0,0));	//fondo negro
										al_draw_bitmap(images[0], 0, 0, 0);		//cargar imagen de fondo
										al_start_timer(timer);
									}
								}
								else {
									fprintf(stderr, "Unable to create timer\n");
									al_shutdown_image_addon();		//si algo fallo, destruir todo lo que
									al_destroy_event_queue(evQ);	//ya se habia inicializado
									evQ = NULL;
									al_destroy_display(display);
									display = NULL;
									free(images);
									images = NULL;
								}
							}
							else {
								fprintf(stderr, "Unable to install image add-on\n");
								al_destroy_event_queue(evQ);
								evQ = NULL;
								al_destroy_display(display);
								display = NULL;
								free(images);
								images = NULL;
							}
						}
						else {
							fprintf(stderr, "Unable to create event queue\n");
							al_destroy_display(display);
							display = NULL;
							free(images);
							images = NULL;
						}
					}
					else {
						fprintf(stderr, "Unable to install keyboard\n");
						al_destroy_display(display);
						display = NULL;
						free(images);
						images = NULL;
					}
				}
				else {
					fprintf(stderr, "Unable to create display\n");
					free(images);
					images = NULL;
				}
			}
			else {
				fprintf(stderr, "Unable to initialize allegro\n");
				free(images);
				images = NULL;
			}
		}
	}
}


bool WormsIo :: isValid()
{
	return valid;
}


void WormsIo :: destroy()
{
	while (nImages-- != 0) {
		al_destroy_bitmap(images[nImages]);
	}

	al_destroy_timer(timer);		//destruir todo lo que se inicializo
	timer = NULL;
	al_shutdown_image_addon();
	al_destroy_event_queue(evQ);
	evQ = NULL;	
	al_destroy_display(display);
	display = NULL;
	free(images);
	images = NULL;

	valid = false;
}


uint32_t WormsIo :: getEvent()
{	
	ALLEGRO_EVENT ev;
	uint32_t evCode = NO_EVENT;

	// si al_get_next_event devuelve false, no se registro un nuevo evento
	// en ese caso, queda el valor de evCode que ya se puso
	if ( al_get_next_event(evQ, &ev) == true ) {
		switch (ev.type) {

			case ALLEGRO_EVENT_KEY_UP: { //se levanto una tecla. determinar cual
				switch (ev.keyboard.keycode) {
					case ALLEGRO_KEY_UP:	evCode = UP_UP;
					break;
					case ALLEGRO_KEY_LEFT:	evCode = LEFT_UP;
					break;
					case ALLEGRO_KEY_DOWN:	evCode = DOWN_UP;
					break;
					case ALLEGRO_KEY_RIGHT: evCode = RIGHT_UP;
					break;
					case ALLEGRO_KEY_W:		evCode = W_UP;
					break;
					case ALLEGRO_KEY_A:		evCode = A_UP;
					break;
					case ALLEGRO_KEY_S:		evCode = S_UP;
					break;
					case ALLEGRO_KEY_D:		evCode = D_UP;
					break;
					case ALLEGRO_KEY_ESCAPE:	evCode = EXIT;
					break; //teoricamente ya se deberia haber terminado el programa, por las dudas
					default:				evCode = NO_EVENT;
					break;	//default se agrega por claridad, queda el valor que ya estaba
				}
			}
			break;

			case ALLEGRO_EVENT_KEY_DOWN: { //se apreto una tecla. determinar cual
				switch (ev.keyboard.keycode) {
					case ALLEGRO_KEY_UP:		evCode = UP_DOWN;
					break;
					case ALLEGRO_KEY_LEFT:		evCode = LEFT_DOWN;
					break;
					case ALLEGRO_KEY_DOWN:		evCode = DOWN_DOWN;
					break;
					case ALLEGRO_KEY_RIGHT:		evCode = RIGHT_DOWN;
					break;
					case ALLEGRO_KEY_W:			evCode = W_DOWN;
					break;
					case ALLEGRO_KEY_A:			evCode = A_DOWN;
					break;
					case ALLEGRO_KEY_S:			evCode = S_DOWN;
					break;
					case ALLEGRO_KEY_D:			evCode = D_DOWN;
					break;
					case ALLEGRO_KEY_ESCAPE:	evCode = EXIT;
					break;
					default:					evCode = NO_EVENT;
					break;
				}
			}
			break;

			//el unico posible evento de timer es que paso el tiempo correspondiente a un frame
			case ALLEGRO_EVENT_TIMER: 		evCode = TIMEOUT;
			break;
			
			//se recibio un tipo no valido, no cuenta como evento en este programa
			default:						evCode = NO_EVENT; 
			break;
		}
	}
	
	return evCode;
}


bool WormsIo :: drawImage(uint32_t imgId, uint32_t x, uint32_t y, bool flipHorizontal)
{
	if (imgId < 0 || imgId >= nImages || x >= SCREEN_WIDTH  || y >= SCREEN_HEIGHT)
		return false; //parametro recibidos no validos

	al_draw_bitmap(images[imgId], x, y, flipHorizontal ? ALLEGRO_FLIP_HORIZONTAL : 0);
	// con ALLEGRO FLIP HORIZONTAL la imagen se pone al reves, con 0 tal como estaba
	return true;	//indicar que se pudo dibujar
}

