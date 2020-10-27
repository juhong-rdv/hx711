/* 
 gurov was here, use this code, or don't, whatever, I don't care. If you see a giant bug with a billion legs, please let me know so it can be squashed

*/
#include "hx711.h"

float g_filter_low=0, g_filter_high=0;

void setHighPri (void)
{
  struct sched_param sched ;

  memset (&sched, 0, sizeof(sched)) ;

  sched.sched_priority = 10 ;
  if (sched_setscheduler (0, SCHED_FIFO, &sched))
    printf ("Warning: Unable to set high priority\n") ;
}


void setup_gpio()
{
  INP_GPIO(DATA_PIN);
  //INP_GPIO(CLOCK_PIN);  
  OUT_GPIO(CLOCK_PIN);
  SCK_OFF;

//   GPIO_PULL = 0;
//   short_wait();
//   GPIO_PULLCLK0 = 1 << DATA_PIN;
 //  short_wait();
//   GPIO_PULL = 0;
//   GPIO_PULLCLK0 = 0;

/*   GPIO_PULL = 2;
   short_wait();
   GPIO_PULLCLK0 = 1 << DATA_PIN;
   short_wait();
   GPIO_PULL = 0;
   GPIO_PULLCLK0 = 0;*/
}

void 	unpull_pins()
{
   GPIO_PULL = 0;
//   short_wait();
   GPIO_PULLCLK0 = 1 << DATA_PIN;
//   short_wait();
   GPIO_PULL = 0;
   GPIO_PULLCLK0 = 0;
} // unpull_pins

void reset_converter(void) {
	SCK_ON;
	usleep(60);
	SCK_OFF;
	usleep(60);
}

void set_gain(int r) {
	int i;

// r = 0 - 128 gain ch a
// r = 1 - 32  gain ch b
// r = 2 - 63  gain ch a

	while( DT_R ); 

	for (i=0;i<24+r;i++) {
		SCK_ON;
		SCK_OFF;
	}
}


unsigned long read_cnt(long offset, int argc) {
	long count;
	int i;
	int b;


  count = 0;


  while( DT_R ); 
	b++;
	b++;
	b++;
	b++;

  for(i=0;i<24	; i++) {
	SCK_ON;
        count = count << 1;
	b++;
	b++;
	b++;
	b++;
        SCK_OFF;
	b++;
	b++;
        if (DT_R > 0 ) { count++; }
//	b++;
//	b++;
  }


	SCK_ON;
	b++;
	b++;
	b++;
	b++;
	SCK_OFF;
	b++;
	b++;
	b++;
	b++;
//  count = ~0x1800000 & count;
//  count = ~0x800000 & count;


 if (count & 0x800000) {
	count |= (long) ~0xffffff;
 }

// if things are broken this will show actual data

#if 1
if (argc < 0 ) {
  for (i=31;i>=0;i--) {
   printf("%d ", ((count-offset) & ( 1 << i )) != 0 );
  }

  printf("n: %10d     -  ", (int)(count - offset));
  printf("\n"); 
}
#endif

  return (count - offset);

}

long Init(int argc, char **argv)
{
	printf("Initial HX711 and Value\n") ;
	
	long ret = -1 ;
	
	int i, j;
  long tmp=0;
  long tmp_avg=0;
  long tmp_avg2;
  long offset=0;
  float spread_percent = SPREAD / 100.0 /2.0;
  int b;
  int nsamples=N_SAMPLES;
  long samples[nsamples];

  if (argc == 2) {
   offset = atol(argv[1]);
  }

  setHighPri();
  setup_io();
  setup_gpio();
  reset_converter();

  j=0;

	
  // get the dirty samples and average them
  for(i=0;i<nsamples;i++) {
  	reset_converter();
  	samples[i] = read_cnt(0, -1);
  	tmp_avg += samples[i];

	//printf("\b\b\b\b") ;
	//printf("%02d%%", (int)(((float)i / (float)nsamples)*100.0)) ;
  }
  //printf("\n") ;

  tmp_avg = tmp_avg / nsamples;

  tmp_avg2 = 0;
  j=0;

  g_filter_low =  (float) tmp_avg * (1.0 - spread_percent);
  g_filter_high = (float) tmp_avg * (1.0 + spread_percent);

  printf("%d %d\n", (int) g_filter_low, (int) g_filter_high);

  for(i=0;i<nsamples;i++) 
  {
	if ((samples[i] < g_filter_high && samples[i] > g_filter_low) || 
            (samples[i] > g_filter_high && samples[i] < g_filter_low) ) {
		tmp_avg2 += samples[i];
	        j++;
	}
  }

  if (j == 0) {
    printf("No data to consider\n");
  }
  else
  {
  	ret = (tmp_avg2 / (long)j) - offset ;
	
  	printf("%ld\n", ret);
  }

  return ret ;
}

void DeInit(void)
{
	unpull_pins();
  restore_io();
}

long GetData(void)
{
	reset_converter();

	long data = 0 ;
	int count = 10 ;
	while(count >= 0 )
	{
		data = read_cnt(0, 0) ;

		if ((data < g_filter_high && data > g_filter_low) || 
	            (data > g_filter_high && data < g_filter_low) ) 
	    {
			break ;
		}

		count-- ;
	}
			
	return data;
}


#if 0
int main(int argc, char **argv)
{
  
	long value_init = Init(argc, argv) ;

	while(1) 
	{
		long data = GetData();

		printf("init(%10d), data=%10d\n", (int)value_init, (int)data ) ;
	}
	
  	DeInit() ;
}
#endif

