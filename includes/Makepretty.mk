
# MAKE IT PRETTY
~~ 	= ~~~~~~~~~
~~~	= $(~~)$(~~)$(~~)
--	= --------------
N	= \n
NNN	= \n\n\n

# COLORS
RED		= \x1b[31;01m
PINK	= \x1b[35m
BLUE	= \x1b[34m
CIEN	= \x1b[36m
GREEN	= \x1b[32m
YELLOW	= \x1b[33m
RESET	= \x1b[0m

PROJECT			= $(NAME)

LIBFT_START		= $(ECHO) "$(BLUE)$(--)$(--)$(N)Starting LIBFT now$(N)$(--)$(--)$(RESET)"
COMPILE_MESSAGE	= $(ECHO) "$(BLUE)Compiling$(RESET) $<"
EXTRA_MESSAGE	= $(ECHO) "$(LOVE)Compiling$(RESET) $<"

FLAGS_MESSAGE	= $(ECHO) "$(PINK)Compiling$(RESET) with flags: $(CFLAG)$(N)$(PINK)Compiling$(RESET) with $(LFLAG)"
COMPILE_PROJECT = $(ECHO) "$(PINK)Compiling$(RESET) $<"
PROJECT_START	= $(ECHO) "$(PINK)$(--)$(--)$(N)Starting $(PROJECT) now$(N)$(--)$(--)$(RESET)"
PROJECT_MESSAGE = $(ECHO) "$(YELLOW)~~~~~~~~~ Done, you can now run $(PROJECT)$(RESET)"
START_MESSAGE	= $(ECHO)"$(N)$(RED)$(PROJECT)$(N)$(LOVE)$(~~)"

EMPTY_MESSAGE	= $(ECHO) ""
MESSY_MESSAGE	= $(ECHO) "$(GREEN)$@ $(PINK)$(PROJECT)"
DONE_MESSAGE 	= $(ECHO) "$(YELLOW)      done.$(RESET)"
DONE_LIBFT		= $(ECHO) "$(BLUE)$(--)$(--)$(N)Compiling LIBFT done.$(N)$(--)$(--)$(RESET)"

RESET_MESSAGE_3	= $(ECHO) "	   $(RESET)done"
RESET_MESSAGE_2	= $(ECHO) "	$(PINK)RE$(GREEN)COM$(YELLOW)PI$(BLUE)LIN$(LOVE)G"
RESET_MESSAGE_1	= $(ECHO) "	$(RESET) $(PROJECT)\n"

ADD_GIT_IGNORE	= $(ECHO) $@ "\n"obj "\n".gitignore >> .gitignore
BYE_GIT_IGNORE	= @$(RM) .gitignore