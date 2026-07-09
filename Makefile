# add mlx to path: `export DYLD_LIBRARY_PATH=/Users/jwikiera/Projets/fdf/FDF/mlx:$DYLD_LIBRARY_PATH`

NAME					:= minirt

UNAME_S					:= $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  FSANITIZE				:= -fsanitize=address -fsanitize=leak
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
CFLAGS					:= -o3 -Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-variable -ggdb $(FSANITIZE) $(OS_FLAG)
RM						:= rm -f

LIB_DIRECTORY			:= ./libs/

LIBFT					:= $(LIBFT_DIRECTORY)libft.a
LIBFT_DIRECTORY			:= $(LIB_DIRECTORY)libft/
LIBFT_HEADERS			:= $(LIBFT_DIRECTORY)./

GC						:= $(GC_DIRECTORY)gc.a
GC_DIRECTORY			:= $(LIB_DIRECTORY)gc/
GC_HEADERS				:= $(GC_DIRECTORY)./

GNL						:= gnl
GNL_LIB					:= $(GNL_DIRECTORY)$(GNL).a
GNL_DIRECTORY			:= $(LIB_DIRECTORY)$(GNL)/
GNL_HEADERS				:= $(GNL_DIRECTORY)./				

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
LIBRARIES				:= -lmlx -lm -lft -lgc -l$(GNL) -L$(GNL_DIRECTORY) -L. -L$(LIBFT_DIRECTORY) -L$(GC_DIRECTORY) -L$(MINILIBX_DIRECTORY) $(FRAMEWORK) $(LINUX_LIBS)
INCLUDES				:= -I$(LIBFT_HEADERS) -I$(GC_HEADERS) -I$(MINILIBX_HEADERS) -I$(GNL_HEADERS) -I$(INCLUDE_DIR) $(LINUX_INCLUDES)

SOURCES_DIRECTORY		:= ./sources/
SOURCES_LIST			:=	main.c\
							trashcan.c\
							backgrounds.c\
							hit_functions/box.c\
							hit_functions/cone.c\
							hit_functions/cylinder.c\
							hit_functions/plane.c\
							hit_functions/quad.c\
							hit_functions/sphere.c\
							hit_functions/ss_quad.c\
							hit_functions/utils.c\
							math/matrix4.c\
							math/projection.c\
							math/quaternion.c\
							math/random.c\
							math/ray.c\
							math/reflection.c\
							math/scalar.c\
							math/transform.c\
							math/trash.c\
							math/tween.c\
							math/vector3.c\
							draw/color.c\
							draw/gizmo.c\
							draw/pixel.c\
							draw/scribe.c\
							draw/tone.c\
							event/mouse_handler.c\
							event/keypress_handler.c\
							lighting/pbr.c\
							lighting/lamps.c\
							material/material.c\
							material/scatter.c\
							material/textures_1.c\
							material/textures_2.c\
							material/textures_3.c\
							raytrace/raytrace.c\
							raytrace/paint.c\
							raytrace/pixel.c\
							raytrace/tasks.c\
							raytrace/trace.c\
							raytrace/simulation.c\
							raster/heatmap.c\
							raster/raster.c\
							raster/shapes.c\
							disk/ffmpeg.c\
							disk/read.c\
							disk/write.c\
							ui/debug.c\
							ui/help.c\
							update/camera.c\
							update/data.c\
							update/item.c\
							update/main_loop.c\
							parsing/parsing.c\
							parsing/parsing_atoos.c\
							parsing/parsing_functions_1.c\
							parsing/parsing_functions_2.c\
							parsing/parsing_functions_3.c\
							parsing/parsing_functions_4.c\
							premade_scenes/scenes_1.c\
							premade_scenes/scenes_2.c\
							premade_scenes/scenes_3.c\
							premade_scenes/scenes_4.c\
							premade_scenes/scenes_5.c\
							premade_scenes/scenes_6.c\
							premade_scenes/scenes_7.c\


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
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@
	#@echo "$(GREEN).$(RESET)\c"

$(LIBFT):
	@echo "$(NAME): Creating $(LIBFT)..."
	@$(MAKE) -sC $(LIBFT_DIRECTORY)

$(GC):
	@echo "$(NAME): Creating $(GC)..."
	@$(MAKE) -sC $(GC_DIRECTORY)

$(GNL):
	@echo "$(NAME): Creating $(GNL_LIB)..."
	@$(MAKE) -sC $(GNL_DIRECTORY)

$(MINILIBX):
	@echo "$(NAME): Creating $(MINILIBX)..."
	cd $(MINILIBX_DIRECTORY) && $(MAKE)
	cp $(MINILIBX) .

$(NAME): $(LIBFT) $(GC) $(GNL) $(MINILIBX) $(OBJECTS_DIRECTORY) $(OBJECTS) $(HEADER_FILES)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBRARIES) $(INCLUDES) -o $(NAME)
ifeq ($(UNAME_S),Linux)
	@export ASAN_OPTIONS=use_sigaltstack=false
endif

clean:
	@$(MAKE) -sC $(LIBFT_DIRECTORY) clean
	@$(MAKE) -sC $(GC_DIRECTORY) clean
	@$(MAKE) -sC $(GNL_DIRECTORY) clean
	@$(MAKE) -sC $(MINILIBX_DIRECTORY) clean
	@rm -rf $(OBJECTS_DIRECTORY)
	@rm -rf *.dSYM
	@echo "$(NAME): $(RED)$(OBJECTS_DIRECTORY) was deleted$(RESET)"
	@echo "$(NAME): $(RED)object files were deleted$(RESET)"

fclean: clean
	@$(MAKE) -sC $(LIBFT_DIRECTORY) fclean
	@$(MAKE) -sC $(GC_DIRECTORY) fclean
	@$(MAKE) -sC $(GNL_DIRECTORY) fclean
	@rm -f $(MINILIBX)
	@echo "$(NAME): $(MINILIBX) was deleted"
	@rm -f $(NAME)
	@echo "$(NAME): $(NAME) was deleted"
	@rm -f $(MINILIBX_NAME)
    @echo "$(NAME): $(MINILIBX_NAME) was deleted"

re: fclean all

norm:
	norminette includes/ sources/ libs/libft/ libs/libprintf/ libs/gc

funcs: $(NAME)
	nm -u $(NAME)

test:	all
	./$(NAME)
