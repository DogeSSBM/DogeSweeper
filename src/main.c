#include "DogeLib/Includes.h"

int main(int argc, char const *argv[])
{
	const Length window = {800, 600};
	init(window);

	while(1){
		Ticks frameStart = getTicks();
		clear();



		draw();
		events(frameStart + TPF);
	}
	return 0;
}
