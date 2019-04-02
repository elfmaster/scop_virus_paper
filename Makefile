all:
	cp test_scop.backup test_scop
	gcc scop_infector.c /opt/elfmaster/lib/libelfmaster.a -o scop_infect
	gcc -N -fPIC -pie -static -nostdlib egg.c -o egg
clean:
	rm -f scop_infect egg

