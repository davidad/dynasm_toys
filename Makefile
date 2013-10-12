all: popcnt1 popcnt2 popcnt3

%.c: %.dasc
	luajit dynasm/dynasm.lua -o $@ $<

%: %.c
	gcc -o $@ $<
