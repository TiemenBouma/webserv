NAME 	:= webserv
FLAGS 	:= -Wall -Werror -Wextra -std=c++98
INC		:= -I includes
CONF 	:= data/config_tiemen.conf

CC = c++
RM = rm -rf

HEADERS 		:=	includes/webserver.h \
					includes/typedef.h \
					classes/Connection.hpp \
					classes/Request.hpp \
					classes/Config.hpp \
					classes/Response.hpp

INCLUDE_FLAGS 	:= $(addprefix -I , $(sort $(dir $(HEADERS))))

		classes/Config.cpp \
SRC = 	classes/Request.cpp \
		classes/Response.cpp \
		sources/webserver.cpp \
		classes/Connection.cpp \
		sources/request_get.cpp \
		sources/request_post.cpp \
		sources/request_error.cpp \
		sources/request_delete.cpp \
		sources/webserver_init.cpp  \
		sources/request_execute.cpp \
		sources/init_mime_types.cpp \
		sources/webserver_receive_request.cpp \
		sources/webserver_handle_connection.cpp \
		sources/main.cpp


OBJ = $(SRC:%.cpp=obj/%.o)

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(FLAGS) $(OBJ) -o $(NAME) $(INCLUDE_FLAGS)

obj/%.o : %.cpp
	@mkdir -p $(dir $@)
	c++ -c $(FLAGS) $< -o $@ $(INCLUDE_FLAGS)

clean:
	$(RM) $(OBJ)
	$(RM) obj/

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: all
	./$(NAME) $(CONF)

.PHONY: clean fclean re
