#include "hx711.h"

#if 1
int main(int argc, char **argv)
{
  
	long value_init = Init(argc, argv) ;

	while(1) 
	{
		long data = GetData();

		printf("init(%d), data=%d\n", (int)value_init, (int)data ) ;
	}
	
  	DeInit() ;
}
#endif

