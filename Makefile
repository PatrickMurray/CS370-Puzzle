NAME    := puzzle
TARBALL := pmurray1_mmahoney_jromph_$(NAME).tar.gz

SRC     := main.c puzzle.c piece.c edge.c permutations.c
HEADERS := headers.h
FILES   := Makefile

CFLAGS      := -g -pg -Wall -Werror -Wpedantic -Ofast
OBJ         := $(SRC:.c=.o)
LINUX_LINKS := -lbsd


all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LINUX_LINKS)


clean:
	-$(RM) *~
	-$(RM) \#*
	-$(RM) *.o
	-$(RM) *.core
	-$(RM) gmon.out


fclean: clean
	-$(RM) $(NAME)


re: fclean all


package: fclean
	tar cvzf $(TARBALL) $(FILES)
