include includes/colors.mk

NAME 	:= webserv
INC		:= -I includes
FLAGS 	:= -Wall -Werror -Wextra -std=c++98
CONF 	:= utils/config_tiemen.conf

HEADERS 		:=	includes/typedef.h \
					classes/Config.hpp \
					classes/Response.hpp \
					classes/Cgi.hpp
					classes/Request.hpp \
					classes/Connection.hpp \
					classes/Response.hpp \
					includes/webserver.h

INCLUDE_FLAGS 	:= $(addprefix -I , $(sort $(dir $(HEADERS))))

SRC = 	sources/main.cpp \
		classes/Config.cpp \
		classes/Cgi.cpp \
		classes/Request.cpp \
		classes/Response.cpp \
		sources/webserver.cpp \
		classes/Connection.cpp \
		sources/request_get.cpp \
		sources/webserver_handle_request.cpp \
		sources/request_post.cpp \
		sources/request_error.cpp \
		sources/request_delete.cpp \
		sources/webserver_init.cpp  \
		sources/webserver_receive.cpp \
		sources/init_mime_types.cpp


OBJ = $(SRC:%.cpp=objects/%.o)

all: $(NAME)

$(NAME) : $(OBJ)
	@c++ $(FLAGS) $(OBJ) -o $(NAME) $(INCLUDE_FLAGS)
	$(ECHO) "Compiled with $(CC) $(FLAGS)"

objects/%.o : %.cpp
	@mkdir -p $(dir $@)
	@c++ -c $(FLAGS) $< -o $@ $(INCLUDE_FLAGS)
	$(ECHO) "Compiling $^"

clean:
	@$(RM) $(OBJ)
	@$(RM) objects/ obj/
	$(ECHO) "Removing objects"

fclean: clean
	@$(RM) $(NAME)
	$(ECHO) "Removing $(NAME)"

re: fclean all

run: all
	./$(NAME) $(CONF)

test: re
	$(ECHO) "$(CYAN)Running Siege$(END)"
	siege -R siege_test/siege.conf > siege_test/test_output.txt
	$(ECHO) "$(GREEN)Siege Success [OK]$(END)"

.PHONY: clean fclean re