#omni_robot make file
omni_control:	omni_control.c
	gcc omni_control.c -o omni_control -lm
