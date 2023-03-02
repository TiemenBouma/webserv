
# COLORS
RED		= \x1b[31;01m
END 	= \x1b[0m\n
PINK	= \x1b[35m
BLUE	= \x1b[34m
CYAN	= \x1b[36m
GREEN	= \x1b[32m
YELLOW	= \x1b[33m
RESET	= \x1b[0m

# COMMANDS
CC			= c++
RM 			= rm -rf
ECHO 		= @echo ""
NEW_DIR 	= mkdir -p
MAKE_C 		= make -C
MAKE_FC		= make fclean -C

# MESSAGES

COMPILE_MESSAGE	= $(ECHO) "$(BLUE)Compiling$(RESET) $<"
FLAGS_MESSAGE	= $(ECHO) "$(PINK)Compiling$(RESET) with flags: $(CFLAG)$(N)$(PINK)Compiling$(RESET) with $(LFLAG)"

PROJECT			= $(NAME)
PROJECT_START	= $(ECHO) "$(PINK)$(--)$(--)$(N)Starting $(PROJECT) now$(N)$(--)$(--)$(RESET)"
START_MESSAGE	= $(ECHO)"$(N)$(RED)$(PROJECT)$(N)$(LOVE)$(~~)"
PROJECT_MESSAGE = $(ECHO) "$(YELLOW)~~~~~~~~~ Done, you can now run $(PROJECT)$(RESET)"

MESSY_MESSAGE	= $(ECHO) "$(GREEN)$@ $(PINK)$(PROJECT)"
DONE_MESSAGE 	= $(ECHO) "$(YELLOW)      done.$(RESET)"

RESET_MESSAGE_2	= $(ECHO) "	$(PINK)RE$(GREEN)COM$(YELLOW)PI$(BLUE)LIN$(LOVE)G"
RESET_MESSAGE_1	= $(ECHO) "	$(RESET) $(PROJECT)\n"
RESET_MESSAGE_3	= $(ECHO) "	   $(RESET)done"

# UPDATE IGNORE
ADD_GIT_IGNORE	= $(ECHO) $@ "\n"obj "\n".gitignore >> .gitignore
BYE_GIT_IGNORE	= @$(RM) .gitignore
