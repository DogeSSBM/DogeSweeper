#pragma once

typedef struct EventHandler{
	EventType type;
	EventCallback callback;
	struct EventHandler *next;
}EventHandler;

EventHandler* appendEventHandler(EventHandler *list, EventHandler *handler)
{
	EventHandler *current = list;
	while(current->next != NULL){
		current = current->next;
	}
	current->next = handler;
	return list;
}

EventHandler* registerEvent(EventHandler *handlerList, EventType type, EventCallback callback)
{
	EventHandler *handler = malloc(sizeof(EventHandler));
	handler->type = type;
	handler->callback = callback;
	handler->next = NULL;
	if(handlerList == NULL)
		return handler;
	return appendEventHandler(handlerList, handler);
}

void handleEvent(EventHandler *handlerList, Event event)
{
	EventHandler *current = handlerList;
	while(current != NULL){
		if(event.type == current->type){
			(*current->callback)(event);
		}
		current = current->next;
	}
}

void events(const Ticks frameEnd, EventHandler *handlerList)
{
	i32 ticksLeft = frameEnd - getTicks();
	do{
		Event event;
		if(!SDL_WaitEventTimeout(&event, ticksLeft))
			return;
		switch(event.type){
		case SDL_QUIT:
			printf("Quitting now!\n");
			exit(0);
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE){
				printf("Quitting now!\n");
				exit(0);
				break;
			}
		default:
			handleEvent(handlerList, event);
			break;
		}
		ticksLeft = frameEnd - getTicks();
	}while(ticksLeft > 0);
}
