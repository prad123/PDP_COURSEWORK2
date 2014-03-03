#include <stdio.h>
#include "frog-functions.h"

int main(){
	long state = 1;
	long x = -5;
	float pos_x, pos_y;
	initialiseRNG(&x);
	for(int i = 0; i < 100; i++){
		state = i;
		frogHop(0,0, &pos_x, &pos_y, &state);
		printf("%f,%f - %ld\n", pos_x, pos_y, 
			getCellFromPosition(pos_x, pos_y));
		//for(int j = 0; j < 500; ++j){
			//state = -(10);
			//int b = willGiveBirth(i, &state);
			//if(b) printf("VOILA %ld\n", i);
			//printf("%ld %ld\n", i, willGiveBirth(i,&state));
		//}
	}
	printf("Done\n");
	return 1;
}
