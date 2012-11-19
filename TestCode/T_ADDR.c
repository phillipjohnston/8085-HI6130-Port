/*
* test.c
*/

#define HOLT_BASE_ADDR 0x8000

int main(void)
{
	unsigned int t0;
	unsigned int t1;
	unsigned int * t0_ptr;
	unsigned int * t1_ptr;
	
	t0 = 0xBECE;
	t1	= 0xFDAD;
	t0_ptr = (unsigned int *) HOLT_BASE_ADDR;
	t1_ptr = (unsigned int *) (HOLT_BASE_ADDR + sizeof(unsigned int));
	
	*t0_ptr = t0;
	*t1_ptr = t1;

	while(1){};
	
	return 0;
}