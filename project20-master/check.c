#include "ap_cint.h"
#define N 32

typedef int8 din_t;
typedef int13 dout_t;
typedef uint5 dsel_t;

dout_t loop_max_bounds(din_t A[N], dsel_t width) {
dout_t out_accum=0;
dsel_t x;
LOOP_X:for (x=0;x<N; x++) {
if (x<width) {
out_accum += A[x];
}
}
return out_accum;
}

int main() {

printf("Hello World!!");
int i;
din_t A[N] = {5};
dsel_t width = 15;
dout_t out_data;

for(i=0;i<N;i++)
	A[i] = 260;
out_data = loop_max_bounds(A, width);
printf("out data = %d\n",out_data);
printf("in data = %d\n",A[0]);

return 1;
}
