# add mlx to path: `export DYLD_LIBRARY_PATH=/Users/jwikiera/Projets/fdf/FDF/mlx:$DYLD_LIBRARY_PATH`

NAME					:= minirt

UNAME_S					:= $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  FSANITIZE				:= #-fsanitize=address -fsanitize=leak
  FRAMEWORK				:=
  LINUX_LIBS			:= -lXext -lX11 -lXi
  LINUX_INCLUDES		:= -I/usr/include
  OS_FLAG				:= -D LINUX
else
  FSANITIZE				:=
  FRAMEWORK				:= -framework OpenGL -framework AppKit
  LINUX_LIBS			:=
  LINUX_INCLUDES		:=
  OS_FLAG				:= -D OSX
  ENV_VARIABLES			:=
endif

CC						:= gcc
CFLAGS					:= -pg -g -Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-variable -ggdb $(FSANITIZE) $(OS_FLAG)
RM						:= rm -f

LIB_DIRECTORY			:= ./libs/

LIBFT					:= $(LIBFT_DIRECTORY)libft.a
LIBFT_DIRECTORY			:= $(LIB_DIRECTORY)libft/
LIBFT_HEADERS			:= $(LIBFT_DIRECTORY)./

ifeq ($(UNAME_S),Darwin)
  MINILIBX_DIRECTORY	:= $(LIB_DIRECTORY)mlx
  MINILIBX_NAME			:= libmlx.dylib
else
  MINILIBX_DIRECTORY	:= $(LIB_DIRECTORY)mlx_linux
  MINILIBX_NAME			:= libmlx.a
endif

MINILIBX				:= $(MINILIBX_DIRECTORY)/$(MINILIBX_NAME)
MINILIBX_HEADERS		:= $(MINILIBX_DIRECTORY)

INCLUDE_DIR				:= ./includes/

# lm: default math lib
LIBRARIES				:= -lmlx -lm -lft -L. -L$(LIBFT_DIRECTORY) -L$(MINILIBX_DIRECTORY) $(FRAMEWORK) $(LINUX_LIBS)
INCLUDES				:= -I$(LIBFT_HEADERS) -I$(MINILIBX_HEADERS) -I$(INCLUDE_DIR) $(LINUX_INCLUDES)

SOURCES_DIRECTORY		:= ./sources/
SOURCES_LIST			:=	main.c\
							loop.c\
							draw_color.c\
							draw_gizmo.c\
							draw_scribe.c\
							event_handlers.c\
							img.c\
							item.c\
							raster_functions.c\
							raster.c\
							raytrace.c\
							ui.c\
							update_data.c\
							update_item.c\
							math_1.c\
							math_2.c\
							math_3.c\
							math_4.c\
							math_5.c\
							math_6.c\
							math_7.c\
							math_8.c\
							materials.c\
							envelope.c\
							hit_functions.c\
							update_camera.c\
							tweens.c\
							scenes.c\
							backgrounds.c\
							textures.c\
							disk.c\
							pbr_lighting.c\
							normals.c\
							transform.c\
							simulation.c\
							trashcan.c

SOURCES					:= $(addprefix $(SOURCES_DIRECTORY), $(SOURCES_LIST))
HEADER_LIST				:= fractol.h
HEADER_FILES			:= $(addprefix $(INCLUDE_DIR), $(HEADER_LIST))

OBJECTS_DIRECTORY		:= objects/
OBJECTS_LIST			:= $(patsubst %.c, %.o, $(SOURCES_LIST))
OBJECTS					:= $(addprefix $(OBJECTS_DIRECTORY), $(OBJECTS_LIST))

.PHONY: all clean fclean re

all: $(NAME)

$(OBJECTS_DIRECTORY):
	mkdir -p $(OBJECTS_DIRECTORY)
	#@echo "$(NAME): $(GREEN)$(OBJECTS_DIRECTORY) was created$(RESET)"

$(OBJECTS_DIRECTORY)%.o : $(SOURCES_DIRECTORY)%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@
	#@echo "$(GREEN).$(RESET)\c"

$(LIBFT):
	@echo "$(NAME): Creating $(LIBFT)..."
	@$(MAKE) -sC $(LIBFT_DIRECTORY)

$(MINILIBX):
	@echo "$(NAME): Creating $(MINILIBX)..."
	cd $(MINILIBX_DIRECTORY) && $(MAKE)
	cp $(MINILIBX) .

$(NAME): $(LIBFT) $(MINILIBX) $(OBJECTS_DIRECTORY) $(OBJECTS) $(HEADER_FILES)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBRARIES) $(INCLUDES) -o $(NAME)
ifeq ($(UNAME_S),Linux)
	@export ASAN_OPTIONS=use_sigaltstack=false
endif

clean:
	@$(MAKE) -sC $(LIBFT_DIRECTORY) clean
	@$(MAKE) -sC $(MINILIBX_DIRECTORY) clean
	@rm -rf $(OBJECTS_DIRECTORY)
	@rm -rf *.dSYM
	@echo "$(NAME): $(RED)$(OBJECTS_DIRECTORY) was deleted$(RESET)"
	@echo "$(NAME): $(RED)object files were deleted$(RESET)"

fclean: clean
	@$(MAKE) -sC $(LIBFT_DIRECTORY) fclean
	@rm -f $(MINILIBX)
	@echo "$(NAME): $(MINILIBX) was deleted"
	@rm -f $(NAME)
	@echo "$(NAME): $(NAME) was deleted"
	@rm -f $(MINILIBX_NAME)
    @echo "$(NAME): $(MINILIBX_NAME) was deleted"

re: fclean all

norm:
	norminette includes/ sources/ libs/libft/ libs/libprintf/

funcs: $(NAME)
	nm -u $(NAME)

test:	all
	./$(NAME)
