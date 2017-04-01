#ifndef WORMS_IO_H
#define WORMS_IO_H


extern "C" {
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
}
#include <cstdint>

#define FPS				50	//eventualmente puede recibir todo esto como param
#define SCREEN_WIDTH	800	
#define	SCREEN_HEIGHT	600

enum eventCodes { NO_EVENT, TIMEOUT, EXIT,  
// NO EVENT: no se registro nada desde el ultimo evento
// TIMEOUT: paso el tiempo que hay que esperar entre frame y frame

//EVENTOS DE TECLADO:
	W_UP, W_DOWN, A_UP, A_DOWN, S_UP, S_DOWN, D_UP, D_DOWN,
	UP_UP, UP_DOWN, LEFT_UP, LEFT_DOWN, DOWN_UP, DOWN_DOWN, RIGHT_UP, RIGHT_DOWN };
//codigos de evento de teclado: la primer palabra indica a tecla corresponde y la segunda si se apreto o solto
//p.ej: W_UP == se solto la tecla W
//		LEFT_DOWN == se apreto la flecha de la izquierda
//		DOWN_UP == se solto la flecha de abajo



class WormsIo {
private:
	bool valid;
	ALLEGRO_DISPLAY * display;
	ALLEGRO_BITMAP ** images;
	uint32_t nImages;
	ALLEGRO_EVENT_QUEUE * evQ;
	ALLEGRO_TIMER * timer;

public:
	WormsIo (uint32_t _nImages, char * imgFiles[]);
	// recibe el numero de imagenes que debe cargar, y un arreglo de punteros a los paths de los
	//archivos donde se encuentra cada una de ellas.
	// la primera imagen va a ser utilizada como fondo, con lo cual _nImages DEBE SER AL MENOS 1. 
	// el resto (si las hubiera) se van a poder dibujar con drawImage(). el id de cada imagen sera
	//igual a indice dentro de imgFiles[] que tenia el path.
	//	p.ej: si quiero dibujar la imagen cuyo path esta guardado en imgFiles[4], el id que le voy
	//a tener que pasar a drawImage() es 4. si le paso 0 (el fondo!) o un numero mayor o igual a nImages,
	//va a ser error

	bool drawImage(uint32_t imgId, uint32_t x, uint32_t y, bool flipHorizontal);
	// dibuja la imagen que indica imgId. la esquina SUPERIOR IZQUIERDA va a estar en x,y
	// si flipHorizontal es true, dibuja la imagen "mirando para el otro lado", si no la deja
	//tal como esta
	// devuelve true si los parametros recibidos eran validos y viceversa
	// notese que en cuanto a x e y, solo verifica que no se excedan del display, no si 
	//una parte de la imagen queda afuera a pesar de que su coordenada inicial sea correcta

	bool isValid();
	void destroy();
	uint32_t getEvent(); //los eventos se devuelven como estan indicados en eventCodes (arriba)
};



#endif //WORMS_IO_H