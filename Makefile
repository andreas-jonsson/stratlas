NAME = stratlas

all:	$(NAME) $(NAME)_cmd

$(NAME):
	make -f Makefile.gui

$(NAME)_cmd:
	make -f Makefile.cmd

clean:
	make -f Makefile.gui clean
	make -f Makefile.cmd clean
