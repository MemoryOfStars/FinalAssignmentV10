draw:maindraw.c draw.c front.c draw.h front.h
	arm-linux-gcc -o draw buttons_test.c draw.c front.c draw.h front.h

clean:
	-rm draw

