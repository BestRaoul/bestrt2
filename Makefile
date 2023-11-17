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
							backgrounds.c\
							loop.c\
							disk_ffmpeg.c\
							disk_read_1.c\
							disk_read_2.c\
							disk_read_3.c\
							disk_write.c\
							draw_color.c\
							draw_color2.c\
							draw_gizmo.c\
							draw_gizmo2.c\
							draw_scribe.c\
							event_handlers.c\
							event_handlers2.c\
							event_handlers3.c\
							hit_functions_1.c\
							hit_functions_2.c\
							hit_functions_box_1.c\
							hit_functions_box_2.c\
							hit_functions_sphere.c\
							hit_functions_plane.c\
							hit_functions_quad.c\
							hit_functions_ss_quad.c\
							hit_functions_cylinder.c\
							hit_functions_cone.c\
							img.c\
							img2.c\
							item_lamps.c\
							item_lamps2.c\
							materials.c\
							materials2.c\
							materials_scatter.c\
							math_1.c\
							math_2.c\
							math_3.c\
							math_4.c\
							math_5.c\
							math_matrix_1.c\
							math_matrix_2.c\
							math_matrix_3.c\
							math_projection_1.c\
							math_projection_2.c\
							math_quaternion.c\
							math_random_1.c\
							math_random_2.c\
							math_ray.c\
							math_reflection.c\
							math_trash.c\
							math_tweens_1.c\
							math_tweens_2.c\
							math_vector_1.c\
							math_vector_2.c\
							math_vector_3.c\
							math_vector_4.c\
							math_vector_5.c\
							math_vector_6.c\
							pbr_lighting_1.c\
							pbr_lighting_2.c\
							raster_1.c\
							raster_2.c\
							raster_functions_1.c\
							raster_functions_2.c\
							raster_heatmap.c\
							raytrace.c\
							raytrace_paint_1.c\
							raytrace_paint_2.c\
							raytrace_pixel.c\
							raytrace_tasks.c\
							raytrace_trace.c\
							scenes_1.c\
							scenes_2.c\
							scenes_3.c\
							scenes_4.c\
							scenes_5.c\
							scenes_6.c\
							scenes_7.c\
							simulation.c\
							textures_1.c\
							textures_2.c\
							textures_3.c\
							transform.c\
							trashcan_1.c\
							trashcan_2.c\
							trashcan_3.c\
							ui_debug.c\
							ui_help.c\
							update_camera_1.c\
							update_camera_2.c\
							update_camera_3.c\
							update_data.c\
							update_item_1.c\
							update_item_2.c\
							update_item_3.c\
							parsing.c\
							parsing_atoos_1.c\
							parsing_atoos_2.c\
							parsing_functions_1.c\
							parsing_functions_2.c\
							parsing_functions_3.c\
							parsing_functions_4.c


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
