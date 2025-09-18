NAME =		PmergeMe

OBJ_DIR =	obj/

INC = \
			PmergeMe.hpp

SRC = \
			main.cpp \
			PmergeMe.cpp

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
	--track-fds=yes \
	./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
# 	./PmergeMe `shuf -i 1-100000 -n 10000 | tr "\n" " "`
# 	./PmergeMe `shuf -i 1-100 -n 30 | tr "\n" " "`
# 	./PmergeMe 82 89 68 11 39 2 36 29 69 99
# 	./PmergeMe 99 8 66 21 86 33 10 7 5 43
# 	./PmergeMe 11 14 26 59 38 13 33 39 18 43 46 55 82 23 3 89 36 72 53 64 60 22 37 19 90 28 1 21 6 70


.PHONY: all clean fclean re val
