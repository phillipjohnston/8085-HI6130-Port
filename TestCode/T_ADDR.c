/*
* test.c
*/

#define HOLT_BASE_ADDR (0x8000)

int main(void)
{
	unsigned int t0;
	unsigned int t1;
	unsigned int t[2];
	unsigned int * t0_ptr;
	unsigned int * t1_ptr;
	unsigned int * t_ptr;
	
	t0 = 0xBECE;
	t1	= 0xFDAD;
	t[0] = 0xBECE;
	t[1] = 0xFDAD;
	t0_ptr = (unsigned int *) (HOLT_BASE_ADDR);
	t1_ptr = (unsigned int *) (HOLT_BASE_ADDR + sizeof(unsigned int));
	t_ptr = t0_ptr;
	t0 = *t0_ptr;
	
	
	
	*t0_ptr = t0;
	*t1_ptr = t1;
	
	*t0_ptr = 0;
	*t1_ptr = 1;
	
	t_ptr[0] = t[0];
	t_ptr[1] = t[1];

	while(1){};
	
	return 0;
}