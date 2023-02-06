NAME := webserver
FLAGS := -Wall -Werror -Wextra -std=c++98 -Wimplicit-fallthrough

SRC = 	classes/Request.cpp \
		classes/Response.cpp \
		classes/Connection.cpp \
		webserver.cpp \
		request_get.cpp \
		request_execute.cpp \
		request_post.cpp \
		request_delete.cpp \
		init_mime_types.cpp \
		webserver_init.cpp  \
		webserver_handle_connection.cpp \
		webserver_receive_request.cpp 


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