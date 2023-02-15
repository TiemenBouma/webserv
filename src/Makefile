NAME 	:= webserv
FLAGS 	:= -Wall -Werror -Wextra -std=c++98
INC		:= -I ../includes
CONF 	:= ../data/config.cfg

CC = c++
RM = rm -rf

HEADERS 		:=	../includes/webserver.h	\
					../includes/typedef.h \
					classes/Connection.hpp \
					classes/Request.hpp \
					classes/Response.hpp

INCLUDE_FLAGS 	:= $(addprefix -I , $(sort $(dir $(HEADERS))))

SRC = 	classes/Request.cpp \
		classes/Response.cpp \
		classes/Connection.cpp \
		classes/Config.cpp \
		webserver.cpp \
		request_get.cpp \
		request_execute.cpp \
		request_post.cpp \
		request_delete.cpp \
<<<<<<< HEAD
		Response.cpp \
		init_mime_types.cpp \
		Config.cpp \
		main.cpp 
=======
		request_error.cpp \
		init_mime_types.cpp \
		webserver_init.cpp  \
		webserver_handle_connection.cpp \
		webserver_receive_request.cpp \
		main.cpp

>>>>>>> tiemen

OBJ = $(SRC:%.cpp=obj/%.o)

all: $(NAME)

$(NAME) : $(OBJ)
<<<<<<< HEAD
	$(CC) $(FLAGS) $(INC) $(OBJ) -o $(NAME) -fsanitize=address -g

obj/%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $(FLAGS) $< -o $@
=======
	c++ $(FLAGS) $(OBJ) -o $(NAME) $(INCLUDE_FLAGS)

obj/%.o : %.cpp
	@mkdir -p $(dir $@)
	c++ -c $(FLAGS) $< -o $@ $(INCLUDE_FLAGS)
>>>>>>> tiemen

clean:
	$(RM) $(OBJ)
	$(RM) obj/

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: all
	./$(NAME) $(CONF)

.PHONY: clean fclean re