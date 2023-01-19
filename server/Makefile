NAME := webserver
FLAGS := -Wall -Werror -Wextra -std=c++98

SRC = 	webserver.cpp \
		Request.cpp \
		request_get.cpp 

OBJ = $(SRC:%.cpp=obj/%.o)

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(FLAGS) $(OBJ) -o $(NAME)

obj/%.o : %.cpp
	@mkdir -p $(dir $@)
	c++ -c $(FLAGS) $< -o $@

clean:
	$(RM) $(OBJ)
	$(RM) -r obj/

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re