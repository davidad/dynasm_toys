all: popcnt1

%.c: %.dasc
	luajit dynasm/dynasm.lua -o $@ $<

%: %.c
	gcc -o $@ $<
