NAME =		RPN

OBJ_DIR =	obj/

INC = \
			RPN.hpp

SRC = \
			main.cpp \
			RPN.cpp

OBJ =		$(patsubst %.cpp,$(OBJ_DIR)%.o,$(SRC))

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g

RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

$(OBJ_DIR)%.o: %.cpp $(INC)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

val:
	valgrind --leak-check=full --leak-check=full \
	--show-leak-kinds=definite,indirect,possible \
	--track-fds=yes ./$(NAME) \
	"8 9 * 9 - 9 - 9 - 4 - 1 +" \
	"7 7 * 7 -" \
	"1 2 * 2 / 2 * 2 4 - +" \
	"(1 + 1)"

test:
	./$(NAME) \
	"" \
	"1 1 + " \
	"11 +" \
	"1 A +" \
	"1 1 + 1" \
	"1" \
	"+" \
	"1 +" \
	"1 0 /" \
	"8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 1 - +" \
	"8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 1 - + 1 +" \
	"8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 1 - + 0 1 - * 1 -" \
	"8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 1 - + 0 1 - * 1 - 1 -" \
	"8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 8 * 1 - + 0 1 - * 1 - 0 1 - *" \
	"8 9 * 9 - 9 - 9 - 4 - 1 +"

.PHONY: all clean fclean re val test
