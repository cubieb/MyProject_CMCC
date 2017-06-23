#include "utils.h"


#if 0
/*
 * The setitimer() is Linux-specified.
 */
int setTimer(int microsec, void ((*sigroutine)(int)))
{
  struct itimerval value, ovalue;

  signal(SIGALRM, sigroutine);
  value.it_value.tv_sec = 0;
  value.it_value.tv_usec = microsec;
  value.it_interval.tv_sec = 0;
  value.it_interval.tv_usec = microsec;
  return setitimer(ITIMER_REAL, &value, &ovalue);
}

void stopTimer(void)
{
  struct itimerval value, ovalue;

  value.it_value.tv_sec = 0;
  value.it_value.tv_usec = 0;
  value.it_interval.tv_sec = 0;
  value.it_interval.tv_usec = 0;
  setitimer(ITIMER_REAL, &value, &ovalue);
}

static void resetTimerAll(void)
{
	stopTimer();
	g_wps_timer_state = 0;
}

#endif 

int main(int argc, char *argv[])
{


}





