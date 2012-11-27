/*
* test.c
*/

#define HOLT_BASE_ADDR 0x8000

unsigned int t0;
unsigned int t1;
unsigned int *t0_ptr = HOLT_BASE_ADDR;
//unsigned int *t1_ptr;


int main(void)
{
	
	t0_ptr[0] = 0xA5A5;
	t0_ptr[1] = 0x5A5A;
	
	t0 = *t0_ptr;
	t1 = t0;

	while(1){};
	
	return 0;
}