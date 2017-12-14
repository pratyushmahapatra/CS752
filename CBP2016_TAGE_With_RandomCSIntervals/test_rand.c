#include <stdio.h>
#include <stdlib.h>

int main(){

srand(10);

for(int i=0; i<100; i++){
int switch_interval=(abs((rand()) * 23)  % 495000) + 5000;

printf("\n random number : %d", switch_interval);
}

}
