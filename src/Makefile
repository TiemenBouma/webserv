NAME 	:= webserv
FLAGS 	:= -Wall -Werror -Wextra -std=c++98
INC		:= -I ../includes
CONF 	:= ../data/config.cfg

CC = c++
RM = rm -rf

SRC = 	webserver.cpp \
		Request.cpp \
		request_get.cpp \
		request_execute.cpp \
		request_post.cpp \
		request_delete.cpp \
		Response.cpp \
		init_mime_types.cpp \
		Config.cpp \
		main.cpp 

OBJ = $(SRC:%.cpp=obj/%.o)

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(INC) $(OBJ) -o $(NAME)

obj/%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $(FLAGS) $< -o $@

clean:
	$(RM) $(OBJ)
	$(RM) obj/

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: all
	./$(NAME) $(CONF)

.PHONY: clean fclean re