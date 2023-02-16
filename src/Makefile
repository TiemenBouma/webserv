
include ../includes/Makepretty.mk

NAME := webserv
FLAGS := -Wall -Werror -Wextra -std=c++98
TEST = ../testers/web_swirls/

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
		init_mime_types.cpp \
		webserver_init.cpp  \
		webserver_handle_connection.cpp \
		webserver_receive_request.cpp \
		main.cpp


OBJ = $(SRC:%.cpp=obj/%.o)

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(FLAGS) $(OBJ) -o $(NAME) $(INCLUDE_FLAGS)

obj/%.o : %.cpp
	@mkdir -p $(dir $@)
	c++ -c $(FLAGS) $< -o $@ $(INCLUDE_FLAGS)

clean:
	$(RM) $(OBJ)
	$(RM) -r obj/

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: re 
	./$(NAME) ../data/default.conf

test:
	@echo "$(RED)$(NNN)	___ TESTING ___ $(NNN)$(RESET)"
	curl -svo /dev/null http://localhost:8080
#	for connection:
# curl http://localhost:8080
# For response (header and body)
# curl -sv /dev/null http://localhost:8080

.PHONY: clean fclean re