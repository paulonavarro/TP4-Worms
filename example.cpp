class worm {
	private
	{
		position p;
		bool lookingRigth;
		wormState iAmDoing;
		ulong frameCount;
	}
	public
	{
		worm();
		void stateJumping();		//
		void stateMoving();
		void continueAction();
		void stopMoving();
		void stopJumping();
		getPosition();
		getWormState();
		getwormFrame();
		bool lookRigth();
	}
};

typedef enum {still, walking, jumping, walkingPending, finishWalking}wormState;
//walkingpending espera a q pasen 5 frames para poder caminar, sino togglea
//finishWalking no tiene la tecla presionada pero se sigue moviendo

do {
	ev = al_get_next_event(event);
	if (ev != NO_EVENT)
	{
		dispalch(ev, wormArray);
	}
} while (c != EXIT)


//En cada frame realizo un continueAction() y redibujo con allegro.
//Si choca sigue el movimiento quedando contra la pared NO REBOTA
